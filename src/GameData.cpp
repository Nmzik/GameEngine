#include "GameData.h"
#include "CacheDatFile.h"
#include "GTAEncryption.h"
#include "RpfFile.h"
#include "YmfLoader.h"
#include "YtypLoader.h"

#include <pugixml.hpp>

GameData::GameData(std::string Path)
{
    TempBuffer = new uint8_t[40 * 1024 * 1024];
    GTAEncryption::getInstance().LoadKeys();

    std::array<std::string, 25> RpfsFiles = {
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
        "update\\update.rpf"};

    for (std::string& rpfFile : RpfsFiles)
    {
        std::string FilePath = Path + rpfFile;

        std::unique_ptr<std::ifstream> rpf = std::make_unique<std::ifstream>(FilePath, std::ios::binary);

        if (!rpf->is_open())
        {
            printf("NOT FOUND RPF %s!\n", (FilePath).c_str());
            continue;
        }

        rpf->seekg(0, std::ios::end);
        uint32_t FileSize = (uint32_t)rpf->tellg();
        rpf->seekg(0, std::ios::beg);

        loadRpf(*rpf, rpfFile, rpfFile, FileSize, 0);

        openedFiles.push_back(std::move(rpf));
    }

    loadGtxd();

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

            size_t index = entry.FileName.find_last_of('.');
            std::string extension = entry.FileName.substr(index);

            std::string_view FileNameNoExtension(entry.FileName);
            FileNameNoExtension.remove_suffix(FileNameNoExtension.size() - index);

            entry.ShortNameHash = GenHash(FileNameNoExtension);

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
                extractFileResource(entry, outputBuffer.data(), outputBuffer.size());

                memstream stream(outputBuffer.data(), outputBuffer.size());

                YtypLoader file(stream);
                for (auto& def : file.ArchetypeDefs)
                {
                    if (def->GetType() == 2)  //MLO
                    {
                        MloDictionary[def->BaseArchetypeDef.assetName] = file.fwEntityDefs;
                    }
                    Archetypes[def->BaseArchetypeDef.assetName] = def;
                }
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
            else if (extension == ".ysc")
            {
                //	YscEntries[entry.FileNameHash] = &entry;
                Entries[ysc][entry.ShortNameHash] = &entry;
            }
        }
        for (auto& entry : rpfFile->BinaryEntries)
        {
            size_t index = entry.FileName.find_last_of('.');
            std::string extension = entry.FileName.substr(index);

            if (extension == ".ymf")
            {
                std::vector<uint8_t> outputBuffer(entry.FileUncompressedSize);
                extractFileBinary(entry, outputBuffer);

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
                extractFileBinary(entry, Buffer);
                loadHandlingData(Buffer);
            }
            else if (entry.FileName == "gta5_cache_y.dat")
            {
                std::vector<uint8_t> Buffer(entry.FileUncompressedSize);
                extractFileBinary(entry, Buffer);

                cacheFile = std::make_unique<CacheDatFile>(Buffer);
            }
            else if (entry.FileName == "water.xml")
            {
                std::vector<uint8_t> Buffer(entry.FileUncompressedSize);
                extractFileBinary(entry, Buffer);
                loadWaterQuads(Buffer);
            }
            else if (entry.FileName == "playerswitchestablishingshots.meta")
            {
                std::vector<uint8_t> Buffer(entry.FileUncompressedSize);
                extractFileBinary(entry, Buffer);
                loadScenesSwitch(Buffer);
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

void GameData::loadHandlingData(std::vector<uint8_t>& Buffer)
{
    pugi::xml_document doc;
    auto error = doc.load_buffer_inplace(Buffer.data(), Buffer.size());

    pugi::xml_node root = doc.child("CHandlingDataMgr");

    pugi::xml_node element = root.child("HandlingData");

    for (pugi::xml_node e = element.first_child(); e != NULL; e = e.next_sibling())  //ITEM
    {
        pugi::xml_node element = e.child("handlingName");
        //
        CarHandling car;
        //
        std::string CarName = element.first_child().value();
        std::transform(CarName.begin(), CarName.end(), CarName.begin(), tolower);
        //
        car.Hash = GenHash(CarName);
        car.mass = element.next_sibling("fMass").attribute("value").as_float();
        car.file = nullptr;

        VehiclesInfo.push_back(car);
    }
}

void GameData::loadGtxd()
{
    pugi::xml_document doc;
    auto error = doc.load_file("assets/gtxd.ymt.rbf.xml");

    pugi::xml_node root = doc.child("CMapParentTxds");

    pugi::xml_node element = root.child("txdRelationships");

    for (pugi::xml_node e = element.first_child(); e != NULL; e = e.next_sibling())  //item
    {
        pugi::xml_node element = e.child("parent");
        std::string ParentName = element.first_child().value();
        std::transform(ParentName.begin(), ParentName.end(), ParentName.begin(), tolower);

        element = e.child("child");
        std::string childName = element.first_child().value();
        GtxdEntries[GenHash(childName)] = GenHash(ParentName);
    }
}

void GameData::loadWaterQuads(std::vector<uint8_t>& Buffer)
{
    pugi::xml_document doc;
    auto error = doc.load_buffer_inplace(Buffer.data(), Buffer.size());

    pugi::xml_node root = doc.child("WaterData");

    pugi::xml_node element = root.child("WaterQuads");

    for (pugi::xml_node e = element.first_child(); e != NULL; e = e.next_sibling())  //item
    {
        if (e.first_child() != nullptr)
        {  // water.xml DLC
            WaterQuad waterQuad;
            pugi::xml_node element = e.child("minX");
            waterQuad.minX = element.attribute("value").as_float();
            ///
            element = element.next_sibling("maxX");
            waterQuad.maxX = element.attribute("value").as_float();
            ///
            element = element.next_sibling("minY");
            waterQuad.minY = element.attribute("value").as_float();
            ///
            element = element.next_sibling("maxY");
            waterQuad.maxY = element.attribute("value").as_float();
            ///
            element = element.next_sibling("Type");
            waterQuad.Type = element.attribute("value").as_int();
            ///
            element = element.next_sibling("IsInvisible");
            waterQuad.IsInvisible = element.attribute("value").as_bool();
            ///
            element = element.next_sibling("HasLimitedDepth");
            waterQuad.HasLimitedDepth = element.attribute("value").as_bool();
            ///
            element = element.next_sibling("z");
            waterQuad.z = element.attribute("value").as_float();
            ///
            element = element.next_sibling("a1");
            waterQuad.a1 = element.attribute("value").as_float();
            ///
            element = element.next_sibling("a2");
            waterQuad.a2 = element.attribute("value").as_float();
            ///
            element = element.next_sibling("a3");
            waterQuad.a3 = element.attribute("value").as_float();
            ///
            element = element.next_sibling("a4");
            waterQuad.a4 = element.attribute("value").as_float();
            ///
            element = element.next_sibling("NoStencil");
            waterQuad.NoStencil = element.attribute("value").as_bool();

            WaterQuads.push_back(waterQuad);
        }
    }
}

void GameData::loadScenesSwitch(std::vector<uint8_t>& Buffer)
{
    pugi::xml_document doc;
    auto error = doc.load_buffer_inplace(Buffer.data(), Buffer.size());

    pugi::xml_node root = doc.child("CPlayerSwitchEstablishingShotMetadataStore");

    pugi::xml_node element = root.child("ShotList");

    for (pugi::xml_node e = element.child("Item"); e != NULL; e = e.next_sibling("Item"))
    {
        pugi::xml_node element = e.child("Name");
        std::string Name = element.first_child().value();
        ///
        element = element.next_sibling("Position");
        //
        glm::vec3 Position;
        Position.x = element.attribute("x").as_float();
        Position.y = element.attribute("y").as_float();
        Position.z = element.attribute("z").as_float();
        Scenes.push_back(Position);
    }
}

void GameData::loadRpf(std::ifstream& rpf, std::string& FullPath_, std::string& FileName_, uint32_t FileSize_, uint64_t FileOffset)
{
    if (FileName_ == "chinesesimp.rpf")
    {
        printf("");
    }
    std::unique_ptr<RpfFile> file = std::make_unique<RpfFile>(&rpf, FullPath_, FileName_, FileSize_, FileOffset);

    for (auto& BinaryFileEntry : file->BinaryEntries)
    {
        if (BinaryFileEntry.FileName == "chinesesimp.rpf")
        {
            printf("");
            continue;
        }

        if (BinaryFileEntry.FileName.substr(BinaryFileEntry.FileName.length() - 4) == ".rpf")
        {
            uint32_t RealFileSize = (BinaryFileEntry.FileSize == 0) ? BinaryFileEntry.FileUncompressedSize : BinaryFileEntry.FileSize;
            loadRpf(rpf, FullPath_, BinaryFileEntry.FileName, RealFileSize, BinaryFileEntry.FileOffset);
        }
    }
    rpfFiles.push_back(std::move(file));
}

void GameData::extractFileBinary(RpfBinaryFileEntry& entry, std::vector<uint8_t>& output)
{
    auto& rpf = entry.File->rpf;

    rpf.seekg(entry.FileOffset);

    if (entry.FileSize > 40 * 1024 * 1024)
    {
        printf("ERROR BUFFER SIZE\n");
    }

    rpf.read((char*)&TempBuffer[0], entry.FileSize);

    if (entry.File->IsAESEncrypted)  //	HAPPENS WITH SOME YMF FILES
        GTAEncryption::getInstance().decryptAES(&TempBuffer[0], entry.FileSize);
    else
        GTAEncryption::getInstance().decryptNG(&TempBuffer[0], entry.FileSize, entry.FileName, entry.FileUncompressedSize);

    GTAEncryption::getInstance().decompressBytes(&TempBuffer[0], entry.FileSize, output.data(), output.size());
}

void GameData::extractFileResource(RpfResourceFileEntry& entry, uint8_t* AllocatedMem, uint64_t AllocatedSize)
{
    auto& rpf = entry.File->rpf;

    rpf.seekg(entry.FileOffset);

    if (entry.FileSize > 40 * 1024 * 1024)
    {
        printf("ERROR BUFFER SIZE\n");
    }

    rpf.read((char*)&TempBuffer[0], entry.FileSize);

    if (entry.IsEncrypted)
    {
        if (entry.File->IsAESEncrypted)
            GTAEncryption::getInstance().decryptAES(&TempBuffer[0], entry.FileSize);
        else
            GTAEncryption::getInstance().decryptNG(&TempBuffer[0], entry.FileSize, entry.FileName, entry.FileSize + 0x10);
    }

    GTAEncryption::getInstance().decompressBytes(&TempBuffer[0], entry.FileSize, AllocatedMem, AllocatedSize);
}