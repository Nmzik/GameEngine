#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include "membuf.h"
#include "RpfEntry.h"
#include <vector>

class RpfFile
{
public:
	std::ifstream* rpf;
	bool IsAESEncrypted = false;

	std::vector<RpfResourceFileEntry> ResourceEntries;
	std::vector<RpfBinaryFileEntry> BinaryEntries;

	RpfFile(std::ifstream& rpf, std::string& FullPath_, std::string& FileName_, uint32_t FileSize_, uint64_t FileOffset);

	void LoadRpf(std::ifstream& rpf, std::string& FileName, uint32_t FileSize, std::string& FullPath);

	~RpfFile();
};
