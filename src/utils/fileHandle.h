#pragma once
#include <cstdio>

class FileHandle {
    FILE* file;
public:
    FileHandle(const char* filePath)
    {
        file = fopen(filePath, "rb");
    }
    
    ~FileHandle()
    {
        if (file)
            fclose(file);
    }
    
    bool isOpen()
    {
        return file != nullptr;
    }
    
    inline void read(void* destination, size_t size)
    {
        fread(destination, size, 1, file);
    }
    
    inline void seek(size_t offset)
    {
        fseek(file, offset, SEEK_SET);
    }
    
    size_t getCurPos()
    {
        return ftell(file);
    }
    
    size_t getFileSize()
    {
        fseek(file, 0, SEEK_END);
        size_t FileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        return FileSize;
    }
    
    FileHandle(FileHandle& handle) = delete;
    FileHandle* operator=(FileHandle& handle) = delete;
    
    FileHandle(FileHandle&& handle)
    {
        file = handle.file;
        handle.file = 0;
    }
};

