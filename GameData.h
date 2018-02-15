#pragma once
#include "RpfFile.h"
#include <algorithm>
#include <unordered_map>
#include "tinyxml2.h"
#include "YtypLoader.h"

struct WaterQuad
{
	float minX;
	float maxX;
	float minY;
	float maxY;
	int Type;
	bool IsInvisible;
	bool HasLimitedDepth;
	float z;
	float a1;
	float a2;
	float a3;
	float a4;
	bool NoStencil;
};

class GameData
{
	std::vector<RpfFile*> RpfFiles;

public:
	std::vector<WaterQuad> WaterQuads;

	std::unordered_map<uint32_t, uint32_t> TextureDictionary; //FIRST - architextureName SECOND - textureDict

	std::unordered_map<uint32_t, RpfResourceFileEntry*> YdrEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry*> YddEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry*> YtdEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry*> YmapEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry*> YtypEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry*> YbnEntries;

	GameData();
	~GameData();

	void LoadWaterQuads();
	void LoadRpf(std::string& RpfPath);
	void LoadRpf(std::istream& rpf, std::string& FullPath_, std::string FileName_, uint32_t FileSize_, uint64_t FileOffset);

	void ExtractFileResource(RpfResourceFileEntry entry, std::vector<uint8_t>& output);

	uint32_t GenHash(std::string Name);
};

