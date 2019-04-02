#pragma once
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "RpfEntry.h"
#include "membuf.h"

class RpfFile
{
public:
    std::ifstream& rpf;
    bool IsAESEncrypted = false;

    std::vector<RpfResourceFileEntry> ResourceEntries;
    std::vector<RpfBinaryFileEntry> BinaryEntries;

    RpfFile(std::ifstream* rpfFile, std::string& FullPath, std::string& FileName, uint32_t FileSize, uint64_t FileOffset);

    ~RpfFile();
};
