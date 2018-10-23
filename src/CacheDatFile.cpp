#include "CacheDatFile.h"

CacheDatFile::CacheDatFile(std::vector<uint8_t>& Data)
{
	memstream stream(Data.data(), Data.size());
	stream.seekg(100);
	
	uint32_t modlen;
	uint32_t structcount = 0;

	size_t SizeData = Data.size();

	std::string line;
	//skip version
	for (uint32_t i = 100; i < SizeData; i++)
	{
		uint8_t b;
		stream.read((char*)&b, sizeof(uint8_t));
		//if (b == 0) break;
		if (b == 0xA) {
			std::string LocalLine(line.c_str());

			if (line[0] == 0x66 && line[1] == 0x77) { //LocalLine == "fwMapDataStore"
				//stream.seekg(1);
				stream.read((char*)&modlen, sizeof(uint32_t));
				structcount = modlen / 64;

				AllMapNodes.reserve(structcount);

				for (uint32_t i = 0; i < structcount; i++)
				{
					MapDataStoreNode node;
					stream.read((char*)&node, sizeof(MapDataStoreNode));
					AllMapNodes.push_back(node);
				}

				i += (int)(modlen + 4);
			}
			else if (line[0] == 0x43 && line[1] == 0x49) { //LocalLine == "CInteriorProxy"
				stream.read((char*)&modlen, sizeof(uint32_t));
				structcount = modlen / 104;

				AllCInteriorProxies.reserve(structcount);

				for (uint32_t i = 0; i < structcount; i++)
				{
					CInteriorProxy proxy;
					stream.read((char*)&proxy, sizeof(CInteriorProxy));
					AllCInteriorProxies.push_back(proxy);
				}

				i += (int)(modlen + 4);
			}
			else if (line[0] == 0x42 && line[1] == 0x6f) { //LocalLine == "BoundsStore"
				stream.read((char*)&modlen, sizeof(uint32_t));
				structcount = modlen / 32;

				AllBoundsStoreItems.reserve(structcount);

				for (uint32_t i = 0; i < structcount; i++)
				{
					BoundsStoreItem item;
					stream.read((char*)&item, sizeof(BoundsStoreItem));
					AllBoundsStoreItems.push_back(item);
				}

				i += (int)(modlen + 4);
			}
			/*
			else if (LocalLine == "</fileDates>") {

			}
			else if (LocalLine == "<module>") {

			}
			else if (LocalLine == "</module>") {

			}
			else if (LocalLine == "fwMapDataStore") {

			}
			else if (LocalLine == "CInteriorProxy") {

			}
			else if (LocalLine == "BoundsStore") {

			}*/
			line.clear();
		}
		else {
			line.push_back(b);
		}
	}
}


CacheDatFile::~CacheDatFile()
{
}
