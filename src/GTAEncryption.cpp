#include "GTAEncryption.h"

#include "aes.hpp"

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
    if (ret != Z_OK)
    {
        printf("ERROR IN ZLIB");
    }
}

GTAEncryption::~GTAEncryption()
{
    (void)inflateEnd(&strm);
}

void GTAEncryption::LoadKeys()
{
    uint8_t key[] = {0xB3, 0x89, 0x73, 0xAF, 0x8B, 0x9E, 0x26, 0x3A, 0x8D, 0xF1, 0x70, 0x32, 0x14, 0x42, 0xB3, 0x93,
                     0x8B, 0xD3, 0xF2, 0x1F, 0xA4, 0xD0, 0x4D, 0xFF, 0x88, 0x2E, 0x04, 0x66, 0x0F, 0xF9, 0x9D, 0xFD};
    std::memcpy(&PC_AES_KEY[0], &key[0], 32);

    uint8_t lutkey[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
        0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x5B, 0x2F, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
        0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
        0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
        0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
        0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF};
    std::memcpy(&LUT[0], &lutkey[0], 256);

    std::ifstream ngkeys("assets/gtav_ng_key.dat", std::ios::binary);

    if (!ngkeys.is_open())
    {
        printf("NOT FOUND NG!");
    }

    for (int i = 0; i < 101; i++)
    {
        ngkeys.read((char*)&PC_NG_KEYS[i], 272);
    }
    ngkeys.close();

    std::ifstream ngtables("assets/gtav_ng_decrypt_tables.dat", std::ios::binary);

    if (!ngtables.is_open())
    {
        printf("NOT FOUND NG TABLES!");
    }

    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            ngtables.read((char*)&PC_NG_DECRYPT_TABLES[i][j], 256 * sizeof(uint32_t));
        }
    }

    ngtables.close();
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
    struct AES_ctx ctx;
    AES_init_ctx(&ctx, PC_AES_KEY);
    for (uint32_t i = 0; i < DataLength / 16; i++)
    {
        AES_ECB_decrypt(&ctx, &data[i * 16]);
    }
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
        uint8_t encryptedBlock[16];
        memcpy(encryptedBlock, data + 16 * blockIndex, 16);
        decryptNGBlock(encryptedBlock, 16, (uint32_t*)&key[0]);
        memcpy(data + 16 * blockIndex, encryptedBlock, 16);
    }
}

void GTAEncryption::decryptNGBlock(uint8_t* data, uint32_t dataLength, uint32_t* key)
{
    data = decryptNGRoundA(data, &key[0], PC_NG_DECRYPT_TABLES[0]);
    data = decryptNGRoundA(data, &key[4], PC_NG_DECRYPT_TABLES[1]);
    for (int k = 2; k <= 15; k++)
        data = decryptNGRoundB(data, &key[4 * k], PC_NG_DECRYPT_TABLES[k]);
    data = decryptNGRoundA(data, &key[4 * 16], PC_NG_DECRYPT_TABLES[16]);
}

uint8_t* GTAEncryption::decryptNGRoundA(uint8_t* data, uint32_t* key, uint32_t table[][256])
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

    return data;
}

// round 3-15
uint8_t* GTAEncryption::decryptNGRoundB(uint8_t* data, uint32_t* key, uint32_t table[][256])
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

    return data;
}

void GTAEncryption::decompressBytes(uint8_t* data, uint32_t dataLength, uint8_t* AllocatedMem, uint64_t AllocatedSize)
{
    strm.avail_in = dataLength;
    strm.next_in = data;
    strm.avail_out = AllocatedSize;
    strm.next_out = &AllocatedMem[0];

    int ret = inflate(&strm, Z_FINISH);

    if (ret < 0)
    {
        printf("Error %d in zlib uncompress\n", ret);
    }

    (void)inflateReset(&strm);
}

/*
do {
ret = inflate(&strm, Z_NO_FLUSH);

if (ret < 0) {
(void)inflateReset(&strm);
printf("Error %d in zlib uncompress\n", ret);
}

if (ret != Z_STREAM_END) {
uint32_t oldSize = output.size();
output.resize(output.size() * 2);

strm.avail_out = oldSize;
strm.next_out = output.data() + oldSize;
}

} while (ret != Z_STREAM_END);
*/

uint32_t GenHash(std::string Name)
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
