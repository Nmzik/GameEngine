#include "GTAEncryption.h"

z_stream GTAEncryption::strm;
uint8_t GTAEncryption::PC_AES_KEY[32];
uint8_t GTAEncryption::LUT[256];
uint8_t GTAEncryption::PC_NG_KEYS[101][272];
uint32_t GTAEncryption::PC_NG_DECRYPT_TABLES[17][16][256];

void GTAEncryption::LoadKeys()
{
	std::ifstream aesFile("gtav_aes_key.dat", std::ios::binary);

	if (!aesFile.is_open()) {
		printf("NOT FOUND AES!");
	}

	aesFile.read((char*)&PC_AES_KEY[0], 32);
	aesFile.close();

	std::ifstream lutFile("gtav_hash_lut.dat", std::ios::binary);

	if (!lutFile.is_open()) {
		printf("NOT FOUND LUT!");
	}

	lutFile.read((char*)&LUT[0], 256);
	lutFile.close();

	std::ifstream ngkeys("gtav_ng_key.dat", std::ios::binary);

	if (!ngkeys.is_open()) {
		printf("NOT FOUND NG!");
	}

	for (int i = 0; i < 101; i++)
	{
		ngkeys.read((char*)&PC_NG_KEYS[i], 272);
	}
	ngkeys.close();

	std::ifstream ngtables("gtav_ng_decrypt_tables.dat", std::ios::binary);

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


	//ZLIB STUFF
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.next_in = Z_NULL;
	strm.avail_in = 0;

	int ret = inflateInit2(&strm, -MAX_WBITS);
	if (ret != Z_OK) {
		printf("ERROR IN ZLIB");
	}
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

uint8_t * GTAEncryption::DecryptAES(uint8_t * data, uint32_t DataLength)
{
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, PC_AES_KEY);
	for (uint32_t i = 0; i < DataLength / 16; i++)
	{
		AES_ECB_decrypt(&ctx, &data[i * 16]);
	}

	return data;
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

	for (uint32_t blockIndex = 0; blockIndex < dataLength / 16; blockIndex++)
	{
		uint8_t* encryptedBlock = new uint8_t[16];
		memcpy(encryptedBlock, data + 16 * blockIndex, 16);
		uint8_t* decryptedBlock = DecryptNGBlock(encryptedBlock, 16, keyuints);
		memcpy(decryptedData + 16 * blockIndex, decryptedBlock, 16);

		delete[] decryptedBlock;
	}

	if (dataLength % 16 != 0)
	{
		uint32_t left = dataLength % 16;
		memcpy(decryptedData + dataLength - left, data + dataLength - left, left);
		//Buffer.BlockCopy(data, data.Length - left, decryptedData, dataLength - left, left);
	}

	delete[] data;
	delete[] keyuints;
	return decryptedData;
}

uint8_t* GTAEncryption::DecryptNGBlock(uint8_t* data, uint32_t dataLength, uint32_t* key)
{
	uint8_t* buffer = new uint8_t[dataLength];
	memcpy(buffer, data, dataLength);
	
	delete[] data;

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


	for (int i = 0; i < 17; i++)
	{
		delete[] subKeys[i];
	}

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
	result[15] = (uint8_t)((x4 >> 24) & 0xFF);

	delete[] data;

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

	delete[] data;

	return result;
}

void GTAEncryption::DecompressBytes(uint8_t * data, uint32_t dataLength, std::vector<uint8_t>& output)
{
	strm.avail_in = dataLength;
	strm.next_in = data;
	strm.avail_out = output.size();
	strm.next_out = &output[0];

	int ret = inflate(&strm, Z_FINISH);

	if (ret < 0) {
		(void)inflateReset(&strm);
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