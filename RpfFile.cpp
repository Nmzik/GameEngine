#include "RpfFile.h"

RpfFile::RpfFile(std::string Path)
{
	std::ifstream rpf(Path);

	if (!rpf.is_open()) {
		printf("NOT FOUND!");
	}

	rpf.read((char*)&Version, sizeof(uint32_t));
	rpf.read((char*)&EntryCount, sizeof(uint32_t));
	rpf.read((char*)&NamesLength, sizeof(uint32_t));
	rpf.read((char*)&Encryption, sizeof(uint32_t));

	if (Version != 0x52504637) {
		printf("SOMETHING WRONG");
		return;
	}

	uint8_t* entriesData = new uint8_t[EntryCount * 16];
	uint8_t* namesData = new uint8_t[NamesLength];

	switch (Encryption)
	{
	case 268435449:

	default:
		break;
	}

	for (int i = 0; i < EntryCount; i++)
	{

	}
}

uint8_t* RpfFile::DecryptNG(uint8_t* data, uint32_t dataLength, uint8_t* key, uint32_t keyLength)
{
	uint8_t* decryptedData = new uint8_t[dataLength];

	uint32_t* keyuints = new uint32_t[keyLength / 4];
	memcpy(keyuints, key, keyLength);
	//Buffer.BlockCopy(key, 0, keyuints, 0, key.Length);

	for (int blockIndex = 0; blockIndex < dataLength / 16; blockIndex++)
	{
		/*uint8_t* encryptedBlock = new uint8_t[16];
		Array.Copy(data, 16 * blockIndex, encryptedBlock, 0, 16);
		uint8_t decryptedBlock = DecryptNGBlock(encryptedBlock, keyuints);
		Array.Copy(decryptedBlock, 0, decryptedData, 16 * blockIndex, 16);*/
	}

	if (dataLength % 16 != 0)
	{
		uint32_t left = dataLength % 16;
		memcpy(decryptedData + dataLength - left, data + dataLength - left, left);
		//Buffer.BlockCopy(data, data.Length - left, decryptedData, dataLength - left, left);
	}

	return decryptedData;
}

uint8_t* DecryptNGBlock(uint8_t* data, uint32_t dataLength, uint32_t* key)
{
	uint8_t* buffer = new uint8_t(dataLength);
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

	/*buffer = DecryptNGRoundA(buffer, subKeys[0], GTA5Keys.PC_NG_DECRYPT_TABLES[0]);
	buffer = DecryptNGRoundA(buffer, subKeys[1], GTA5Keys.PC_NG_DECRYPT_TABLES[1]);
	for (int k = 2; k <= 15; k++)
		buffer = DecryptNGRoundB(buffer, subKeys[k], GTA5Keys.PC_NG_DECRYPT_TABLES[k]);
	buffer = DecryptNGRoundA(buffer, subKeys[16], GTA5Keys.PC_NG_DECRYPT_TABLES[16]);*/

	return buffer;
}


// round 1,2,16
uint8_t* DecryptNGRoundA(uint8_t* data, uint32_t* key, uint32_t** table)
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
uint8_t* DecryptNGRoundB(uint8_t* data, uint32_t* key, uint32_t** table)
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

RpfFile::~RpfFile()
{

}
