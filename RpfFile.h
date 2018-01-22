#pragma once
#include "GameWorld.h"
#include "fstream"

class RpfFile
{
	uint32_t Version;
	uint32_t EntryCount;
	uint32_t NamesLength;
	uint32_t Encryption;
public:
	RpfFile(std::string Path);

	uint8_t* DecryptNG(uint8_t* data, uint32_t dataLength, uint8_t* key, uint32_t keyLength);
	~RpfFile();
};

