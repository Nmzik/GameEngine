#pragma once
#include "membuf.h"

inline int GetSizeFromFlags(uint32_t flags)
{
	//	dexfx simplified version
	uint32_t s0       = ((flags >> 27) & 0x1) << 0;  // 1 bit  - 27        (*1)
	uint32_t s1       = ((flags >> 26) & 0x1) << 1;  // 1 bit  - 26        (*2)
	uint32_t s2       = ((flags >> 25) & 0x1) << 2;  // 1 bit  - 25        (*4)
	uint32_t s3       = ((flags >> 24) & 0x1) << 3;  // 1 bit  - 24        (*8)
	uint32_t s4       = ((flags >> 17) & 0x7F) << 4; // 7 bits - 17 - 23   (*16)   (max 127 * 16)
	uint32_t s5       = ((flags >> 11) & 0x3F) << 5; // 6 bits - 11 - 16   (*32)   (max 63  * 32)
	uint32_t s6       = ((flags >> 7) & 0xF) << 6;   // 4 bits - 7  - 10   (*64)   (max 15  * 64)
	uint32_t s7       = ((flags >> 5) & 0x3) << 7;   // 2 bits - 5  - 6    (*128)  (max 3   * 128)
	uint32_t s8       = ((flags >> 4) & 0x1) << 8;   // 1 bit  - 4         (*256)
	uint32_t ss       = ((flags >> 0) & 0xF);        // 4 bits - 0  - 3
	uint32_t baseSize = 0x200 << (int)ss;
	uint32_t size     = baseSize * (s0 + s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8);
	return (int)size;
}

class RpfEntry
{
	public:
	uint32_t NameHash;
	uint32_t ShortNameHash;

	std::string FileName;
	std::string Path;
};

class RpfDirectoryEntry : public RpfEntry
{
	public:
	uint32_t EntriesIndex;
	uint32_t EntriesCount;

	RpfDirectoryEntry(memstream& stream, memstream& NamesStream);
};

class RpfFile;

class RpfFileEntry : public RpfEntry
{
	public:
	RpfFile* File;
	uint64_t FileOffset;
	uint32_t FileSize;

	bool IsEncrypted;
};

class RpfBinaryFileEntry : public RpfFileEntry
{
	public:
	uint32_t FileUncompressedSize;
	uint32_t EncryptionType;

	RpfBinaryFileEntry(memstream& stream, uint64_t StartPos, memstream& NamesStream);
};

class RpfResourceFileEntry : public RpfFileEntry
{
	public:
	uint32_t SystemFlags;
	uint32_t GraphicsFlags;

	int32_t SystemSize;
	uint64_t UncompressedFileSize;

	RpfResourceFileEntry(memstream& stream, std::istream& originalFile, uint64_t StartPos, memstream& NamesStream);
};
