#pragma once
#include "membuf.h"
#include "GameWorld.h"

class RpfEntry {
public:
	uint32_t NameHash;
	uint32_t ShortNameHash;

	uint32_t NameOffset = 0;
	std::string Name;
	std::string NameLower;
	std::string Path;
};

class RpfDirectoryEntry : public RpfEntry {
public:
	uint32_t EntriesIndex;
	uint32_t EntriesCount;

	void Read(memstream& stream) {
		stream.read((char*)&NameOffset, sizeof(uint32_t));
		uint32_t ident;
		stream.read((char*)&ident, sizeof(uint32_t));
		if (ident != 0x7FFFFF00u)
		{
			printf("ERROR DIRECTORY");
		}
		stream.read((char*)&EntriesIndex, sizeof(uint32_t));
		stream.read((char*)&EntriesCount, sizeof(uint32_t));
	}
};
class RpfFileEntry : public RpfEntry {
public:
	uint32_t FileOffset;
	uint32_t FileSize;
	bool IsEncrypted;
};

class RpfBinaryFileEntry : public RpfFileEntry {
public:
	uint32_t FileUncompressedSize;
	uint32_t EncryptionType;

	void Read(memstream& stream) {
		uint64_t buf;
		stream.read((char*)&buf, sizeof(uint64_t));
		NameOffset = (uint32_t)buf & 0xFFFF;
		FileSize = (uint32_t)(buf >> 16) & 0xFFFFFF;
		FileOffset = (uint32_t)(buf >> 40) & 0xFFFFFF;
		stream.read((char*)&FileUncompressedSize, sizeof(uint32_t));

		stream.read((char*)&EncryptionType, sizeof(uint32_t));
	}
};

class RpfResourceFileEntry : public RpfFileEntry {
public:
	uint32_t SystemFlags;
	uint32_t GraphicsFlags;

	void Read(memstream& stream) {
		stream.read((char*)&NameOffset, sizeof(uint16_t)); //READING 16 bit to 32bit vatiable NEED TO CLEAR IT!

		uint8_t buf1[3];
		stream.read((char*)&buf1, 3);
		FileSize = (uint32_t)buf1[0] + (uint32_t)(buf1[1] << 8) + (uint32_t)(buf1[2] << 16);

		uint8_t buf2[3];
		stream.read((char*)&buf2, 3);
		FileOffset = ((uint32_t)buf2[0] + (uint32_t)(buf2[1] << 8) + (uint32_t)(buf2[2] << 16)) & 0x7FFFFF;

		stream.read((char*)&SystemFlags, sizeof(uint32_t));
		stream.read((char*)&GraphicsFlags, sizeof(uint32_t));

		// there are sometimes resources with length=0xffffff which actually
		// means length>=0xffffff
		if (FileSize == 0xFFFFFF)
		{
			printf("TODO");
			/*BinaryReader cfr = File.CurrentFileReader;
			long opos = cfr.BaseStream.Position;
			cfr.BaseStream.Position = File.StartPos + ((long)FileOffset * 512); //need to use the base offset!!
			var buf = cfr.ReadBytes(16);
			FileSize = ((uint)buf[7] << 0) | ((uint)buf[14] << 8) | ((uint)buf[5] << 16) | ((uint)buf[2] << 24);
			cfr.BaseStream.Position = opos;*/
		}
	}
};