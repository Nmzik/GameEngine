#pragma once
#include <cstdint>
#include <vector>

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
    ycd
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
