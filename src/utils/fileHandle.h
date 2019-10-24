#pragma once
#if WIN32
#include <fstream>
//#include <windows.h>
#else
#include <cstdio>
#endif

class FileHandle
{
#if WIN32
    //HANDLE hFile;
    std::ifstream file;
#else
    FILE* file;
#endif
public:
    FileHandle(const char* filePath)
    {
#if WIN32
        file = std::ifstream(filePath, std::ios::binary);
        //hFile = CreateFileA(filePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
#else
        file = fopen(filePath, "rb");
#endif
    }

    ~FileHandle()
    {
#if WIN32
        //if (hFile != INVALID_HANDLE_VALUE)
        //	CloseHandle(hFile);

#else
        if (file)
            fclose(file);
#endif
    }

    bool isOpen()
    {
#if WIN32
        //return hFile != INVALID_HANDLE_VALUE;
        return file.is_open();
#else
        return file != nullptr;
#endif
    }

    inline void read(void* destination, uint64_t size)
    {
#if WIN32
        //DWORD read = 0;
        //ReadFile(hFile, destination, size, &read, NULL);
        file.read((char*)destination, size);
#else
        fread(destination, size, 1, file);
#endif
    }

    inline void seek(uint64_t offset)
    {
#if WIN32
        file.seekg(offset);
        //LARGE_INTEGER offsetToSeek = {0};
        //offsetToSeek.QuadPart = offset;
        //SetFilePointerEx(hFile, offsetToSeek, NULL, FILE_BEGIN);
        //LONG _high = (LONG)(offset >> 32);
        //SetFilePointer(hFile, (LONG)(offset & 0xFFFFFFFF), (_high > 0 ? &_high : NULL), FILE_BEGIN);

        //SetFilePointer(hFile, offset, NULL, FILE_BEGIN);
#else
        fseek(file, offset, SEEK_SET);
#endif
    }

    uint64_t getCurPos()
    {
#if WIN32
        return file.tellg();
        //LARGE_INTEGER offset = {0};
        //LARGE_INTEGER offsetToSeek = {0};
        //SetFilePointerEx(hFile, offset, &offsetToSeek, FILE_BEGIN);
        //DWORD _low;
        //LONG _high = 0;
        //_low = SetFilePointer(hFile, 0, &_high, FILE_CURRENT);
        //return (uint64_t)_low + (((uint64_t)_high) << 32);
#else
        return ftell(file);
#endif
    }

    uint64_t getFileSize()
    {
#if WIN32
        //DWORD fileSize = GetFileSize(hFile, NULL);
        ///return fileSize;
        file.seekg(0, std::ios::end);
        uint32_t FileSize = (uint32_t)file.tellg();
        file.seekg(0, std::ios::beg);
        return FileSize;
#else
        fseek(file, 0, SEEK_END);
        uint64_t FileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        return FileSize;
#endif
    }

    FileHandle(FileHandle& handle) = delete;
    FileHandle* operator=(FileHandle& handle) = delete;

    FileHandle(FileHandle&& handle) noexcept
    {
#if WIN32
        //hFile = handle.hFile;
        //handle.hFile = INVALID_HANDLE_VALUE;
#else
        file = handle.file;
        handle.file = 0;
#endif
    }
};
