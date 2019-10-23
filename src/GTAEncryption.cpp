#include "GTAEncryption.h"
#include <stdexcept>
#include <string>
#include "utils/fileHandle.h"

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

GTAEncryption GTAEncryption::gtaEncryption;

GTAEncryption::GTAEncryption()
{
    //	ZLIB STUFF
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;

    int ret = inflateInit2(&strm, -MAX_WBITS);
    assert(ret == Z_OK);
}

GTAEncryption::~GTAEncryption()
{
    (void)inflateEnd(&strm);
}

void GTAEncryption::loadKeys(std::string keyPath)
{
#if TARGET_OS_IPHONE
    std::string Path = keyPath + "assets/magic.dat";
    const char* path = Path.c_str();
#else
    const char* path = "assets/magic.dat";
#endif
    FileHandle stream(path);

    if (!stream.isOpen())
    {
        throw std::runtime_error("KEY FILE IS MISSING\n");
    }

    stream.read((char*)&PC_AES_KEY[0], 32);
    stream.read((char*)&LUT[0], 256);
    for (int i = 0; i < 101; i++)
    {
        stream.read((char*)&PC_NG_KEYS[i], 272);
    }

    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            stream.read((char*)&PC_NG_DECRYPT_TABLES[i][j], 256 * sizeof(uint32_t));
        }
    }
    stream.read((char*)&PC_AWC_KEY[0], 16);

    AES_init_ctx(&ctx, PC_AES_KEY);
}

uint32_t GTAEncryption::calculateHash(std::string& text)
{
    uint32_t result = 0;
    for (int index = 0; index < text.size(); index++)
    {
        uint32_t temp = 1025 * (LUT[text[index]] + result);
        result = (temp >> 6) ^ temp;
    }

    return 32769 * ((9 * result >> 11) ^ (9 * result));
}

uint8_t* GTAEncryption::getNGKey(std::string& name, uint32_t length)
{
    uint32_t hash = calculateHash(name);
    uint32_t keyidx = (hash + length + (101 - 40)) % 0x65;
    return PC_NG_KEYS[keyidx];
}

void GTAEncryption::decryptAES(uint8_t* data, uint32_t DataLength)
{
    for (uint32_t i = 0; i < DataLength / 16; i++)
    {
        AES_ECB_decrypt(&ctx, &data[i * 16]);
    }
}

void GTAEncryption::decrypt_RSXXTEA(uint8_t* data, size_t dataLength)
{
    // Rockstar's modified version of XXTEA
    uint32_t* blocks = new uint32_t[dataLength / 4];
    memcpy(&blocks[0], &data[0], dataLength);

    int block_count = dataLength / 4;
    uint32_t a, b = blocks[0], i;

    i = (uint32_t)(0x9E3779B9 * (6 + 52 / block_count));
    do
    {
        for (int block_index = block_count - 1; block_index >= 0; --block_index)
        {
            a = blocks[(block_index > 0 ? block_index : block_count) - 1];
            b = blocks[block_index] -= (a >> 5 ^ b << 2) + (b >> 3 ^ a << 4) ^ (i ^ b) + (PC_AWC_KEY[block_index & 3 ^ (i >> 2 & 3)] ^ a ^ 0x7B3A207F);
        }
        i -= 0x9E3779B9;
    } while (i != 0);

    memcpy(&data[0], &blocks[0], dataLength);
}

void GTAEncryption::decryptNG(uint8_t* data, uint32_t dataLength, std::string& name, uint32_t length)
{
    uint8_t* key = getNGKey(name, length);
    decryptNG(data, dataLength, key);
}

void GTAEncryption::decryptNG(uint8_t* data, uint32_t dataLength, uint8_t* key)
{
    for (uint32_t blockIndex = 0; blockIndex < dataLength / 16; blockIndex++)
    {
        decryptNGBlock(data + 16 * blockIndex, 16, (uint32_t*)&key[0]);
    }
}

void GTAEncryption::decryptNGBlock(uint8_t* data, uint32_t dataLength, uint32_t* key)
{
    decryptNGRoundA(data, &key[0], PC_NG_DECRYPT_TABLES[0]);
    decryptNGRoundA(data, &key[4], PC_NG_DECRYPT_TABLES[1]);
    for (int k = 2; k <= 15; k++)
        decryptNGRoundB(data, &key[4 * k], PC_NG_DECRYPT_TABLES[k]);
    decryptNGRoundA(data, &key[4 * 16], PC_NG_DECRYPT_TABLES[16]);
}

void GTAEncryption::decryptNGRoundA(uint8_t* data, uint32_t* key, uint32_t table[][256])
{
    uint32_t x1 = table[0][data[0]] ^ table[1][data[1]] ^ table[2][data[2]] ^ table[3][data[3]] ^ key[0];
    uint32_t x2 = table[4][data[4]] ^ table[5][data[5]] ^ table[6][data[6]] ^ table[7][data[7]] ^ key[1];
    uint32_t x3 = table[8][data[8]] ^ table[9][data[9]] ^ table[10][data[10]] ^ table[11][data[11]] ^ key[2];
    uint32_t x4 = table[12][data[12]] ^ table[13][data[13]] ^ table[14][data[14]] ^ table[15][data[15]] ^ key[3];

    data[0] = (uint8_t)((x1 >> 0) & 0xFF);
    data[1] = (uint8_t)((x1 >> 8) & 0xFF);
    data[2] = (uint8_t)((x1 >> 16) & 0xFF);
    data[3] = (uint8_t)((x1 >> 24) & 0xFF);
    data[4] = (uint8_t)((x2 >> 0) & 0xFF);
    data[5] = (uint8_t)((x2 >> 8) & 0xFF);
    data[6] = (uint8_t)((x2 >> 16) & 0xFF);
    data[7] = (uint8_t)((x2 >> 24) & 0xFF);
    data[8] = (uint8_t)((x3 >> 0) & 0xFF);
    data[9] = (uint8_t)((x3 >> 8) & 0xFF);
    data[10] = (uint8_t)((x3 >> 16) & 0xFF);
    data[11] = (uint8_t)((x3 >> 24) & 0xFF);
    data[12] = (uint8_t)((x4 >> 0) & 0xFF);
    data[13] = (uint8_t)((x4 >> 8) & 0xFF);
    data[14] = (uint8_t)((x4 >> 16) & 0xFF);
    data[15] = (uint8_t)((x4 >> 24) & 0xFF);
}

// round 3-15
void GTAEncryption::decryptNGRoundB(uint8_t* data, uint32_t* key, uint32_t table[][256])
{
    uint32_t x1 = table[0][data[0]] ^ table[7][data[7]] ^ table[10][data[10]] ^ table[13][data[13]] ^ key[0];
    uint32_t x2 = table[1][data[1]] ^ table[4][data[4]] ^ table[11][data[11]] ^ table[14][data[14]] ^ key[1];
    uint32_t x3 = table[2][data[2]] ^ table[5][data[5]] ^ table[8][data[8]] ^ table[15][data[15]] ^ key[2];
    uint32_t x4 = table[3][data[3]] ^ table[6][data[6]] ^ table[9][data[9]] ^ table[12][data[12]] ^ key[3];

    data[0] = (uint8_t)((x1 >> 0) & 0xFF);
    data[1] = (uint8_t)((x1 >> 8) & 0xFF);
    data[2] = (uint8_t)((x1 >> 16) & 0xFF);
    data[3] = (uint8_t)((x1 >> 24) & 0xFF);
    data[4] = (uint8_t)((x2 >> 0) & 0xFF);
    data[5] = (uint8_t)((x2 >> 8) & 0xFF);
    data[6] = (uint8_t)((x2 >> 16) & 0xFF);
    data[7] = (uint8_t)((x2 >> 24) & 0xFF);
    data[8] = (uint8_t)((x3 >> 0) & 0xFF);
    data[9] = (uint8_t)((x3 >> 8) & 0xFF);
    data[10] = (uint8_t)((x3 >> 16) & 0xFF);
    data[11] = (uint8_t)((x3 >> 24) & 0xFF);
    data[12] = (uint8_t)((x4 >> 0) & 0xFF);
    data[13] = (uint8_t)((x4 >> 8) & 0xFF);
    data[14] = (uint8_t)((x4 >> 16) & 0xFF);
    data[15] = (uint8_t)((x4 >> 24) & 0xFF);
}

void GTAEncryption::decompressBytes(uint8_t* data, uint32_t dataLength, uint8_t* allocatedMem, uint32_t allocatedSize)
{
    strm.avail_in = dataLength;
    strm.next_in = data;
    strm.avail_out = allocatedSize;
    strm.next_out = &allocatedMem[0];

    int ret = inflate(&strm, Z_FINISH);

    if (ret < 0)
    {
        printf("Error %d in zlib uncompress\n", ret);
    }

    (void)inflateReset(&strm);
}

uint32_t GenHash(std::string_view Name)
{
    uint32_t h = 0;
    for (int i = 0; i < Name.size(); i++)
    {
        h += (uint8_t)Name[i];
        h += (h << 10);
        h ^= (h >> 6);
    }
    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15);

    return h;
}
