#include "RpfFile.h"

RpfFile::RpfFile(std::string FileName_)
{
	std::string Path("C:\\Program Files\\Rockstar Games\\Grand Theft Auto V\\");
	std::ifstream rpf(Path + FileName_, std::ios::binary);
	if (!rpf.is_open()) {
		printf("NOT FOUND RPF!\n");
		return;
	}

	std::istream& fileStream(rpf);

	FileName = FileName_;
	rpf.seekg(0, std::ios::end);
	FileSize = rpf.tellg();
	rpf.seekg(0, std::ios::beg);

	LoadRpf(rpf);
}
RpfFile::RpfFile(std::istream& rpf, std::string FileName_, uint32_t FileSize_, uint64_t FileOffset)
{
	FileName = FileName_;
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
		printf("AES\n");
		return;
		break;
	case 0x0FEFFFFF:
		printf("NG\n");
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
			printf("DIRECTORY\n");
			RpfDirectoryEntry entry;
			entry.Read(EntriesStream);
			NamesStream.seekg(entry.NameOffset);
			std::getline(NamesStream, entry.Name, '\0');
			printf("%s\n", entry.Name.c_str());
		}
		else if ((x & 0x80000000) == 0)
		{
			printf("BINARY\n");
			RpfBinaryFileEntry entry;
			entry.Read(EntriesStream);
			NamesStream.seekg(entry.NameOffset);
			std::getline(NamesStream, entry.Name, '\0');
			printf("%s\n", entry.Name.c_str());
			BinaryEntries.push_back(entry);
		}
		else {
			printf("RESOURCE\n");
			RpfResourceFileEntry entry;
			entry.Read(EntriesStream);
			NamesStream.seekg(entry.NameOffset);
			std::getline(NamesStream, entry.Name, '\0');
			printf("%s\n", entry.Name.c_str());
			ResourceEntries.push_back(entry);
		}
	}

	printf("=========================================\n");
	printf("FILE===========================%s\n", FileName.c_str());
	for (auto& BinaryEntry : BinaryEntries)
	{
		if (BinaryEntry.Name.substr(BinaryEntry.Name.length() - 4) == ".rpf")
		{
			uint32_t RealFileSize = (BinaryEntry.FileSize == 0) ? BinaryEntry.FileUncompressedSize : BinaryEntry.FileSize;
			RpfFile Subfile(rpf, BinaryEntry.Name, RealFileSize, startPos + ((uint64_t)BinaryEntry.FileOffset * 512));
			SubFiles.push_back(Subfile);
		}
	}

}

RpfFile::~RpfFile()
{

}
