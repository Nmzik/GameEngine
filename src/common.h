#pragma once
#include <cassert>
#include <cstdint>
#include <cstring>
#include "GlobalPool.h"

//BIG ENDIAN
template <class T>
void endSwap(T* objp)
{
    unsigned char* memp = reinterpret_cast<unsigned char*>(objp);
    std::reverse(memp, memp + sizeof(T));
}

class memstream
{
    uint64_t offset;

public:
    int32_t systemSize;
    size_t bufferSize;
    const uint8_t* data;

    memstream(const uint8_t* buffer, size_t sizeData)
        : offset(0)
        , bufferSize(sizeData)
        , data(buffer)
    {
    }

    const uint8_t* read(uint64_t Count)
    {
        assert(offset + Count <= bufferSize);
        uint64_t origOffset = offset;

        offset += Count;

        return &data[origOffset];
    }

    void read(void* dst, uint64_t size)
    {
        assert(offset + size <= bufferSize);
        memcpy(dst, &data[offset], size);
        offset += size;
    }

    char* getString()
    {
        return (char*)&data[offset];
    }

    void seekg(uint64_t OffsetFile)
    {
        offset = OffsetFile;
    }

    uint64_t tellg()
    {
        return offset;
    }

    void seekCur(int64_t OffsetFile)
    {
        offset += OffsetFile;
    }
};

enum Type
{
    ydr,
    ydd,
    yft,
    ytd,
    ybn,
    ymap,
    ynd,
    ynv,
    ycd,
    ysc,
    awc,
    null
};

class FileType;

class Resource
{
public:
    Type type;
    uint32_t hash;
    int32_t systemSize;
    FileType* file;
    uint8_t* buffer;
    uint32_t bufferSize;

    Resource(Type t, uint32_t resHash, FileType* _file)
        : type(t)
        , hash(resHash)
        , file(_file)
        , bufferSize(0)
    {
    }

    ~Resource()
    {
    }

private:
};
