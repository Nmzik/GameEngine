#pragma once
#include "RpfEntry.h"
#include "membuf.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class RpfFile
{
	public:
	std::ifstream* rpf;
	bool IsAESEncrypted = false;

	std::vector<RpfResourceFileEntry> ResourceEntries;
	std::vector<RpfBinaryFileEntry> BinaryEntries;

	RpfFile(std::ifstream& rpf, std::string& FullPath, std::string& FileName, uint32_t FileSize, uint64_t FileOffset);

	~RpfFile();
};
