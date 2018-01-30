#include "GTAEncryption.h"

uint8_t GTAEncryption::LUT[256];
uint8_t GTAEncryption::PC_NG_KEYS[101][272];
uint32_t GTAEncryption::PC_NG_DECRYPT_TABLES[17][16][256];

void GTAEncryption::LoadKeys()
{
	std::ifstream lutFile("C:\\Users\\nmzik\\source\\repos\\MyGameEngine\\MyGameEngine\\gtav_hash_lut.dat", std::ios::binary);

	if (!lutFile.is_open()) {
		printf("NOT FOUND LUT!");
	}

	lutFile.read((char*)&LUT[0], 256);
	lutFile.close();

	std::ifstream ngkeys("C:\\Users\\nmzik\\source\\repos\\MyGameEngine\\MyGameEngine\\gtav_ng_key.dat", std::ios::binary);

	if (!ngkeys.is_open()) {
		printf("NOT FOUND NG!");
	}

	for (int i = 0; i < 101; i++)
	{
		ngkeys.read((char*)&PC_NG_KEYS[i], 272);
	}
	ngkeys.close();

	std::ifstream ngtables("C:\\Users\\nmzik\\source\\repos\\MyGameEngine\\MyGameEngine\\gtav_ng_decrypt_tables.dat", std::ios::binary);

	if (!ngtables.is_open()) {
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

uint32_t GTAEncryption::CalculateHash(std::string text)
{
	uint32_t result = 0;
	for (int index = 0; index < text.size(); index++)
	{
		uint32_t temp = 1025 * (LUT[text[index]] + result);
		result = (temp >> 6) ^ temp;
	}

	return 32769 * ((9 * result >> 11) ^ (9 * result));
}

uint8_t* GTAEncryption::GetNGKey(std::string name, uint32_t length)
{
	uint32_t hash = CalculateHash(name);
	uint32_t keyidx = (hash + length + (101 - 40)) % 0x65;
	return PC_NG_KEYS[keyidx];
}

uint8_t* GTAEncryption::DecryptNG(uint8_t* data, uint32_t dataLength, std::string name, uint32_t length)
{
	uint8_t* key = GetNGKey(name, length);
	return DecryptNG(data, dataLength, key);
}

uint8_t* GTAEncryption::DecryptNG(uint8_t* data, uint32_t dataLength, uint8_t* key)
{
	uint8_t* decryptedData = new uint8_t[dataLength];

	uint32_t* keyuints = new uint32_t[272 / 4]; //KEYLENGTH HARDCODED
	memcpy(keyuints, key, 272);
	//Buffer.BlockCopy(key, 0, keyuints, 0, key.Length);

	for (int blockIndex = 0; blockIndex < dataLength / 16; blockIndex++)
	{
		uint8_t* encryptedBlock = new uint8_t[16];
		memcpy(encryptedBlock, data + 16 * blockIndex, 16);
		uint8_t* decryptedBlock = DecryptNGBlock(encryptedBlock, 16, keyuints);
		memcpy(decryptedData + 16 * blockIndex, decryptedBlock, 16);
	}

	if (dataLength % 16 != 0)
	{
		uint32_t left = dataLength % 16;
		memcpy(decryptedData + dataLength - left, data + dataLength - left, left);
		//Buffer.BlockCopy(data, data.Length - left, decryptedData, dataLength - left, left);
	}

	return decryptedData;
}

uint8_t* GTAEncryption::DecryptNGBlock(uint8_t* data, uint32_t dataLength, uint32_t* key)
{
	uint8_t* buffer = new uint8_t[dataLength];
	memcpy(buffer, data, dataLength);

	// prepare key...
	uint32_t* subKeys[17];
	for (int i = 0; i < 17; i++)
	{
		subKeys[i] = new uint32_t[4];
		subKeys[i][0] = key[4 * i + 0];
		subKeys[i][1] = key[4 * i + 1];
		subKeys[i][2] = key[4 * i + 2];
		subKeys[i][3] = key[4 * i + 3];
	}

	buffer = DecryptNGRoundA(buffer, subKeys[0], PC_NG_DECRYPT_TABLES[0]);
	buffer = DecryptNGRoundA(buffer, subKeys[1], PC_NG_DECRYPT_TABLES[1]);
	for (int k = 2; k <= 15; k++)
	buffer = DecryptNGRoundB(buffer, subKeys[k], PC_NG_DECRYPT_TABLES[k]);
	buffer = DecryptNGRoundA(buffer, subKeys[16], PC_NG_DECRYPT_TABLES[16]);

	return buffer;
}

uint8_t* GTAEncryption::DecryptNGRoundA(uint8_t* data, uint32_t* key, uint32_t table[][256])
{
	uint32_t x1 =
		table[0][data[0]] ^
		table[1][data[1]] ^
		table[2][data[2]] ^
		table[3][data[3]] ^
		key[0];
	uint32_t x2 =
		table[4][data[4]] ^
		table[5][data[5]] ^
		table[6][data[6]] ^
		table[7][data[7]] ^
		key[1];
	uint32_t x3 =
		table[8][data[8]] ^
		table[9][data[9]] ^
		table[10][data[10]] ^
		table[11][data[11]] ^
		key[2];
	uint32_t x4 =
		table[12][data[12]] ^
		table[13][data[13]] ^
		table[14][data[14]] ^
		table[15][data[15]] ^
		key[3];

	uint8_t* result = new uint8_t[16];
	result[0] = (uint8_t)((x1 >> 0) & 0xFF);
	result[1] = (uint8_t)((x1 >> 8) & 0xFF);
	result[2] = (uint8_t)((x1 >> 16) & 0xFF);
	result[3] = (uint8_t)((x1 >> 24) & 0xFF);
	result[4] = (uint8_t)((x2 >> 0) & 0xFF);
	result[5] = (uint8_t)((x2 >> 8) & 0xFF);
	result[6] = (uint8_t)((x2 >> 16) & 0xFF);
	result[7] = (uint8_t)((x2 >> 24) & 0xFF);
	result[8] = (uint8_t)((x3 >> 0) & 0xFF);
	result[9] = (uint8_t)((x3 >> 8) & 0xFF);
	result[10] = (uint8_t)((x3 >> 16) & 0xFF);
	result[11] = (uint8_t)((x3 >> 24) & 0xFF);
	result[12] = (uint8_t)((x4 >> 0) & 0xFF);
	result[13] = (uint8_t)((x4 >> 8) & 0xFF);
	result[14] = (uint8_t)((x4 >> 16) & 0xFF);
	result[15] = (uint8_t)((x4 >> 24) & 0xFF);;
	return result;
}



// round 3-15
uint8_t* GTAEncryption::DecryptNGRoundB(uint8_t* data, uint32_t* key, uint32_t table[][256])
{
	uint32_t x1 =
		table[0][data[0]] ^
		table[7][data[7]] ^
		table[10][data[10]] ^
		table[13][data[13]] ^
		key[0];
	uint32_t x2 =
		table[1][data[1]] ^
		table[4][data[4]] ^
		table[11][data[11]] ^
		table[14][data[14]] ^
		key[1];
	uint32_t x3 =
		table[2][data[2]] ^
		table[5][data[5]] ^
		table[8][data[8]] ^
		table[15][data[15]] ^
		key[2];
	uint32_t x4 =
		table[3][data[3]] ^
		table[6][data[6]] ^
		table[9][data[9]] ^
		table[12][data[12]] ^
		key[3];

	uint8_t* result = new uint8_t[16];
	result[0] = (uint8_t)((x1 >> 0) & 0xFF);
	result[1] = (uint8_t)((x1 >> 8) & 0xFF);
	result[2] = (uint8_t)((x1 >> 16) & 0xFF);
	result[3] = (uint8_t)((x1 >> 24) & 0xFF);
	result[4] = (uint8_t)((x2 >> 0) & 0xFF);
	result[5] = (uint8_t)((x2 >> 8) & 0xFF);
	result[6] = (uint8_t)((x2 >> 16) & 0xFF);
	result[7] = (uint8_t)((x2 >> 24) & 0xFF);
	result[8] = (uint8_t)((x3 >> 0) & 0xFF);
	result[9] = (uint8_t)((x3 >> 8) & 0xFF);
	result[10] = (uint8_t)((x3 >> 16) & 0xFF);
	result[11] = (uint8_t)((x3 >> 24) & 0xFF);
	result[12] = (uint8_t)((x4 >> 0) & 0xFF);
	result[13] = (uint8_t)((x4 >> 8) & 0xFF);
	result[14] = (uint8_t)((x4 >> 16) & 0xFF);
	result[15] = (uint8_t)((x4 >> 24) & 0xFF);
	return result;
}