#include "RpfFile.h"
#include "GTAEncryption.h"

RpfFile::RpfFile(std::ifstream* rpfFile, std::string& FullPath, std::string& FileName, uint32_t FileSize, uint64_t FileOffset)
    : rpf(*rpfFile)
    , path(FullPath)
{
    rpf.seekg(FileOffset);

    uint64_t startPos = rpf.tellg();

    uint32_t Version;
    uint32_t EntryCount;
    uint32_t NamesLength;
    uint32_t Encryption;

    rpf.read((char*)&Version, sizeof(uint32_t));
    rpf.read((char*)&EntryCount, sizeof(uint32_t));
    rpf.read((char*)&NamesLength, sizeof(uint32_t));
    rpf.read((char*)&Encryption, sizeof(uint32_t));

    if (Version != 0x52504637)
    {
        printf("SOMETHING WRONG");
        return;
    }

    std::unique_ptr<uint8_t[]> entriesData = std::make_unique<uint8_t[]>(EntryCount * 16);
    std::unique_ptr<uint8_t[]> namesData = std::make_unique<uint8_t[]>(NamesLength);

    rpf.read((char*)&entriesData[0], EntryCount * 16);
    rpf.read((char*)&namesData[0], NamesLength);

    switch (Encryption)
    {
        case 0x0FFFFFF9:
            //	printf("AES\n");
            GTAEncryption::getInstance().decryptAES(entriesData.get(), EntryCount * 16);
            GTAEncryption::getInstance().decryptAES(namesData.get(), NamesLength);
            isAESEncrypted = true;
            break;
        case 0x0FEFFFFF:
            //	printf("NG\n");
            GTAEncryption::getInstance().decryptNG(entriesData.get(), EntryCount * 16, FileName, FileSize);
            GTAEncryption::getInstance().decryptNG(namesData.get(), NamesLength, FileName, FileSize);
            break;
        default:
            printf("OPENIV FORMAR?\n");
            break;
    }

    memstream EntriesStream(entriesData.get(), EntryCount * 16);
    memstream NamesStream(namesData.get(), NamesLength);

    for (uint32_t i = 0; i < EntryCount; i++)
    {
        uint32_t x;
        uint32_t y;
        EntriesStream.read((char*)&y, sizeof(uint32_t));
        EntriesStream.read((char*)&x, sizeof(uint32_t));
        EntriesStream.seekCur(-8);

        if (x == 0x7fffff00)
        {
            //	printf("DIRECTORY\n");
            RpfDirectoryEntry entry(EntriesStream, NamesStream);
            //	printf("%s\n", entry.Name.c_str());
        }
        else if ((x & 0x80000000) == 0)
        {
            //	printf("BINARY\n");
            RpfBinaryFileEntry entry(EntriesStream, startPos, NamesStream);
            entry.File = this;
            //	printf("%s\n", entry.Name.c_str());
            binaryEntries.push_back(entry);
        }
        else
        {
            //	printf("RESOURCE\n");
            RpfResourceFileEntry entry(EntriesStream, rpf, startPos, NamesStream);
            entry.File = this;
            entry.IsEncrypted = ((entry.FileName.substr(entry.FileName.length() - 4)) == ".ysc") ? true : false;  //HACK
            //	printf("%s\n", entry.Name.c_str());
            resourceEntries.push_back(entry);
        }
    }
}

RpfFile::~RpfFile()
{
}
