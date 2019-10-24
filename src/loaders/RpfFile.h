#pragma once
#include <memory>
#include <string>
#include <vector>
#include "RpfEntry.h"
#include "../utils/fileHandle.h"

class RpfFile
{
public:
    FileHandle& rpf;
    bool isAESEncrypted = false;
    std::string path;
    std::vector<RpfResourceFileEntry> resourceEntries;
    std::vector<RpfBinaryFileEntry> binaryEntries;

    RpfFile(FileHandle& rpfFile, std::string& fullPath, std::string& fileName, uint32_t fileSize, uint64_t fileOffset);

    ~RpfFile();
};
