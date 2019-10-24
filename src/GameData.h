#pragma once
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <glm/vec3.hpp>

#include "utils/fileHandle.h"

class RpfFile;
class RpfBinaryFileEntry;
class RpfResourceFileEntry;
class YtypLoader;
class YftLoader;
class YndLoader;
class AwcLoader;
class CacheDatFile;

class fwArchetype;
struct fwEntityDef;

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
    std::unique_ptr<uint8_t[]> tempBuffer;
    std::string mainDirPath;
    std::vector<std::unique_ptr<RpfFile>> rpfFiles;
    std::vector<std::unique_ptr<FileHandle>> openedFiles;

public:
    std::vector<WaterQuad> waterQuads;
    std::unique_ptr<CacheDatFile> cacheFile;

    std::unordered_map<uint32_t, fwArchetype*> archetypes;
    std::unordered_map<uint32_t, std::vector<fwEntityDef>> mloDictionary;
    std::unordered_map<uint32_t, RpfResourceFileEntry*> entries[10];
    std::unordered_map<uint32_t, RpfBinaryFileEntry*> audios;

    std::unordered_map<uint32_t, uint32_t> hdTextures;
    std::vector<std::unique_ptr<YndLoader>> nodes;
    std::unordered_map<uint32_t, uint32_t> gtxdEntries;
    std::vector<CarHandling> vehiclesInfo;
    std::vector<glm::vec3> scenes;
    std::vector<WaterQuad*> waterPosition;

    GameData(std::string path);
    ~GameData();

    void load();
    void loadHandlingData(std::vector<uint8_t>& buffer);
    void loadGtxd(std::vector<uint8_t>& Buffer);
    void loadWaterQuads(std::vector<uint8_t>& buffer);
    void loadScenesSwitch(std::vector<uint8_t>& buffer);

    void loadRpf(FileHandle& rpf, std::string& FullPath_, std::string& FileName_, uint32_t FileSize_, uint64_t FileOffset);

    void extractFileBinary(RpfBinaryFileEntry& entry, uint8_t* allocatedMem, uint32_t allocatedSize);
    void extractFileResource(RpfResourceFileEntry& entry, uint8_t* allocatedMem, uint32_t allocatedSize);
};
