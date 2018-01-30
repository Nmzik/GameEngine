#pragma once
#include "GameWorld.h"

class GTAEncryption
{
	static uint8_t LUT[256];
	static uint8_t PC_NG_KEYS[101][272];
	static uint32_t PC_NG_DECRYPT_TABLES[17][16][256];
public:
	static void LoadKeys();

	static uint32_t CalculateHash(std::string text);

	static uint8_t * GetNGKey(std::string name, uint32_t length);

	static uint8_t * DecryptNG(uint8_t* data, uint32_t dataLength, std::string name, uint32_t length);
	static uint8_t * DecryptNG(uint8_t * data, uint32_t dataLength, uint8_t * key);
	static uint8_t * DecryptNGBlock(uint8_t * data, uint32_t dataLength, uint32_t * key);
	static uint8_t * DecryptNGRoundA(uint8_t* data, uint32_t* key, uint32_t table[][256]);
	static uint8_t * DecryptNGRoundB(uint8_t * data, uint32_t * key, uint32_t table[][256]);
};
