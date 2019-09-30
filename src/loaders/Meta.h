#pragma once
#include "FileType.h"

struct MetaStructureEntryInfo_s
{
    uint32_t EntryNameHash;
    int32_t DataOffset;
    uint8_t DataType;
    uint8_t Unknown_9h;
    int16_t ReferenceTypeIndex;
    uint32_t ReferenceKey;
};

struct MetaEnumEntryInfo_s
{
    uint32_t EntryNameHash;
    int32_t EntryValue;
};

class MetaStructureInfo
{
public:
    uint32_t StructureNameHash;
    uint32_t StructureKey;
    uint32_t Unknown_8h;
    uint32_t Unknown_Ch;  //	0x00000000;
    pgPtr<MetaStructureEntryInfo_s> entries;
    int32_t StructureSize;
    int16_t Unknown_1Ch;  //	0x0000;
    int16_t EntriesCount;

    void Resolve(memstream& file)
    {
        entries.Resolve(file);
    }
};

class MetaEnumInfo
{
public:
    uint32_t EnumNameHash;
    uint32_t EnumKey;
    pgPtr<MetaEnumEntryInfo_s> entries;
    int32_t EntriesCount;
    int32_t Unknown_14h;  //	0x00000000;

    void Resolve(memstream& file)
    {
        entries.Resolve(file);
    }
};

class MetaDataBlock
{
public:
    uint32_t StructureNameHash;
    int32_t DataLength;
    uint64_t DataPointer;

    void Resolve(memstream& file)
    {
        TranslatePTR(DataPointer);
    }
};

class Meta : ResourceFileBase
{
public:
    int32_t Unknown_10h;
    uint16_t Unknown_14h;
    uint8_t HasUselessData;
    uint8_t Unknown_17h;
    int32_t Unknown_18h;
    int32_t RootBlockIndex;
    MetaStructureInfo* StructureInfos;
    MetaEnumInfo* EnumInfos;
    MetaDataBlock* DataBlocks;
    uint64_t NamePointer;
    uint64_t UselessPointer;
    uint16_t StructureInfosCount;
    uint16_t EnumInfosCount;
    uint16_t DataBlocksCount;
    uint16_t Unknown_4Eh;
    uint32_t Unknown_50h;
    uint32_t Unknown_54h;
    uint32_t Unknown_58h;
    uint32_t Unknown_5Ch;
    uint32_t Unknown_60h;
    uint32_t Unknown_64h;
    uint32_t Unknown_68h;
    uint32_t Unknown_6Ch;

    void Resolve(memstream& file)
    {
        StructureInfos = (MetaStructureInfo*)&file.data[(uint64_t)StructureInfos & ~0x50000000];
        for (int i = 0; i < StructureInfosCount; i++)
        {
            StructureInfos[i].Resolve(file);
        }

        EnumInfos = (MetaEnumInfo*)&file.data[(uint64_t)EnumInfos & ~0x50000000];
        for (size_t i = 0; i < EnumInfosCount; i++)
        {
            EnumInfos[i].Resolve(file);
        }

        DataBlocks = (MetaDataBlock*)&file.data[(uint64_t)DataBlocks & ~0x50000000];
        for (int i = 0; i < DataBlocksCount; i++)
        {
            DataBlocks[i].Resolve(file);
        }
    }

    Meta() = delete;
};
