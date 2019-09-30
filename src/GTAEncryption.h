#pragma once
#include <cassert>
#include <string>
#include <string_view>
#include "aes.hpp"
#include "zlib.h"

uint32_t GenHash(std::string_view Name);

class GTAEncryption
{
private:
    //zlib
    z_stream strm;
    //aes
    AES_ctx ctx;

    uint8_t PC_AES_KEY[32];
    uint8_t LUT[256];
    uint8_t PC_NG_KEYS[101][272];
    uint32_t PC_NG_DECRYPT_TABLES[17][16][256];
    uint32_t PC_AWC_KEY[4];

public:
    static GTAEncryption& getInstance()
    {
        return gtaEncryption;
    }

    static GTAEncryption gtaEncryption;

    GTAEncryption();
    ~GTAEncryption();

    void loadKeys(std::string keyPath);

    uint32_t calculateHash(std::string& text);

    uint8_t* getNGKey(std::string& name, uint32_t length);

    void decryptAES(uint8_t* data, uint32_t DataLength);
    void decrypt_RSXXTEA(uint8_t* data, size_t dataLength);
    void decryptNG(uint8_t* data, uint32_t dataLength, std::string& name, uint32_t length);
    void decryptNG(uint8_t* data, uint32_t dataLength, uint8_t* key);
    void decryptNGBlock(uint8_t* data, uint32_t dataLength, uint32_t* key);
    void decryptNGRoundA(uint8_t* data, uint32_t* key, uint32_t table[][256]);
    void decryptNGRoundB(uint8_t* data, uint32_t* key, uint32_t table[][256]);

    void decompressBytes(uint8_t* data, uint32_t dataLength, uint8_t* allocatedMem, uint32_t allocatedSize);
};
