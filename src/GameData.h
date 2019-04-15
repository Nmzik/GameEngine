#pragma once
#include <algorithm>
#include <memory>
#include <string_view>
#include <unordered_map>
#include "RpfFile.h"
#include "glm/glm.hpp"

class RpfFile;
class YtypLoader;
class YftLoader;
class CacheDatFile;

class fwArchetype;

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
    uint8_t* TempBuffer;

    std::vector<std::unique_ptr<RpfFile>> rpfFiles;
    std::vector<std::unique_ptr<std::ifstream>> openedFiles;

public:
    std::vector<WaterQuad> WaterQuads;
    std::unique_ptr<CacheDatFile> cacheFile;

    std::unordered_map<uint32_t, fwArchetype*> Archetypes;
    //	std::unordered_map<uint32_t, std::vector<fwEntityDef>> MloDictionary;
    std::unordered_map<uint32_t, RpfResourceFileEntry*> Entries[10];

    std::unordered_map<uint32_t, uint32_t> HDTextures;

    std::unordered_map<uint32_t, uint32_t> GtxdEntries;
    std::vector<CarHandling> VehiclesInfo;
    std::vector<glm::vec3> Scenes;

    GameData(std::string Path);
    ~GameData();

    void loadHandlingData(std::vector<uint8_t>& Buffer);
    void loadGtxd();
    void loadWaterQuads(std::vector<uint8_t>& Buffer);
    void loadScenesSwitch(std::vector<uint8_t>& Buffer);

    void loadRpf(std::ifstream& rpf, std::string& FullPath_, std::string& FileName_, uint32_t FileSize_, uint64_t FileOffset);

    void extractFileBinary(RpfBinaryFileEntry& entry, std::vector<uint8_t>& output);
    void extractFileResource(RpfResourceFileEntry& entry, uint8_t* AllocatedMem, uint64_t AllocatedSize);
};
