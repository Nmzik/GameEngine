#include "YmfLoader.h"
#include <algorithm>

uint8_t YmfLoader::DetectType(memstream & stream)
{
	stream.seekg(0);

	uint32_t signature = *(uint32_t*)stream.read(sizeof(uint32_t));
	endSwap(&signature);

	if (signature == 0x30464252) {
		printf("RBF\n");
		return 0;
	}
	else if (signature == 1347635534) {
		//printf("PSO\n");
		return 1;
	}
	return 2;
}

YmfLoader::YmfLoader(memstream& stream)
{
	uint8_t type = DetectType(stream);

	if (type != 1) {
		//printf("ERROR YMF\n");
		//CAUSES ERROR IN ZLIB ALSO
		return;
	}

	stream.seekg(0);

	while (stream.tellg() < stream.BufferSize)
	{
		uint32_t section = *(uint32_t*)stream.read(sizeof(uint32_t));
		endSwap(&section);

		uint32_t length = *(uint32_t*)stream.read(sizeof(uint32_t));
		endSwap(&length);

		stream.seekCur(-8);

		switch (section)
		{
		case PSIN: {
			//printf("PSIN\n");
			_PsoDataSection.Read(stream);
			break;
		}
		case PMAP:
			//printf("PMAP\n");
			_PsoDataMapSection.Read(stream);
			break;
		case PSCH:
			//printf("PSCH\n");
			stream.seekCur(length);
			break;
		case PSIG:
			//printf("PSIG\n");
			stream.seekCur(length);
			break;
		case STRF:
			//printf("STRF\n");
			stream.seekCur(length);
			break;
		case STRS:
			//printf("STRS\n");
			stream.seekCur(length);
			break;
		case STRE:
			//printf("STRE\n");
			stream.seekCur(length);
			break;
		case CHKS:
			//printf("CHKS\n");
			stream.seekCur(length);
			break;
		default:
			//printf("SECTION NOT FOUND\n");
			break;
		}
	}

	for (int i = 0; i < _PsoDataMapSection.EntriesCount; i++)
	{
		if (_PsoDataMapSection.Entries[i].NameHash == 0x59869c63) { //SD => HD TEXTURES

			stream.seekg(_PsoDataMapSection.Entries[i].Offset);

			for (int j = 0; j < _PsoDataMapSection.Entries[i].Length / sizeof(CHDTxdAssetBinding); j++)
			{
				CHDTxdAssetBinding* texture = (CHDTxdAssetBinding*)stream.read(sizeof(CHDTxdAssetBinding));
				HDtextures.push_back(texture);
			}

		}
	}

}


YmfLoader::~YmfLoader()
{
}
