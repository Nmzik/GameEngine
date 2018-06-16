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

	LoadHandlingData();
	LoadScenesSwitch();
	LoadWaterQuads();
	LoadGtxd();

	YdrEntries.reserve(110223);
	YddEntries.reserve(17500);
	YtdEntries.reserve(51008);
	YftEntries.reserve(12052);
	YmapEntries.reserve(9176);
	YbnEntries.reserve(17418);

	for (auto& rpfFile : RpfFiles)
	{
		for (auto& entry : rpfFile->ResourceEntries)
		{
			std::string extension = entry.Name.substr(entry.Name.length() - 4);
			std::transform(entry.Name.begin(), entry.Name.end(), entry.Name.begin(), tolower);
			//entry.NameHash = GenHash(entry.Name);
			size_t index = entry.Name.find_last_of('.');
			entry.ShortNameHash = GenHash(entry.Name.substr(0, index));

			if (extension == ".ydr") {
				//YdrEntries[GenHash(entry.Name.substr(0, entry.Name.length() - 4) + "_lod")] = &entry; //WHY????
				//YdrEntries[entry.NameHash] = &entry;
				YdrEntries[entry.ShortNameHash] = &entry;
			}
			else if (extension == ".ydd") {
				//YddEntries[entry.NameHash] = &entry;
				YddEntries[entry.ShortNameHash] = &entry;
			}
			else if (extension == ".yft") {
				//YftEntries[entry.NameHash] = &entry;
				YftEntries[entry.ShortNameHash] = &entry;
			}
			else if (extension == ".ynd") {
				//YndEntries[entry.NameHash] = &entry;
				YndEntries[entry.ShortNameHash] = &entry;
			}
			else if (extension == ".ynv") {
				//YnvEntries[entry.NameHash] = &entry;
				YnvEntries[entry.ShortNameHash] = &entry;
			}
			else if (extension == ".ytd") {
				//YtdEntries[entry.NameHash] = &entry;
				YtdEntries[entry.ShortNameHash] = &entry;
			}
			else if (extension == ".ybn") {
				//YbnEntries[entry.NameHash] = &entry;
				YbnEntries[entry.ShortNameHash] = &entry;
			}
			else if (entry.Name.substr(entry.Name.length() - 5) == ".ymap") {
				//YmapEntries[entry.NameHash] = &entry;
				YmapEntries[entry.ShortNameHash] = &entry;
			}
			else if (entry.Name.substr(entry.Name.length() - 5) == ".ytyp") {
				//YtypEntries[entry.NameHash] = &entry;
				//YtypEntries[entry.ShortNameHash] = &entry;

				std::vector<uint8_t> outputBuffer(1024 * 1024);
				ExtractFileResource(entry, outputBuffer);

				memstream stream(outputBuffer.data(), outputBuffer.size());

				YtypLoader file(stream);
				for (auto& def : file.CBaseArchetypeDefs)
				{
					CBaseArchetypeDefs[def.assetName] = def;
				}

				for (auto& def : file.CTimeArchetypeDefs)
				{
					CTimeArchetypeDefs[def._BaseArchetypeDef.assetName] = def;
				}

				/*if (file.CMloArchetypeDefs.size() != 0)
				{
					MloDictionary[file.CMloArchetypeDefs[0]._BaseArchetypeDef.assetName] = file.CEntityDefs;
				}*/
			}
		}
	}
}


GameData::~GameData()
{
}

void GameData::LoadHandlingData()
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult = doc.LoadFile("C:\\Users\\nmzik\\Desktop\\handling.meta");

	tinyxml2::XMLElement * root = doc.FirstChildElement("CHandlingDataMgr");

	if (root == nullptr) printf("ERROR");

	tinyxml2::XMLElement* element = root->FirstChildElement("HandlingData");
	if (element == nullptr) printf("ERROR ELEMENT");

	for (tinyxml2::XMLElement* e = element->FirstChildElement("Item"); e != NULL; e = e->NextSiblingElement("Item"))
	{
		CarHandling car;

		tinyxml2::XMLElement* element = e->FirstChildElement("handlingName");
		std::string CarName = element->FirstChild()->Value();

		std::transform(CarName.begin(), CarName.end(), CarName.begin(), tolower);
		car.Hash = GenHash(CarName);
		///
		element = element->NextSiblingElement("fMass");
		element->QueryFloatAttribute("value", &car.mass);

		car.file = nullptr;

		Vehicles.push_back(car);
	}
}

void GameData::LoadGtxd()
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult = doc.LoadFile("C:\\Users\\nmzik\\Desktop\\gtxd.ymt.rbf.xml");

	tinyxml2::XMLElement * root = doc.FirstChildElement("CMapParentTxds");

	if (root == nullptr) printf("ERROR");

	tinyxml2::XMLElement* element = root->FirstChildElement("txdRelationships");
	if (element == nullptr) printf("ERROR ELEMENT");

	for (tinyxml2::XMLElement* e = element->FirstChildElement("item"); e != NULL; e = e->NextSiblingElement("item"))
	{
		tinyxml2::XMLElement* element = e->FirstChildElement("parent");
		std::string ParentName = element->FirstChild()->Value();
		std::transform(ParentName.begin(), ParentName.end(), ParentName.begin(), tolower);

		element = e->FirstChildElement("child");
		std::string childName = element->FirstChild()->Value();
		GtxdEntries[GenHash(childName)] = GenHash(ParentName);
	}
}

void GameData::LoadWaterQuads()
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult = doc.LoadFile("C:\\Users\\nmzik\\Desktop\\water.xml");

	//tinyxml2::XMLNode* pRoot = doc.FirstChild();
	//if(pRoot == nullptr) printf("ERRORMAIN");
	tinyxml2::XMLElement * root = doc.FirstChildElement("WaterData");

	if (root == nullptr) printf("ERROR");

	tinyxml2::XMLElement* element = root->FirstChildElement("WaterQuads");
	if (element == nullptr) printf("ERROR ELEMENT");

	//tinyxml2::XMLElement* ItemElement = element->FirstChildElement("Item");
	//if (ItemElement == nullptr) printf("ERROR ELEMENT");

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
}

void GameData::LoadScenesSwitch()
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult = doc.LoadFile("C:\\Users\\nmzik\\Desktop\\playerswitchestablishingshots.meta");

	tinyxml2::XMLElement * root = doc.FirstChildElement("CPlayerSwitchEstablishingShotMetadataStore");

	if (root == nullptr) printf("ERROR");

	tinyxml2::XMLElement* element = root->FirstChildElement("ShotList");
	if (element == nullptr) printf("ERROR ELEMENT");

	for (tinyxml2::XMLElement* e = element->FirstChildElement("Item"); e != NULL; e = e->NextSiblingElement("Item"))
	{
		tinyxml2::XMLElement* element = e->FirstChildElement("Name");
		std::string Name = element->FirstChild()->Value();
		///
		element = element->NextSiblingElement("Position");
		glm::vec3 Position;
		element->QueryFloatAttribute("x", &Position.x);
		element->QueryFloatAttribute("y", &Position.y);
		element->QueryFloatAttribute("z", &Position.z);
		Scenes.push_back(Position);
	}
}

void GameData::LoadRpf(std::string& RpfPath)
{
	RpfFile* file = new RpfFile(RpfPath);
	RpfFiles.push_back(file);

	for (auto& BinaryFileEntry : file->BinaryEntries)
	{
		if (BinaryFileEntry.Name.substr(BinaryFileEntry.Name.length() - 4) == ".rpf")
		{
			uint32_t RealFileSize = (BinaryFileEntry.FileSize == 0) ? BinaryFileEntry.FileUncompressedSize : BinaryFileEntry.FileSize;
			LoadRpf(*file->rpf, RpfPath, BinaryFileEntry.Name, RealFileSize, file->startPos + ((uint64_t)BinaryFileEntry.FileOffset * 512));
		}
	}
}

void GameData::LoadRpf(std::ifstream& rpf, std::string& FullPath_, std::string FileName_, uint32_t FileSize_, uint64_t FileOffset)
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
	auto& rpf = entry.File->rpf;

	rpf->seekg(entry.FileOffset);

	uint8_t* tbytes = new uint8_t[entry.FileSize];
	rpf->read((char*)&tbytes[0], entry.FileSize);

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

	GTAEncryption::DecompressBytes(tbytes, entry.FileSize, output);
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
	//}

	//return nullptr;
}
