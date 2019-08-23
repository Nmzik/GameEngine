#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include "common.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#define SYSTEM_BASE 0x50000000
#define GRAPHICS_BASE 0x60000000

inline void TranslatePTR(uint64_t& pointer)
{
    if ((pointer & SYSTEM_BASE) == SYSTEM_BASE)
    {
        pointer = pointer & ~0x50000000;
    }
    if ((pointer & GRAPHICS_BASE) == GRAPHICS_BASE)
    {
        pointer = pointer & ~0x60000000;
    }
}

#define SYSTEM_BASE_PTR(NAME) NAME = NAME & ~0x50000000;
#define GRAPHICS_BASE_PTR(NAME) NAME = NAME & ~0x60000000;

struct Array_Structure  // 16 bytes - pointer for a structure array
{
    uint32_t Pointer;
    uint32_t Unk0;
    uint16_t Count1;
    uint16_t Count2;
    uint32_t Unk1;
};
struct Array_uint  // 16 bytes - pointer for a uint array
{
    uint32_t Pointer;
    uint32_t Unk0;
    uint16_t Count1;
    uint16_t Count2;
    uint32_t Unk1;
};
struct Array_float  // 16 bytes - pointer for a float array
{
    uint32_t Pointer;
    uint32_t Unk0;
    uint16_t Count1;
    uint16_t Count2;
    uint32_t Unk1;
};
struct Array_byte  // 16 bytes - pointer for a byte array
{
    uint32_t Pointer;
    uint32_t Unk0;
    uint16_t Count1;
    uint16_t Count2;
    uint32_t Unk1;
};
struct CharPointer  // 16 bytes - pointer for a char array
{
    uint32_t Pointer;
    uint32_t Unk0;
    uint16_t Count1;
    uint16_t Count2;
    uint32_t Unk1;
};

struct Array_StructurePointer  // 16 bytes - pointer for a structure pointer array
{
    uint32_t Pointer;
    uint32_t Unk0;
    uint16_t Count1;
    uint16_t Count2;
    uint32_t Unk1;
};

struct ResourceSimpleList64Ptr
{
    // structure data
    uint64_t EntriesPointer;
    uint16_t EntriesCount;
    uint16_t EntriesCapacity;
    uint32_t Unused1;
};

struct ResourcePointerList64
{
    uint64_t EntriesPointer;
    uint16_t EntriesCount;
    uint16_t EntriesCapacity;
};

struct ResourceFileBase
{
    uint32_t FileVFT;
    uint32_t FileUnknown;
    uint64_t FilePagesInfoPointer;
};

enum Unk_1264241711  // entity lodLevel
    : int            // Key:1856311430
{
    LODTYPES_DEPTH_HD = 0,
    LODTYPES_DEPTH_LOD = 1,
    LODTYPES_DEPTH_SLOD1 = 2,     // RARELY USED
    LODTYPES_DEPTH_SLOD2 = 3,     // RARELY USED
    LODTYPES_DEPTH_SLOD3 = 4,     // thanks Tadden :D //REAL SLOD
    LODTYPES_DEPTH_ORPHANHD = 5,  //	NOT USED? || ACTUALLY USED FOR INTERIORS
    LODTYPES_DEPTH_SLOD4 = 6,     // USED ONLY FOR GRASS
};

enum Unk_648413703  // entity priorityLevel
    : int           // Key:2200357711
{
    PRI_REQUIRED = 0,         // 1943361227
    PRI_OPTIONAL_HIGH = 1,    // 3993616791
    PRI_OPTIONAL_MEDIUM = 2,  //	515598709
    PRI_OPTIONAL_LOW = 3,     // 329627604
};

struct fwEntityDef  // 128 bytes, Key:1825799514
{
    uint32_t Unused0;                    //	0
    uint32_t Unused1;                    //	4
    uint32_t archetypeName;              //	8   8: Hash: 0: archetypeName
    uint32_t flags;                      //	12   12: UnsignedInt: 0: flags
    uint32_t guid;                       //	16   16: UnsignedInt: 0: guid
    uint32_t Unused2;                    //	20
    uint32_t Unused3;                    //	24
    uint32_t Unused4;                    //	28
    glm::vec3 position;                  //	32   32: Float_XYZ: 0: position
    float Unused5;                       //	44
    glm::quat rotation;                  //	48   48: Float_XYZW: 0: rotation
    float scaleXY;                       //	64   64: Float: 0: 2627937847
    float scaleZ;                        //	68   68: Float: 0: 284916802
    int32_t parentIndex;                 //	72   72: SignedInt: 0: parentIndex
    float lodDist;                       //	76   76: Float: 0: lodDist
    float childLodDist;                  //	80   80: Float: 0: childLodDist//3398912973
    Unk_1264241711 lodLevel;             //	84   84: IntEnum: 1264241711: lodLevel  //LODTYPES_DEPTH_
    uint32_t numChildren;                //	88   88: UnsignedInt: 0: numChildren//2793909385
    Unk_648413703 priorityLevel;         //	92   92: IntEnum: 648413703: priorityLevel//647098393
    Array_StructurePointer extensions;   //	96   96: Array: 0: extensions  {0: StructurePointer: 0: 256}
    int32_t ambientOcclusionMultiplier;  //	112   112: SignedInt: 0: ambientOcclusionMultiplier//415356295
    int32_t artificialAmbientOcclusion;  //	116   116: SignedInt: 0: artificialAmbientOcclusion//599844163
    uint32_t tintValue;                  //	120   120: UnsignedInt: 0: tintValue//1015358759
    uint32_t Unused6;                    //	124
};

class datBase
{
    uint32_t VFT;
    uint32_t Unknown_4h;  // 0x00000001
};

template <typename TValue>
class atArray
{
public:
    TValue* m_offset;
    uint16_t m_count;
    uint16_t m_size;

public:
    atArray()
    {
        m_offset = 0;
        m_count = 0;
        m_size = 0;
    }

    atArray(int capacity)
    {
        m_offset = new TValue[capacity];
        m_count = 0;
        m_size = capacity;
    }

    ~atArray()
    {
        delete[] m_offset;
    }

    void reserve(uint16_t size)
    {
        m_offset = new TValue[size];
        m_count = 0;
        m_size = size;
    }

    auto& operator[](uint16_t idx)
    {
        return m_offset[idx];
    }

    auto begin()
    {
        return &m_offset[m_size];
    }

    auto end()
    {
        return &m_offset[m_count];
    }
};

template <typename T>
class pgPtr
{
public:
    T* pointer;

    T* operator->() const
    {
        return (T*)pointer;
    }

    T* operator*() const
    {
        return (T*)pointer;
    }

    T operator[](int i) const
    {
        return *((T*)&pointer[i]);
    }

    pgPtr operator=(T* other)
    {
        pointer = other;
        return *this;
    }

    void Resolve(memstream& file)
    {
        pointer = (T*)&file.data[(uint64_t)pointer & ~0x50000000];
    }
};

template <typename TValue>
class pgObjectArray
{
private:
    pgPtr<pgPtr<TValue>> m_objects;
    uint16_t m_count;
    uint16_t m_size;

public:
    inline uint16_t size() const
    {
        return m_count;
    }

    TValue* operator[](uint16_t offset)
    {
        return *((*m_objects)[offset]);
    }

    TValue* Get(uint16_t offset)
    {
        return *((*m_objects)[offset]);
    }

    inline void Resolve(memstream& file)
    {
        m_objects.Resolve(file);

        for (int i = 0; i < m_count; i++)
        {
            (*m_objects)[i].Resolve(file);
            (*m_objects)[i]->Resolve(file);
        }
    }
};

template <typename TValue>
class pgArray
{
private:
    pgPtr<TValue> m_offset;
    uint16_t m_count;
    uint16_t m_size;
    uint32_t padding;

public:
    pgArray()
    {
        m_offset = (TValue*)0;
        m_count = 0;
        m_size = 0;
    }

    pgArray(int capacity)
    {
        m_offset = new TValue[capacity];
        m_count = 0;
        m_size = capacity;
    }

    TValue& Get(uint16_t offset)
    {
        return (*m_offset)[offset];
    }

    inline uint16_t GetSize() const
    {
        return m_size;
    }

    inline uint16_t GetCount() const
    {
        return m_count;
    }

    inline void Resolve(memstream& file)
    {
        m_offset.Resolve(file);
    }
};

/*template<typename TValue>
 class pgDictionary
 {
 private:
 pgPtr<pgBase> m_parent;
 uint32_t m_usageCount;
 pgArray<uint32_t> m_hashes;
 pgObjectArray<TValue> m_values;
 
 public:
 struct iterator : public std::iterator<std::forward_iterator_tag, std::pair<uint32_t, TValue*>>
 {
 private:
 pgDictionary* m_base;
 int m_index;
 
 std::pair<uint32_t, TValue*> m_value;
 
 private:
 inline std::pair<uint32_t, TValue*> GetValue()
 {
 return std::make_pair(m_base->m_hashes.Get(m_index), m_base->m_values.Get(m_index));
 }
 
 public:
 inline iterator(pgDictionary* base, int index)
 : m_base(base), m_index(index)
 {
 m_value = GetValue();
 }
 
 inline std::pair<uint32_t, TValue*> operator*() const
 {
 return m_value;
 }
 
 inline const std::pair<uint32_t, TValue*>* operator->() const
 {
 return &m_value;
 }
 
 inline const iterator& operator++()
 {
 m_index++;
 m_value = GetValue();
 
 return *this;
 }
 
 inline friend bool operator!=(const iterator& left, const iterator& right)
 {
 return (left.m_base != right.m_base || left.m_index != right.m_index);
 }
 
 inline friend bool operator==(const iterator& left, const iterator& right)
 {
 return !(left != right);
 }
 };
 
 public:
 pgDictionary()
 {
 m_usageCount = 1;
 }
 
 inline iterator begin()
 {
 return iterator(this, 0);
 }
 
 inline iterator end()
 {
 return iterator(this, m_hashes.GetCount());
 }
 
 inline void Add(uint32_t keyHash, TValue* value)
 {
 m_hashes.Set(m_hashes.GetCount(), keyHash);
 m_values.Set(m_values.GetCount(), value);
 }
 
 inline void Add(const char* key, TValue* value)
 {
 Add(HashString(key), value);
 }
 
 inline TValue* Get(uint32_t keyHash)
 {
 for (int i = 0; i < m_hashes.GetCount(); i++)
 {
 if (m_hashes.Get(i) == keyHash)
 {
 return m_values.Get(i);
 }
 }
 
 return nullptr;
 }
 
 inline TValue* Get(const char* key)
 {
 return Get(HashString(key));
 }
 
 inline TValue* GetAt(uint16_t index)
 {
 return m_values.Get(index);
 }
 
 inline uint16_t GetCount() const
 {
 return m_hashes.GetCount();
 }
 
 inline void Resolve(memstream& file)
 {
 m_parent.Resolve(file);
 m_hashes.Resolve(file);
 m_values.Resolve(file);
 }
 };
 */

#include "GameRenderer.h"

class FileType
{
public:
    uint32_t refCount = 0;
    uint64_t gpuMemory = 0;
    bool loaded = false;

    FileType()
    {
    }
    virtual ~FileType()
    {
    }

    virtual void init(memstream& file)
    {
    }
    virtual void finalize(GameRenderer* renderer, memstream& file)
    {
    }
};
