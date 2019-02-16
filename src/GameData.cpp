#include "GameData.h"

#include "YmfLoader.h"

#include "CacheDatFile.h"
#include "GTAEncryption.h"
#include "YtypLoader.h"

#include "tinyxml2.h"

GameData::GameData()
{
	TempBuffer = new uint8_t[40 * 1024 * 1024];
	GTAEncryption::getInstance().LoadKeys();

	std::vector<std::string> RpfsFiles = {
	    "common.rpf", "x64a.rpf", "x64b.rpf", "x64c.rpf", "x64d.rpf", "x64e.rpf", "x64f.rpf", "x64g.rpf", "x64h.rpf", "x64i.rpf", "x64j.rpf", "x64k.rpf",
	    "x64l.rpf",   "x64m.rpf", "x64n.rpf", "x64o.rpf", "x64p.rpf", "x64q.rpf", "x64r.rpf", "x64s.rpf", "x64t.rpf", "x64u.rpf", "x64v.rpf", "x64w.rpf",
	};

	for (std::string& rpfFile : RpfsFiles)
	{
		std::string Path("C:\\Program Files\\Rockstar Games\\Grand Theft Auto V\\");

		std::unique_ptr<std::ifstream> rpf = std::make_unique<std::ifstream>(Path + rpfFile, std::ios::binary);

		if (!rpf->is_open())
		{
			printf("NOT FOUND RPF!\n");
			continue;
		}

		rpf->seekg(0, std::ios::end);
		uint32_t FileSize = (uint32_t)rpf->tellg();
		rpf->seekg(0, std::ios::beg);

		LoadRpf(*rpf, rpfFile, rpfFile, FileSize, 0);

		openedFiles.push_back(std::move(rpf));
	}

	LoadGtxd();

	Entries[ydr].reserve(58000);
	Entries[ydd].reserve(8600);
	Entries[yft].reserve(6100);
	Entries[ytd].reserve(25600);
	Entries[ybn].reserve(8800);
	Entries[ymap].reserve(4600);
	Entries[ynd].reserve(300);
	Entries[ynv].reserve(4500);
	HDTextures.reserve(16481);

	for (auto& rpfFile : rpfFiles)
	{
		for (auto& entry : rpfFile->ResourceEntries)
		{
			//	NO FILES DETECTED WITH uppercase in their names
			//	also it seems that full name (with extension) is never used in game engine
			//	std::transform(entry.FileName.begin(), entry.FileName.end(), entry.FileName.begin(), tolower);
			//	entry.FileNameHash = GenHash(entry.FileName);

			size_t index          = entry.FileName.find_last_of('.');
			std::string extension = entry.FileName.substr(index);
			entry.ShortNameHash   = GenHash(entry.FileName.substr(0, index));

			if (extension == ".ydr")
			{
				//	YdrEntries[GenHash(entry.FileName.substr(0, entry.FileName.length() - 4) + "_lod")] = &entry; //WHY????
				//	YdrEntries[entry.FileNameHash] = &entry;
				Entries[ydr][entry.ShortNameHash] = &entry;
			}
			else if (extension == ".ydd")
			{
				//	YddEntries[entry.FileNameHash] = &entry;
				Entries[ydd][entry.ShortNameHash] = &entry;
			}
			else if (extension == ".ycd")
			{
				//	YddEntries[entry.FileNameHash] = &entry;
				Entries[ycd][entry.ShortNameHash] = &entry;
			}
			else if (extension == ".yft")
			{
				//	YftEntries[entry.FileNameHash] = &entry;
				Entries[yft][entry.ShortNameHash] = &entry;
			}
			else if (extension == ".ytd")
			{
				//	YtdEntries[entry.FileNameHash] = &entry;
				Entries[ytd][entry.ShortNameHash] = &entry;
			}
			else if (extension == ".ybn")
			{
				//	YbnEntries[entry.FileNameHash] = &entry;
				Entries[ybn][entry.ShortNameHash] = &entry;
			}
			else if (extension == ".ymap")
			{
				//	YmapEntries[entry.FileNameHash] = &entry;
				Entries[ymap][entry.ShortNameHash] = &entry;
			}
			else if (extension == ".ytyp")
			{
				//	YtypEntries[entry.FileNameHash] = &entry;
				//	YtypEntries[entry.ShortNameHash] = &entry;

				std::vector<uint8_t> outputBuffer(entry.UncompressedFileSize);
				ExtractFileResource(entry, outputBuffer.data(), outputBuffer.size());

				memstream stream(outputBuffer.data(), outputBuffer.size());

				YtypLoader file(stream);
				for (auto& def : file.ArchetypeDefs)
				{
					Archetypes[def->BaseArchetypeDef.assetName] = def;
				}

				/*if (file.CMloArchetypeDefs.size() != 0)
				{
				 MloDictionary[file.CMloArchetypeDefs[0]._BaseArchetypeDef.assetName] = file.fwEntityDefs;
				}*/
			}
			else if (extension == ".ynd")
			{
				//	YndEntries[entry.FileNameHash] = &entry;
				Entries[ynd][entry.ShortNameHash] = &entry;
			}
			else if (extension == ".ynv")
			{
				//	YnvEntries[entry.FileNameHash] = &entry;
				Entries[ynv][entry.ShortNameHash] = &entry;
			}
		}
		for (auto& entry : rpfFile->BinaryEntries)
		{
			size_t index          = entry.FileName.find_last_of('.');
			std::string extension = entry.FileName.substr(index);

			if (extension == ".ymf")
			{
				std::vector<uint8_t> outputBuffer(entry.FileUncompressedSize);
				ExtractFileBinary(entry, outputBuffer);

				memstream stream(outputBuffer.data(), outputBuffer.size());
				YmfLoader loader(stream);

				for (auto texture : loader.HDtextures)
				{
					HDTextures[GenHash(texture->targetAsset)] = GenHash(texture->HDTxd);
				}
			}

			else if (entry.FileName == "handling.meta")
			{
				std::vector<uint8_t> Buffer(entry.FileUncompressedSize);
				ExtractFileBinary(entry, Buffer);
				LoadHandlingData(Buffer);
			}
			else if (entry.FileName == "gta5_cache_y.dat")
			{
				std::vector<uint8_t> Buffer(entry.FileUncompressedSize);
				ExtractFileBinary(entry, Buffer);

				cacheFile = std::make_unique<CacheDatFile>(Buffer);
			}
			else if (entry.FileName == "water.xml")
			{
				std::vector<uint8_t> Buffer(entry.FileUncompressedSize);
				ExtractFileBinary(entry, Buffer);
				LoadWaterQuads(Buffer);
			}
			else if (entry.FileName == "playerswitchestablishingshots.meta")
			{
				std::vector<uint8_t> Buffer(entry.FileUncompressedSize);
				ExtractFileBinary(entry, Buffer);
				LoadScenesSwitch(Buffer);
			}
		}
	}

	/*bool has_collision = false;
	size_t countBuvket = Entries[ydd].bucket_count();
	for (size_t bucket = 0; bucket < Entries[ydd].bucket_count(); bucket++) {

	 if (Entries[ydd].bucket_size(bucket) > 1) {
	  has_collision = true;
	  break;
	 }
	}*/
}

GameData::~GameData()
{
}

void GameData::LoadHandlingData(std::vector<uint8_t>& Buffer)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult = doc.Parse((char*)&Buffer[0], Buffer.size());

	tinyxml2::XMLElement* root = doc.FirstChildElement("CHandlingDataMgr");

	tinyxml2::XMLElement* element = root->FirstChildElement("HandlingData");

	for (tinyxml2::XMLElement* e = element->FirstChildElement("Item"); e != NULL; e = e->NextSiblingElement("Item"))
	{
		CarHandling car;

		tinyxml2::XMLElement* element = e->FirstChildElement("handlingName");
		std::string CarName           = element->FirstChild()->Value();

		std::transform(CarName.begin(), CarName.end(), CarName.begin(), tolower);
		car.Hash = GenHash(CarName);
		///
		element = element->NextSiblingElement("fMass");
		element->QueryFloatAttribute("value", &car.mass);

		car.file = nullptr;

		VehiclesInfo.push_back(car);
	}
}

void GameData::LoadGtxd()
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult = doc.LoadFile("assets/gtxd.ymt.rbf.xml");

	tinyxml2::XMLElement* root = doc.FirstChildElement("CMapParentTxds");

	tinyxml2::XMLElement* element = root->FirstChildElement("txdRelationships");

	for (tinyxml2::XMLElement* e = element->FirstChildElement("item"); e != NULL; e = e->NextSiblingElement("item"))
	{
		tinyxml2::XMLElement* element = e->FirstChildElement("parent");
		std::string ParentName        = element->FirstChild()->Value();
		std::transform(ParentName.begin(), ParentName.end(), ParentName.begin(), tolower);

		element                         = e->FirstChildElement("child");
		std::string childName           = element->FirstChild()->Value();
		GtxdEntries[GenHash(childName)] = GenHash(ParentName);
	}
}

void GameData::LoadWaterQuads(std::vector<uint8_t>& Buffer)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult = doc.Parse((char*)&Buffer[0], Buffer.size());

	tinyxml2::XMLElement* root = doc.FirstChildElement("WaterData");

	tinyxml2::XMLElement* element = root->FirstChildElement("WaterQuads");

	for (tinyxml2::XMLElement* e = element->FirstChildElement("Item"); e != NULL; e = e->NextSiblingElement("Item"))
	{
		if (e->FirstChild() != nullptr)
		{ // water.xml DLC
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
}

void GameData::LoadScenesSwitch(std::vector<uint8_t>& Buffer)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult = doc.Parse((char*)&Buffer[0], Buffer.size());

	tinyxml2::XMLElement* root = doc.FirstChildElement("CPlayerSwitchEstablishingShotMetadataStore");

	tinyxml2::XMLElement* element = root->FirstChildElement("ShotList");

	for (tinyxml2::XMLElement* e = element->FirstChildElement("Item"); e != NULL; e = e->NextSiblingElement("Item"))
	{
		tinyxml2::XMLElement* element = e->FirstChildElement("Name");
		std::string Name              = element->FirstChild()->Value();
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
	std::unique_ptr<RpfFile> file = std::make_unique<RpfFile>(rpf, FullPath_, FileName_, FileSize_, FileOffset);

	for (auto& BinaryFileEntry : file->BinaryEntries)
	{
		if (BinaryFileEntry.FileName.substr(BinaryFileEntry.FileName.length() - 4) == ".rpf")
		{
			uint32_t RealFileSize = (BinaryFileEntry.FileSize == 0) ? BinaryFileEntry.FileUncompressedSize : BinaryFileEntry.FileSize;
			LoadRpf(rpf, FullPath_, BinaryFileEntry.FileName, RealFileSize, BinaryFileEntry.FileOffset);
		}
	}
	rpfFiles.push_back(std::move(file));
}

void GameData::ExtractFileBinary(RpfBinaryFileEntry& entry, std::vector<uint8_t>& output)
{
	auto& rpf = entry.File->rpf;

	rpf->seekg(entry.FileOffset);

	if (entry.FileSize > 40 * 1024 * 1024)
	{
		printf("ERROR BUFFER SIZE\n");
	}

	rpf->read((char*)&TempBuffer[0], entry.FileSize);

	if (entry.File->IsAESEncrypted) //	HAPPENS WITH SOME YMF FILES
		GTAEncryption::getInstance().DecryptAES(&TempBuffer[0], entry.FileSize);
	else
		GTAEncryption::getInstance().DecryptNG(TempBuffer, entry.FileSize, entry.FileName, entry.FileUncompressedSize);

	GTAEncryption::getInstance().DecompressBytes(TempBuffer, entry.FileSize, output.data(), output.size());
}

void GameData::ExtractFileResource(RpfResourceFileEntry& entry, uint8_t* AllocatedMem, uint64_t AllocatedSize)
{
	auto& rpf = entry.File->rpf;

	rpf->seekg(entry.FileOffset);

	if (entry.FileSize > 40 * 1024 * 1024)
	{
		printf("ERROR BUFFER SIZE\n");
	}

	rpf->read((char*)&TempBuffer[0], entry.FileSize);

	//	uint8_t* decr = tbytes;
	//	if (entry.IsEncrypted)
	//{
	/*if (IsAESEncrypted)
	{
	 decr = GTACrypto.DecryptAES(tbytes);
	}
	else //if (IsNGEncrypted) //assume the archive is set to NG encryption if not AES... (comment: fix for openIV modded files)
	{*/
	//	decr = GTACrypto.DecryptNG(tbytes, entry.FileName, entry.FileSize);
	//}
	//	else
	//{ }
	//}

	GTAEncryption::getInstance().DecompressBytes(TempBuffer, entry.FileSize, AllocatedMem, AllocatedSize);
}
