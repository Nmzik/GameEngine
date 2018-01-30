#pragma once
#include "GameWorld.h"
#include "fstream"
#include <iostream>  
#include "membuf.h"
#include "GTAEncryption.h"
#include "RpfEntry.h"

class RpfFile
{
	std::string FileName;
	std::streampos FileSize;
	uint64_t startPos;

	uint32_t Version;
	uint32_t EntryCount;
	uint32_t NamesLength;
	uint32_t Encryption;
public:
	std::vector<RpfFile> SubFiles;

	std::vector<RpfResourceFileEntry> ResourceEntries;
	std::vector<RpfBinaryFileEntry> BinaryEntries;

	RpfFile(std::string FileName_);

	RpfFile(std::istream& rpf, std::string FileName_, uint32_t FileSize_, uint64_t FileOffset);

	void LoadRpf(std::istream& rpf);

	~RpfFile();
};

