#include "RpfFile.h"

RpfFile::RpfFile(std::ifstream& rpf, std::string& FullPath_, std::string& FileName_, uint32_t FileSize_, uint64_t FileOffset)
{
	this->rpf = &rpf;

	rpf.seekg(FileOffset);

	LoadRpf(rpf, FileName_, FileSize_, FullPath_);
}

void RpfFile::LoadRpf(std::ifstream& rpf, std::string& FileName, uint32_t FileSize, std::string& FullPath)
{
	uint64_t startPos = rpf.tellg();

	uint32_t Version;
	uint32_t EntryCount;
	uint32_t NamesLength;
	uint32_t Encryption;

	rpf.read((char*)&Version, sizeof(uint32_t));
	rpf.read((char*)&EntryCount, sizeof(uint32_t));
	rpf.read((char*)&NamesLength, sizeof(uint32_t));
	rpf.read((char*)&Encryption, sizeof(uint32_t));

	if (Version != 0x52504637) {
		printf("SOMETHING WRONG");
		return;
	}

	uint8_t* entriesData = myNew uint8_t[EntryCount * 16];
	uint8_t* namesData = myNew uint8_t[NamesLength];

	rpf.read((char*)&entriesData[0], EntryCount * 16);
	rpf.read((char*)&namesData[0], NamesLength);

	switch (Encryption)
	{
	case 0x0FFFFFF9:
		//printf("AES\n");
		entriesData = GTAEncryption::DecryptAES(entriesData, EntryCount * 16);
		namesData = GTAEncryption::DecryptAES(namesData, NamesLength);
		//IsAESEncrypted = true;
		break;
	case 0x0FEFFFFF:
		//printf("NG\n");
		entriesData = GTAEncryption::DecryptNG(entriesData, EntryCount * 16, FileName, FileSize);
		namesData = GTAEncryption::DecryptNG(namesData, NamesLength, FileName, FileSize);
		break;
	default:
		printf("ERROR");
		break;
	}

	memstream EntriesStream(entriesData, EntryCount * 16);
	memstream NamesStream(namesData, NamesLength);

	for (uint32_t i = 0; i < EntryCount; i++)
	{
		uint32_t x;
		uint32_t y;
		EntriesStream.read((char*)&y, sizeof(uint32_t));
		EntriesStream.read((char*)&x, sizeof(uint32_t));
		EntriesStream.seekg(-8, std::ios::cur);

		if (x == 0x7fffff00) {
			//printf("DIRECTORY\n");
			RpfDirectoryEntry entry(EntriesStream);
			NamesStream.seekg(entry.NameOffset);
			std::getline(NamesStream, entry.Name, '\0');
			//printf("%s\n", entry.Name.c_str());
		}
		else if ((x & 0x80000000) == 0)
		{
			//printf("BINARY\n");
			RpfBinaryFileEntry entry(EntriesStream, startPos);
			NamesStream.seekg(entry.NameOffset);
			std::getline(NamesStream, entry.Name, '\0');
			entry.File = this;
			//printf("%s\n", entry.Name.c_str());
			BinaryEntries.push_back(entry);
		}
		else {
			//printf("RESOURCE\n");
			RpfResourceFileEntry entry(EntriesStream, rpf, startPos);
			NamesStream.seekg(entry.NameOffset);
			std::getline(NamesStream, entry.Name, '\0');
			entry.Path = FullPath;
			entry.File = this;
			//printf("%s\n", entry.Name.c_str());
			ResourceEntries.push_back(entry);
		}
	}

	delete[] entriesData;
	delete[] namesData;
}

RpfFile::~RpfFile()
{

}
