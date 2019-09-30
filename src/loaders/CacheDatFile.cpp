#include <string>
#include "CacheDatFile.h"
#include "../common.h"

CacheDatFile::CacheDatFile(std::vector<uint8_t>& data)
{
    memstream stream(data.data(), data.size());
    stream.seekg(100);

    uint32_t modlen;
    uint32_t structcount = 0;

    size_t SizeData = data.size();

    std::string line;
    //	skip version
    for (uint32_t i = 100; i < SizeData; i++)
    {
        uint8_t b;
        stream.read((char*)&b, sizeof(uint8_t));
        //	if (b == 0) break;
        if (b == 0xA)
        {
            if (line[0] == 0x66 && line[1] == 0x77)
            {  //	LocalLine == "fwMapDataStore"
                //	stream.seekg(1);
                stream.read((char*)&modlen, sizeof(uint32_t));
                structcount = modlen / sizeof(MapDataStoreNode);

                allMapNodes.reserve(structcount);

                for (uint32_t i = 0; i < structcount; i++)
                {
                    MapDataStoreNode node;
                    stream.read((char*)&node, sizeof(MapDataStoreNode));
                    allMapNodes.push_back(node);
                }

                i += (int)(modlen + 4);
            }
            else if (line[0] == 0x43 && line[1] == 0x49)
            {  //	LocalLine == "CInteriorProxy"
                stream.read((char*)&modlen, sizeof(uint32_t));
                structcount = modlen / sizeof(CInteriorProxy);

                allCInteriorProxies.reserve(structcount);

                for (uint32_t i = 0; i < structcount; i++)
                {
                    CInteriorProxy proxy;
                    stream.read((char*)&proxy, sizeof(CInteriorProxy));
                    allCInteriorProxies.push_back(proxy);
                }

                i += (int)(modlen + 4);
            }
            else if (line[0] == 0x42 && line[1] == 0x6f)
            {  //	LocalLine == "BoundsStore"
                stream.read((char*)&modlen, sizeof(uint32_t));
                structcount = modlen / sizeof(BoundsStoreItem);

                allBoundsStoreItems.reserve(structcount);

                for (uint32_t i = 0; i < structcount; i++)
                {
                    BoundsStoreItem item;
                    stream.read((char*)&item, sizeof(BoundsStoreItem));
                    allBoundsStoreItems.push_back(item);
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
        else
        {
            line.push_back(b);
        }
    }
}

CacheDatFile::~CacheDatFile()
{
}
