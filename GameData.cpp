#include "GameData.h"

GameData::GameData()
{
	GTAEncryption::LoadKeys();

	std::vector <std::string> RpfsFiles = {
		"x64a.rpf",
		"x64b.rpf",
		"x64c.rpf",
		"x64d.rpf",
		"x64e.rpf",
		"x64f.rpf",
		"x64g.rpf",
		"x64h.rpf",
		"x64i.rpf",
		"x64j.rpf",
		"x64k.rpf",
		"x64l.rpf",
		"x64m.rpf",
		"x64n.rpf",
		"x64o.rpf",
		"x64p.rpf",
		"x64q.rpf",
		"x64r.rpf",
		"x64s.rpf",
		"x64t.rpf",
		"x64u.rpf",
		"x64v.rpf",
		"x64w.rpf",
	};

	for (std::string& rpfFile : RpfsFiles)
	{
		LoadRpf(rpfFile);
	}
	LoadWaterQuads();
	printf("%d\n", sizeof(RpfFile));
	printf("%d\n", sizeof(std::vector<RpfFile>) * RpfFiles.size() * sizeof(RpfFile)/1024/1024);
	//shrink to fit
	for (auto& rpfFile : RpfFiles)
	{
		for (auto& entry : rpfFile->ResourceEntries) //////////////////////SOMETHING FISHY IS GOING ON HERE!!!!!!!!!!!
		{
			if (!entry.Name.empty()) {
				std::string extension = entry.Name.substr(entry.Name.length() - 4);
				std::transform(entry.Name.begin(), entry.Name.end(), entry.Name.begin(), tolower);
				entry.NameHash = GenHash(entry.Name);
				size_t index = entry.Name.find_last_of('.');
				entry.ShortNameHash = (index > 0) ? GenHash(entry.Name.substr(0, index)) : entry.NameHash;

				if (extension == ".ydr") {
					YdrEntries[GenHash(entry.Name.substr(0, entry.Name.length() - 4) + "_lod")] = &entry;
					YdrEntries[entry.NameHash] = &entry;
					YdrEntries[entry.ShortNameHash] = &entry;
				}
				if (extension == ".ydd") {
					if (entry.Name.length() > 13) { //SHOULD WORK
						if (entry.Name.substr(entry.Name.length() - 13) == "_children.ydd") {
							
							//YddEntries[GenHash(entry.Name.substr(0, entry.Name.length() - 13) + "_lod")] = entry;
							YddEntries[GenHash(entry.Name.substr(0, entry.Name.length() - 13))] = &entry;
							YddEntries[GenHash(entry.Name.substr(0, entry.Name.length() - 13))] = &entry;

							size_t index1 = entry.Name.find_last_of('_');
							if (index1 > 0)
							{
								std::string str1 = entry.Name.substr(0, index1);
								size_t index2 = str1.find_last_of('_');
								if (index2 > 0)
								{
									std::string str2 = str1.substr(0, index2);
									YddEntries[GenHash(str2 + "_lod")] = &entry;
									uint32_t maxi = 100;
									for (uint32_t i = 1; i <= maxi; i++)
									{
										std::string str3 = str2 + "_" + std::to_string(i);
										if (i < 10)
											str3.insert(str3.length() - 1, "0");
										//printf("%s\n", str3.c_str());
										//std::string str3 = str2 + "_" + i.ToString().PadLeft(2, '0');
										YddEntries[GenHash(str3 + "_lod")] = &entry;
									}
								}
							}

						}
					}

					YddEntries[entry.NameHash] = &entry;
					YddEntries[entry.ShortNameHash] = &entry;
				}
				if (extension == ".ytd") {
					YtdEntries[entry.NameHash] = &entry;
					YtdEntries[entry.ShortNameHash] = &entry;
				}
				if (extension == ".ybn") {
					YbnEntries[entry.NameHash] = &entry;
					YbnEntries[entry.ShortNameHash] = &entry;
				}
				if (entry.Name.substr(entry.Name.length() - 5) == ".ymap") {
					YmapEntries[entry.NameHash] = &entry;
					YmapEntries[entry.ShortNameHash] = &entry;
				}
			}
		}
	}
}


GameData::~GameData()
{
}

void GameData::LoadWaterQuads()
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult  = doc.LoadFile("C:\\Users\\nmzik\\Desktop\\water.xml");

	//tinyxml2::XMLNode* pRoot = doc.FirstChild();
	//if(pRoot == nullptr) printf("ERRORMAIN");
	tinyxml2::XMLElement * root = doc.FirstChildElement("WaterData");

	if (root == nullptr) printf("ERROR");

	tinyxml2::XMLElement* element = root->FirstChildElement("WaterQuads");
	if (element == nullptr) printf("ERROR ELEMENT");

	//tinyxml2::XMLElement* ItemElement = element->FirstChildElement("Item");
	//if (ItemElement == nullptr) printf("ERROR ELEMENT");

	int test = 0;
	for (tinyxml2::XMLElement* e = element->FirstChildElement("Item"); e != NULL; e = e->NextSiblingElement("Item"))
	{
		WaterQuad waterQuad;
		tinyxml2::XMLElement* element = e->FirstChildElement("minX");
		element->QueryFloatAttribute("value", &waterQuad.minX);
		///
		element = element->NextSiblingElement("maxX");
		element->QueryFloatAttribute("value", &waterQuad.maxX);
		///
		element = element->NextSiblingElement("minY");
		element->QueryFloatAttribute("value", &waterQuad.minY);
		///
		element = element->NextSiblingElement("maxY");
		element->QueryFloatAttribute("value", &waterQuad.maxY);
		///
		element = element->NextSiblingElement("Type");
		element->QueryIntAttribute("value", &waterQuad.Type);
		///
		element = element->NextSiblingElement("IsInvisible");
		element->QueryBoolAttribute("value", &waterQuad.IsInvisible);
		///
		element = element->NextSiblingElement("HasLimitedDepth");
		element->QueryBoolAttribute("value", &waterQuad.HasLimitedDepth);
		///
		element = element->NextSiblingElement("z");
		element->QueryFloatAttribute("value", &waterQuad.z);
		///
		element = element->NextSiblingElement("a1");
		element->QueryFloatAttribute("value", &waterQuad.a1);
		///
		element = element->NextSiblingElement("a2");
		element->QueryFloatAttribute("value", &waterQuad.a2);
		///
		element = element->NextSiblingElement("a3");
		element->QueryFloatAttribute("value", &waterQuad.a3);
		///
		element = element->NextSiblingElement("a4");
		element->QueryFloatAttribute("value", &waterQuad.a4);
		///
		element = element->NextSiblingElement("NoStencil");
		element->QueryBoolAttribute("value", &waterQuad.NoStencil);

		WaterQuads.push_back(waterQuad);
	}

	printf("%d", test);
}

void GameData::LoadRpf(std::string& RpfPath)
{
	std::string Path("C:\\Program Files\\Rockstar Games\\Grand Theft Auto V\\");

	std::ifstream rpf(Path + RpfPath, std::ios::binary);
	if (!rpf.is_open()) {
		printf("NOT FOUND RPF!\n");
		return;
	}

	std::istream& fileStream(rpf);
	RpfFile* file = new RpfFile(fileStream, RpfPath);
	RpfFiles.push_back(file);

	for (auto& BinaryFileEntry : file->BinaryEntries)
	{
		if (BinaryFileEntry.Name.substr(BinaryFileEntry.Name.length() - 4) == ".rpf")
		{
			uint32_t RealFileSize = (BinaryFileEntry.FileSize == 0) ? BinaryFileEntry.FileUncompressedSize : BinaryFileEntry.FileSize;
			LoadRpf(fileStream, RpfPath, BinaryFileEntry.Name, RealFileSize, file->startPos + ((uint64_t)BinaryFileEntry.FileOffset * 512));
		}
	}
}

void GameData::LoadRpf(std::istream& rpf, std::string& FullPath_, std::string FileName_, uint32_t FileSize_, uint64_t FileOffset)
{
	RpfFile* file = new RpfFile(rpf, FullPath_, FileName_, FileSize_, FileOffset);
	RpfFiles.push_back(file);
	for (auto& BinaryFileEntry : file->BinaryEntries)
	{
		if (BinaryFileEntry.Name.substr(BinaryFileEntry.Name.length() - 4) == ".rpf")
		{
			uint32_t RealFileSize = (BinaryFileEntry.FileSize == 0) ? BinaryFileEntry.FileUncompressedSize : BinaryFileEntry.FileSize;
			LoadRpf(rpf, FullPath_, BinaryFileEntry.Name, RealFileSize, file->startPos + ((uint64_t)BinaryFileEntry.FileOffset * 512));
		}
	}
}

void GameData::ExtractFileResource(RpfResourceFileEntry entry, std::vector<uint8_t>& output)
{
	std::string Path("C:\\Program Files\\Rockstar Games\\Grand Theft Auto V\\");
	std::ifstream rpf(Path + entry.Path, std::ios::binary);

	rpf.seekg(entry.File->startPos + ((long)entry.FileOffset * 512));

	if (entry.FileSize > 0) {
		uint32_t offset = 0x10;
		uint32_t totlen = entry.FileSize - offset;

		uint8_t* tbytes = new uint8_t[totlen];

		rpf.seekg(offset, std::ios::cur);
		rpf.read((char*)&tbytes[0], (int)totlen);

		//uint8_t* decr = tbytes;
		//if (entry.IsEncrypted)
		//{
			/*if (IsAESEncrypted)
			{
				decr = GTACrypto.DecryptAES(tbytes);
			}
			else //if (IsNGEncrypted) //assume the archive is set to NG encryption if not AES... (comment: fix for openIV modded files)
			{*/
				//decr = GTACrypto.DecryptNG(tbytes, entry.Name, entry.FileSize);
			//}
			//else
			//{ }
		//}

		GTAEncryption::DecompressBytes(tbytes, totlen, output);
		delete[] tbytes;
		/*if (deflated != nullptr)
		{
			//return deflated;
		}
		else
		{
			entry.FileSize -= offset;
			return decr;
		}*/
	}

	//return nullptr;
}

uint32_t GameData::GenHash(std::string Name)
{
	uint32_t h = 0;
	for (int i = 0; i < Name.size(); i++)
	{
		h += (uint8_t)Name[i];
		h += (h << 10);
		h ^= (h >> 6);
	}
	h += (h << 3);
	h ^= (h >> 11);
	h += (h << 15);

	return h;
}
