#include "GameData.h"

#include "YtypLoader.h"
#include "CacheDatFile.h"

GameData::GameData()
{
	GTAEncryption::LoadKeys();

	std::vector <std::string> RpfsFiles = {
		"common.rpf",
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
		std::string Path("C:\\Program Files\\Rockstar Games\\Grand Theft Auto V\\");

		std::ifstream* rpf = myNew std::ifstream(Path + rpfFile, std::ios::binary);
		Files.push_back(rpf);

		if (!rpf->is_open()) {
			printf("NOT FOUND RPF!\n");
			return;
		}

		rpf->seekg(0, std::ios::end);
		uint32_t FileSize = (uint32_t)rpf->tellg();
		rpf->seekg(0, std::ios::beg);

		LoadRpf(*rpf, rpfFile, rpfFile, FileSize, 0);
	}

	LoadGtxd();

	YdrEntries.reserve(55112);
	YddEntries.reserve(8582);
	YtdEntries.reserve(25504);
	YftEntries.reserve(6026);
	YmapEntries.reserve(4588);
	YbnEntries.reserve(8709);

	bool foundHandling = false;
	bool WaterFound = false;

	for (auto& rpfFile : RpfFiles)
	{
		for (auto& entry : rpfFile->ResourceEntries)
		{
			//NO FILES DETECTED WITH uppercase in their names
			//also it seems that full name (with extension) is never used in game engine
			//std::transform(entry.Name.begin(), entry.Name.end(), entry.Name.begin(), tolower);
			//entry.NameHash = GenHash(entry.Name);

			size_t index = entry.Name.find_last_of('.');
			std::string extension = entry.Name.substr(index);
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
			else if (extension == ".ymap") {
				//YmapEntries[entry.NameHash] = &entry;
				YmapEntries[entry.ShortNameHash] = &entry;
			}
			else if (extension == ".ytyp") {
				//YtypEntries[entry.NameHash] = &entry;
				//YtypEntries[entry.ShortNameHash] = &entry;

				std::vector<uint8_t> outputBuffer(entry.SystemSize + entry.GraphicsSize);
				ExtractFileResource(entry, outputBuffer);

				memstream2 stream(outputBuffer.data(), outputBuffer.size());

				YtypLoader file(stream);
				for (auto& def : file.ArchetypeDefs)
				{
					Archetypes[def->BaseArchetypeDef.assetName] = def;
				}

				/*if (file.CMloArchetypeDefs.size() != 0)
				{
					MloDictionary[file.CMloArchetypeDefs[0]._BaseArchetypeDef.assetName] = file.CEntityDefs;
				}*/
			}
		}
		for (auto& entry : rpfFile->BinaryEntries)
		{
			if (entry.Name == "handling.meta" && !foundHandling) {
				std::vector<uint8_t> Buffer(entry.FileUncompressedSize);
				ExtractFileBinary(entry, Buffer);
				LoadHandlingData(Buffer);
				foundHandling = true;
			}
			if (entry.Name == "gta5_cache_y.dat") {
				std::vector<uint8_t> Buffer(entry.FileUncompressedSize);
				ExtractFileBinary(entry, Buffer);

				cacheFile = std::make_unique<CacheDatFile>(Buffer);
			}
			if (entry.Name == "water.xml" && !WaterFound) {
				std::vector<uint8_t> Buffer(entry.FileUncompressedSize);
				ExtractFileBinary(entry, Buffer);
				LoadWaterQuads(Buffer);
				WaterFound = true;
			}
			if (entry.Name == "playerswitchestablishingshots.meta") {
				std::vector<uint8_t> Buffer(entry.FileUncompressedSize);
				ExtractFileBinary(entry, Buffer);
				LoadScenesSwitch(Buffer);
			}
		}
	}
}


GameData::~GameData()
{
	GTAEncryption::Cleanup();

	for (auto& rpf : RpfFiles)
	{
		delete rpf;
	}

	for (auto& file : Files)
	{
		delete file;
	}
}

void GameData::LoadHandlingData(std::vector<uint8_t>& Buffer)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult = doc.Parse((char*)&Buffer[0], Buffer.size());

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
	tinyxml2::XMLError eResult = doc.LoadFile("assets/gtxd.ymt.rbf.xml");

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

void GameData::LoadWaterQuads(std::vector<uint8_t>& Buffer)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult = doc.Parse((char*)&Buffer[0], Buffer.size());

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

void GameData::LoadScenesSwitch(std::vector<uint8_t>& Buffer)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult = doc.Parse((char*)&Buffer[0], Buffer.size());

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

void GameData::LoadRpf(std::ifstream& rpf, std::string& FullPath_, std::string& FileName_, uint32_t FileSize_, uint64_t FileOffset)
{
	RpfFile* file = myNew RpfFile(rpf, FullPath_, FileName_, FileSize_, FileOffset);
	RpfFiles.push_back(file);

	for (auto& BinaryFileEntry : file->BinaryEntries)
	{
		if (BinaryFileEntry.Name.substr(BinaryFileEntry.Name.length() - 4) == ".rpf")
		{
			uint32_t RealFileSize = (BinaryFileEntry.FileSize == 0) ? BinaryFileEntry.FileUncompressedSize : BinaryFileEntry.FileSize;
			LoadRpf(rpf, FullPath_, BinaryFileEntry.Name, RealFileSize, BinaryFileEntry.FileOffset);
		}
	}
}

uint8_t tbytes[20*1024*1024]; //20MB ?? IS IT ENOUGHT???

void GameData::ExtractFileBinary(RpfBinaryFileEntry& entry, std::vector<uint8_t>& output)
{
	auto& rpf = entry.File->rpf;

	rpf->seekg(entry.FileOffset);
	rpf->read((char*)&tbytes[0], entry.FileSize);

	GTAEncryption::DecryptNG(tbytes, entry.FileSize, entry.Name, entry.FileUncompressedSize);

	GTAEncryption::DecompressBytes(tbytes, entry.FileSize, output);
}

void GameData::ExtractFileResource(RpfResourceFileEntry& entry, std::vector<uint8_t>& output)
{
	auto& rpf = entry.File->rpf;

	rpf->seekg(entry.FileOffset);
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
}
