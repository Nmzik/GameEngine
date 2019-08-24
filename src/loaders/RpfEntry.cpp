#include "RpfEntry.h"
#include <istream>

RpfDirectoryEntry::RpfDirectoryEntry(memstream& stream, memstream& NamesStream)
{
    uint32_t NameOffset;
    stream.read((char*)&NameOffset, sizeof(uint32_t));
    NamesStream.seekg(NameOffset);
    FileName = NamesStream.getString();

    uint32_t ident;
    stream.read((char*)&ident, sizeof(uint32_t));
    if (ident != 0x7FFFFF00u)
    {
        printf("ERROR DIRECTORY");
    }
    stream.read((char*)&EntriesIndex, sizeof(uint32_t));
    stream.read((char*)&EntriesCount, sizeof(uint32_t));
}

RpfBinaryFileEntry::RpfBinaryFileEntry(memstream& stream, uint64_t StartPos, memstream& NamesStream)
{
    uint64_t buf;
    stream.read((char*)&buf, sizeof(uint64_t));
    uint32_t NameOffset = (uint32_t)buf & 0xFFFF;
    NamesStream.seekg(NameOffset);
    FileName = NamesStream.getString();

    FileSize = (uint32_t)(buf >> 16) & 0xFFFFFF;
    FileOffset = (uint32_t)(buf >> 40) & 0xFFFFFF;
    stream.read((char*)&FileUncompressedSize, sizeof(uint32_t));

    stream.read((char*)&EncryptionType, sizeof(uint32_t));

    //	if (EncryptionType == 1) IsEncrypted = true;

    FileOffset = StartPos + ((uint64_t)FileOffset * 512);
}

RpfResourceFileEntry::RpfResourceFileEntry(memstream& stream, std::istream& originalFile, uint64_t StartPos, memstream& NamesStream)
{
    uint16_t NameOffset;
    stream.read((char*)&NameOffset, sizeof(uint16_t));
    NamesStream.seekg(NameOffset);
    FileName = NamesStream.getString();

    uint8_t buf1[3];
    stream.read((char*)&buf1, 3);
    FileSize = (uint32_t)buf1[0] + (uint32_t)(buf1[1] << 8) + (uint32_t)(buf1[2] << 16);

    uint8_t buf2[3];
    stream.read((char*)&buf2, 3);
    FileOffset = ((uint32_t)buf2[0] + (uint32_t)(buf2[1] << 8) + (uint32_t)(buf2[2] << 16)) & 0x7FFFFF;

    uint32_t SystemFlags;
    uint32_t GraphicsFlags;
    stream.read((char*)&SystemFlags, sizeof(uint32_t));
    stream.read((char*)&GraphicsFlags, sizeof(uint32_t));

    SystemSize = GetSizeFromFlags(SystemFlags);
    GraphicsSize = GetSizeFromFlags(GraphicsFlags);

    UncompressedFileSize = SystemSize + GraphicsSize;
    //

    // there are sometimes resources with length=0xffffff which actually
    // means length>=0xffffff
    if (FileSize == 0xFFFFFF)
    {
        uint64_t pos = originalFile.tellg();
        originalFile.seekg(StartPos + ((long)FileOffset * 512));
        uint8_t buf[16];
        originalFile.read((char*)&buf[0], 16);
        FileSize = ((uint32_t)buf[7] << 0) | ((uint32_t)buf[14] << 8) | ((uint32_t)buf[5] << 16) | ((uint32_t)buf[2] << 24);
        originalFile.seekg(pos);
    }

    FileSize = FileSize - 0x10;
    FileOffset = StartPos + ((long)FileOffset * 512) + 0x10;
}
