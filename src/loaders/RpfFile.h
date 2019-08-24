#pragma once
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "RpfEntry.h"

class RpfFile
{
public:
    std::ifstream& rpf;
    bool isAESEncrypted = false;
    std::string path;
    std::vector<RpfResourceFileEntry> resourceEntries;
    std::vector<RpfBinaryFileEntry> binaryEntries;

    RpfFile(std::ifstream* rpfFile, std::string& fullPath, std::string& fileName, uint32_t fileSize, uint64_t fileOffset);

    ~RpfFile();
};
