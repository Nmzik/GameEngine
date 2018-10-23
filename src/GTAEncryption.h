#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "zlib.h"
#include "aes.h"

uint32_t GenHash(std::string Name);

class GTAEncryption
{
	static z_stream strm;

	static uint8_t PC_AES_KEY[32];
	static uint8_t LUT[256];
	static uint8_t PC_NG_KEYS[101][272];
	static uint32_t PC_NG_DECRYPT_TABLES[17][16][256];
public:
	static void LoadKeys();
	static void Cleanup();

	static uint32_t CalculateHash(std::string text);

	static uint8_t * GetNGKey(std::string name, uint32_t length);

	static void DecryptAES(uint8_t* data, uint32_t DataLength);
	static void DecryptNG(uint8_t* data, uint32_t dataLength, std::string name, uint32_t length);
	static void DecryptNG(uint8_t * data, uint32_t dataLength, uint8_t * key);
	static void DecryptNGBlock(uint8_t * data, uint32_t dataLength, uint32_t * key);
	static uint8_t * DecryptNGRoundA(uint8_t* data, uint32_t* key, uint32_t table[][256]);
	static uint8_t * DecryptNGRoundB(uint8_t * data, uint32_t * key, uint32_t table[][256]);

	static void DecompressBytes(uint8_t* data, uint32_t dataLength, std::vector<uint8_t>& output);
};

