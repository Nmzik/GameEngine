#pragma once
#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "FileType.h"

enum PsoSection
{
    PSIN = 0x5053494E,
    PMAP = 0x504D4150,
    PSCH = 0x50534348,
    PSIG = 0x50534947,
    STRF = 0x53545246,
    STRS = 0x53545253,
    STRE = 0x53545245,
    CHKS = 0x43484B53,
};

struct CHDTxdAssetBinding  // 132 bytes, Type:0
{
    uint8_t assetType;     //	0   assetType: BYTE_ENUM_VALUE: 0: 3387532954
    uint8_t Unused01;      //	1
    uint16_t Unused02;     //	2
    char targetAsset[64];  //	4   targetAsset: INT_0Bh: 4: 4194304
    char HDTxd[64];
};

class PsoDataMappingEntry
{
public:
    uint32_t NameHash;
    int Offset;
    int Unknown_8h;
    int Length;

    void Read(memstream& stream)
    {
        NameHash = *(uint32_t*)stream.read(sizeof(uint32_t));
        endSwap(&NameHash);
        Offset = *(int*)stream.read(sizeof(int));
        endSwap(&Offset);
        Unknown_8h = *(int*)stream.read(sizeof(int));
        endSwap(&Unknown_8h);
        Length = *(int*)stream.read(sizeof(int));
        endSwap(&Length);
    }
};

class PsoDataSection
{
public:
    uint32_t Ident;
    int Length;
    std::unique_ptr<uint8_t[]> Data;

    void Read(memstream& stream)
    {
        Ident = *(uint32_t*)stream.read(sizeof(uint32_t));
        Length = *(int*)stream.read(sizeof(int));
        endSwap(&Length);

        stream.seekCur(-8);
        Data = std::make_unique<uint8_t[]>(Length);
        memcpy(&Data[0], &stream.data[stream.tellg()], Length);
        stream.seekCur(Length);
    }
};

class PsoDataMapSection
{
public:
    int Ident;
    int Length;
    int RootId;
    short EntriesCount;
    short Unknown_Eh;
    std::unique_ptr<PsoDataMappingEntry[]> Entries;

    void Read(memstream& stream)
    {
        Ident = *(int*)stream.read(sizeof(int));
        endSwap(&Ident);
        Length = *(int*)stream.read(sizeof(int));
        endSwap(&Length);
        RootId = *(int*)stream.read(sizeof(int));
        endSwap(&RootId);
        EntriesCount = *(int16_t*)stream.read(sizeof(int16_t));
        endSwap(&EntriesCount);
        Unknown_Eh = *(int16_t*)stream.read(sizeof(int16_t));
        endSwap(&Unknown_Eh);

        Entries = std::make_unique<PsoDataMappingEntry[]>(EntriesCount);
        for (int i = 0; i < EntriesCount; i++)
        {
            Entries[i].Read(stream);
        }
    }
};

class RbfEntryDescription
{
public:
    std::string Name;
    int Type;
};
class IRbfType
{
    std::string Name;
};
class RbfBoolean : public IRbfType
{
public:
    std::string Name;
    bool Value;
};
class RbfBytes : public IRbfType
{
public:
    std::string Name;
    uint8_t* Value;  //byte[]
};
class RbfFloat : public IRbfType
{
public:
    std::string Name;
    float Value;
};
class RbfFloat3 : public IRbfType
{
public:
    std::string Name;
    float X;
    float Y;
    float Z;
};
class RbfString : public IRbfType
{
public:
    std::string Name;
    std::string Value;
};
class RbfStructure : public IRbfType
{
public:
    std::string Name;
    std::vector<IRbfType*> Children;  //List
};
class RbfUint32 : public IRbfType
{
public:
    std::string Name;
    uint32_t Value;
};

class YmfLoader
{
public:
    std::vector<CHDTxdAssetBinding*> HDtextures;
    RbfStructure* current = nullptr;
    std::stack<RbfStructure*> structures;
    std::vector<std::unique_ptr<RbfStructure>> vectorOfStructures;
    std::vector<RbfEntryDescription> descriptors;

    std::vector<std::pair<std::string, std::string>> gtxd;  //hack

    PsoDataSection _PsoDataSection;
    PsoDataMapSection _PsoDataMapSection;

    YmfLoader(memstream& stream);
    ~YmfLoader();

    void loadRBF(memstream& stream);
    void loadPSO(memstream& stream);

    void ParseElement(memstream& stream, int descriptorIndex, uint8_t dataType);

    uint8_t DetectType(memstream& stream);
};
