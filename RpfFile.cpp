#include "RpfFile.h"

RpfFile::RpfFile(std::istream& rpf, std::string& FileName_)
{
	FileName = FileName_;
	FullPath = FileName_;
	rpf.seekg(0, std::ios::end);
	FileSize = rpf.tellg();
	rpf.seekg(0, std::ios::beg);

	LoadRpf(rpf);
}
RpfFile::RpfFile(std::istream& rpf, std::string& FullPath_, std::string& FileName_, uint32_t FileSize_, uint64_t FileOffset)
{
	FileName = FileName_;
	FullPath = FullPath_;
	FileSize = FileSize_;

	rpf.seekg(FileOffset);

	LoadRpf(rpf);
}
void RpfFile::LoadRpf(std::istream& rpf)
{
	startPos = rpf.tellg();

	rpf.read((char*)&Version, sizeof(uint32_t));
	rpf.read((char*)&EntryCount, sizeof(uint32_t));
	rpf.read((char*)&NamesLength, sizeof(uint32_t));
	rpf.read((char*)&Encryption, sizeof(uint32_t));

	if (Version != 0x52504637) {
		printf("SOMETHING WRONG");
		return;
	}

	uint8_t* entriesData = new uint8_t[EntryCount * 16];
	uint8_t* namesData = new uint8_t[NamesLength];

	rpf.read((char*)&entriesData[0], EntryCount * 16);
	rpf.read((char*)&namesData[0], NamesLength);

	switch (Encryption)
	{
	case 0x0FFFFFF9:
		//printf("AES\n");
		entriesData = GTAEncryption::DecryptAES(entriesData, EntryCount * 16);
		namesData = GTAEncryption::DecryptAES(namesData, NamesLength);
		//printf("FILE===========================%s\n", FileName.c_str());
		return;
		break;
	case 0x0FEFFFFF:
		//printf("NG\n");
		entriesData = GTAEncryption::DecryptNG(entriesData, EntryCount * 16, FileName, (uint32_t)FileSize);
		namesData = GTAEncryption::DecryptNG(namesData, NamesLength, FileName, (uint32_t)FileSize);
		break;
	default:
		printf("ERROR");
		break;
	}

	memstream EntriesStream(entriesData, EntryCount * 16);
	memstream NamesStream(namesData, NamesLength);

	for (int i = 0; i < EntryCount; i++)
	{
		uint32_t x;
		uint32_t y;
		EntriesStream.read((char*)&y, sizeof(uint32_t));
		EntriesStream.read((char*)&x, sizeof(uint32_t));
		EntriesStream.seekg(-8, std::ios::cur);

		if (x == 0x7fffff00) {
			//printf("DIRECTORY\n");
			RpfDirectoryEntry entry;
			entry.Read(EntriesStream);
			NamesStream.seekg(entry.NameOffset);
			std::getline(NamesStream, entry.Name, '\0');
			//printf("%s\n", entry.Name.c_str());
		}
		else if ((x & 0x80000000) == 0)
		{
			//printf("BINARY\n");
			RpfBinaryFileEntry entry;
			entry.Read(EntriesStream);
			NamesStream.seekg(entry.NameOffset);
			std::getline(NamesStream, entry.Name, '\0');
			//printf("%s\n", entry.Name.c_str());
			BinaryEntries.push_back(entry);
		}
		else {
			//printf("RESOURCE\n");
			RpfResourceFileEntry entry;
			entry.Read(EntriesStream, rpf, startPos);
			NamesStream.seekg(entry.NameOffset);
			std::getline(NamesStream, entry.Name, '\0');
			entry.Path = FullPath;
			entry.File = this;
			//printf("%s\n", entry.Name.c_str());
			ResourceEntries.push_back(entry);
		}
	}

	//printf("=========================================\n");
	//printf("FILE===========================%s\n", FileName.c_str());
}

RpfFile::~RpfFile()
{

}
