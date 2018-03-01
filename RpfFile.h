#pragma once
#include "fstream"
#include <iostream>  
#include <string>
#include "membuf.h"
#include "GTAEncryption.h"
#include "RpfEntry.h"

class RpfFile
{
public:
	uint64_t startPos;

	std::vector<RpfResourceFileEntry> ResourceEntries;
	std::vector<RpfBinaryFileEntry> BinaryEntries;

	RpfFile(std::istream& rpf, std::string& FileName_);

	RpfFile(std::istream& rpf, std::string& FullPath_, std::string& FileName_, uint32_t FileSize_, uint64_t FileOffset);

	void LoadRpf(std::istream& rpf, std::string& FileName, uint32_t FileSize, std::string& FullPath);

	~RpfFile();
};

