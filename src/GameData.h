#pragma once
#include "RpfFile.h"
#include <algorithm>
#include <unordered_map>
#include "tinyxml2.h"
#include "glm/glm.hpp"

class RpfFile;
class YtypLoader;
class YftLoader;
class CacheDatFile;

struct CTimeArchetypeDef;
struct CBaseArchetypeDef;
class Archetype;

/*class RpfResourceFileEntry;
class RpfBinaryFileEntry*/

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

struct CarHandling
{
	uint32_t Hash;
	float mass;
	YftLoader* file;
};

class GameData
{
	std::vector<RpfFile*> RpfFiles;
	std::vector<std::ifstream*> Files;

public:
	std::vector<WaterQuad> WaterQuads;
	std::unique_ptr<CacheDatFile> cacheFile;

	std::unordered_map<uint32_t, Archetype*> Archetypes;
	//std::unordered_map<uint32_t, std::vector<CEntityDef>> MloDictionary;
	std::unordered_map<uint32_t, RpfResourceFileEntry*> YdrEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry*> YddEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry*> YtdEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry*> YftEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry*> YmapEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry*> YtypEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry*> YbnEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry*> YndEntries;
	std::unordered_map<uint32_t, RpfResourceFileEntry*> YnvEntries;

	std::unordered_map<uint32_t, uint32_t> GtxdEntries;
	std::vector<CarHandling> Vehicles;
	std::vector<glm::vec3> Scenes;

	GameData();
	~GameData();

	void LoadHandlingData(std::vector<uint8_t>& Buffer);
	void LoadGtxd();
	void LoadWaterQuads(std::vector<uint8_t>& Buffer);
	void LoadScenesSwitch(std::vector<uint8_t>& Buffer);

	void LoadRpf(std::ifstream& rpf, std::string& FullPath_, std::string& FileName_, uint32_t FileSize_, uint64_t FileOffset);

	void ExtractFileBinary(RpfBinaryFileEntry & entry, std::vector<uint8_t>& output);
	void ExtractFileResource(RpfResourceFileEntry& entry, std::vector<uint8_t>& output);
};

