#pragma once
#include "FileType.h"

struct Sequence
{
    // structure data
    uint32_t Unknown_00h;  //identifier / name?
    uint32_t DataLength;
    uint32_t Unused_08h;                    // 0x00000000
    uint32_t FrameOffset;                   //offset to frame data items / bytes
    uint32_t RootMotionRefsOffset;          //offset to root motion items (relative to start of the chunk, -32), ==BlockLength when no root motion
    uint16_t Unused_14h;                    //0x0000
    uint16_t NumFrames;                     // count of frame data items
    uint16_t FrameLength;                   //stride of frame data item
    uint16_t IndirectQuantizeFloatNumInts;  //total number of ints that the indirect quantize float channels take (not the frames data though!)
    uint16_t QuantizeFloatValueBits;        //total number of quantize float value bits per frame?
    uint8_t ChunkSize;                      //64|255                 0x40|0xFF
    uint8_t RootMotionRefCounts;            //0|17|20|21|49|52|53    0x11|0x14|0x15|0x31|0x34|0x35

    void Resolve(memstream& file)
    {
    }
};

struct AnimationBoneId
{
    uint16_t BoneId;
    uint8_t Unk0;
    uint8_t Track;
};

struct Animation1
{
    // structure data
    uint32_t VFT;
    uint32_t Unknown_04h;  // 0x00000001
    uint32_t Unused_08h;   // 0x00000000
    uint32_t Unused_0Ch;   // 0x00000000
    uint8_t Unknown_10h;
    uint8_t Unknown_11h;
    uint16_t Unused_12h;  // 0x0000
    uint16_t Frames;
    uint16_t SequenceFrameLimit;
    float Duration;
    uint32_t Unknown_1Ch;
    uint32_t Unused_20h;  // 0x00000000
    uint32_t Unused_24h;  // 0x00000000
    uint32_t Unused_28h;  // 0x00000000
    uint32_t Unused_2Ch;  // 0x00000000
    uint32_t Unused_30h;  // 0x00000000
    uint32_t Unused_34h;  // 0x00000000
    uint32_t MaxSeqBlockLength;
    uint32_t UsageCount;
    pgObjectArray<Sequence> Sequences;
    pgArray<AnimationBoneId> BoneIds;

    void Resolve(memstream& file)
    {
        //not sure size of class

        Sequences.Resolve(file);
        BoneIds.Resolve(file);
    }
};

struct AnimationMapEntry
{
    // structure data
    uint32_t Hash;
    uint32_t Unknown_04h;  // 0x00000000
    pgPtr<Animation1> AnimationPtr;
    pgPtr<AnimationMapEntry> NextEntryPtr;
    uint32_t Unknown_18h;  // 0x00000000
    uint32_t Unknown_1Ch;  // 0x00000000

    void Resolve(memstream& file)
    {
        AnimationPtr.Resolve(file);
        AnimationPtr->Resolve(file);

        NextEntryPtr.Resolve(file);
        NextEntryPtr->Resolve(file);
    };
};

struct AnimationMap
{
    // structure data
    uint32_t VFT;
    uint32_t Unknown_04h;
    uint32_t Unknown_08h;
    uint32_t Unknown_0Ch;
    uint32_t Unknown_10h;
    uint32_t Unknown_14h;
    pgObjectArray<AnimationMapEntry> AnimationsPointer;
    uint32_t Unknown_24h;
    uint32_t Unknown_28h;
    uint32_t Unknown_2Ch;

    void Resolve(memstream& file)
    {
        AnimationsPointer.Resolve(file);
    }
};

enum ClipPropertyAttributeType : uint8_t
{
    Float = 1,
    Int = 2,
    Bool = 3,
    String = 4,
    Vector3 = 6,
    Vector4 = 8,
    HashString = 12,
};

struct ClipPropertyAttribute
{
    uint32_t VFT;
    uint32_t Unknown_04h;
    ClipPropertyAttributeType Type;
    uint8_t Unknown_09h;   // 0x00
    uint16_t Unknown_Ah;   // 0x0000
    uint32_t Unknown_Ch;   // 0x00000000
    uint32_t Unknown_10h;  // 0x00000000
    uint32_t Unknown_14h;  // 0x00000000
    uint32_t NameHash;
    uint32_t Unknown_1Ch;  // 0x00000000

    void Resolve(memstream& file)
    {
    }
};

struct ClipProperty
{
    uint32_t VFT;
    uint32_t Unknown_04h;
    uint32_t Unknown_08h;  // 0x00000000
    uint32_t Unknown_0Ch;  // 0x00000000
    uint32_t Unknown_10h;  // 0x00000000
    uint32_t Unknown_14h;  // 0x00000000
    uint32_t NameHash;
    uint32_t Unknown_1Ch;  // 0x00000000
    pgObjectArray<ClipPropertyAttribute> AttributesPointer;
    uint32_t Unknown_2Ch;  // 0x00000000
    uint32_t Unknown_30h;  // 0x00000000
    uint32_t Unknown_34h;  // 0x00000000
    uint32_t UnkHash;
    uint32_t Unknown_3Ch;  // 0x00000000

    void Resolve(memstream& file)
    {
        AttributesPointer.Resolve(file);
    }
};

struct ClipTagList;
struct ClipTag;

struct ClipTagList
{
    // structure data
    pgObjectArray<ClipTag> TagsPointer;
    uint32_t Unknown_0Ch;  // 0x00000000
    uint32_t HasBlockTag;  // 0, 1
    uint32_t Unknown_14h;  // 0x00000000
    uint32_t Unknown_18h;  // 0x00000000
    uint32_t Unknown_1Ch;  // 0x00000000

    void Resolve(memstream& file)
    {
        TagsPointer.Resolve(file);
    }
};

struct ClipTag : ClipProperty
{
    float Unknown_40h;
    float Unknown_44h;
    pgPtr<ClipTagList> TagsPointer;

    void Resolve(memstream& file)
    {
        ClipProperty::Resolve(file);

        TagsPointer.Resolve(file);
        TagsPointer->Resolve(file);
    }
};

enum ClipType : uint32_t
{
    Animation = 1,
    AnimationList = 2,
};

struct ClipPropertyMapEntry;

struct ClipPropertyMap
{
    // structure data
    pgObjectArray<ClipPropertyMapEntry> PropertyEntriesPointer;
    uint32_t Unknown_0Ch;

    void Resolve(memstream& file)
    {
        PropertyEntriesPointer.Resolve(file);
    }
};

struct ClipBase
{
    // structure data
    uint32_t VFT;
    uint32_t Unknown_04h;
    uint32_t Unknown_08h;  // 0x00000000
    uint32_t Unknown_0Ch;  // 0x00000000
    ClipType Type;         // 1, 2
    uint32_t Unknown_14h;  // 0x00000000
    pgPtr<char> NamePointer;
    uint16_t NameLength;    // short, name length
    uint16_t NameCapacity;  // short, name length +1
    uint32_t Unknown_24h;   // 0x00000000
    uint64_t Unknown_28hPtr;
    uint32_t Unknown_30h;  // 0, 1
    uint32_t Unknown_34h;  // 0x00000000
    pgPtr<ClipTagList> TagsPointer;
    pgPtr<ClipPropertyMap> PropertiesPointer;
    uint32_t Unknown_48h;
    uint32_t Unknown_4Ch;  // 0x00000000

    void Resolve(memstream& file)
    {
        NamePointer.Resolve(file);

        TagsPointer.Resolve(file);
        TagsPointer->Resolve(file);

        PropertiesPointer.Resolve(file);
        PropertiesPointer->Resolve(file);
    }
};

struct ClipMapEntry
{
    // structure data
    uint32_t Hash;
    uint32_t Unknown_04h;  // 0x00000000
    pgPtr<ClipBase> ClipPointer;
    pgPtr<ClipMapEntry> NextPointer;
    uint32_t Unknown_18h;  // 0x00000000
    uint32_t Unknown_1Ch;  // 0x00000000

    void Resolve(memstream& file)
    {
        ClipPointer.Resolve(file);
        ClipPointer->Resolve(file);

        NextPointer.Resolve(file);
        NextPointer->Resolve(file);
    };
};

struct ClipPropertyMapEntry
{
    // structure data
    uint32_t PropertyNameHash;
    uint32_t Unknown_04h;  // 0x00000000
    pgPtr<ClipProperty> DataPointer;
    pgPtr<ClipPropertyMapEntry> NextPointer;
    uint32_t Unknown_18h;  // 0x00000000
    uint32_t Unknown_1Ch;  // 0x00000000

    void Resolve(memstream& file)
    {
        DataPointer.Resolve(file);

        NextPointer.Resolve(file);
    }
};

struct ClipDictionary : pgBase
{
    uint32_t Unknown_10h;  // 0x00000000
    uint32_t Unknown_14h;  // 0x00000000
    pgPtr<AnimationMap> AnimationsPointer;
    uint32_t Unknown_20h;
    uint32_t Unknown_24h;  // 0x00000000
    pgObjectArray<ClipMapEntry> ClipsPointer;
    uint32_t Unknown_34h;
    uint32_t Unknown_38h;  // 0x00000000
    uint32_t Unknown_3Ch;  // 0x00000000

    void Resolve(memstream& file)
    {
        AnimationsPointer.Resolve(file);
        AnimationsPointer->Resolve(file);

        ClipsPointer.Resolve(file);
    }
};

class YcdLoader : public FileType
{
public:
    YcdLoader(memstream& file);
    ~YcdLoader();
};
