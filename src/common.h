#pragma once
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
    size_t BufferSize;
    const uint8_t* data;

    memstream(const uint8_t* buffer, size_t sizeData)
        : data(buffer)
        , offset(0)
        , BufferSize(sizeData)
    {
    }

    const uint8_t* read(uint64_t Count)
    {
        uint64_t origOffset = offset;

        offset += Count;

        return &data[origOffset];
    }

    void read(void* dst, uint64_t size)
    {
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
    uint32_t Hash;
    int32_t SystemSize;
    FileType* file;
    uint8_t* Buffer;
    uint64_t BufferSize;

    Resource(Type t, uint32_t hash, FileType* _file)
        : type(t)
        , Hash(hash)
        , file(_file)
        , BufferSize(0)
    {
    }

    ~Resource()
    {
    }

private:
};
