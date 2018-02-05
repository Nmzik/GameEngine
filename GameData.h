#pragma once
#include "RpfFile.h"
#include <algorithm>
#include <unordered_map>

class GameData
{
	std::vector<RpfFile*> RpfFiles;

public:
	std::unordered_map<uint32_t, RpfResourceFileEntry> YdrEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry> YddEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry> YtdEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry> YmapEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry> YbnEntries;

	GameData();
	~GameData();

	void LoadRpf(std::string& RpfPath);
	void LoadRpf(std::istream& rpf, std::string& FullPath_, std::string FileName_, uint32_t FileSize_, uint64_t FileOffset);

	void ExtractFileResource(RpfResourceFileEntry entry, std::vector<uint8_t>& output);

	uint32_t GenHash(std::string Name);
};

