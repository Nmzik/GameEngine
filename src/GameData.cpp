#include "GameData.h"
#include "AwcLoader.h"
#include "CacheDatFile.h"
#include "GTAEncryption.h"
#include "RpfFile.h"
#include "YmfLoader.h"
#include "YndLoader.h"
#include "YnvLoader.h"
#include "YtypLoader.h"

#include <pugixml.hpp>

GameData::GameData(std::string path)
:mainDirPath(path)
{
    tempBuffer = new uint8_t[40 * 1024 * 1024];
    nodes.resize(32 * 32);
    GTAEncryption::getInstance().loadKeys();
}

GameData::~GameData()
{
}

void GameData::load()
{
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
        std::string FilePath = mainDirPath + rpfFile;
        
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
    
    entries[ydr].reserve(58000);
    entries[ydd].reserve(8600);
    entries[yft].reserve(6100);
    entries[ytd].reserve(25600);
    entries[ybn].reserve(8800);
    entries[ymap].reserve(4600);
    entries[ynd].reserve(300);
    entries[ynv].reserve(4500);
    HDTextures.reserve(16481);
    
    for (auto& rpfFile : rpfFiles)
    {
        for (auto& entry : rpfFile->resourceEntries)
        {
            //    NO FILES DETECTED WITH uppercase in their names
            //    also it seems that full name (with extension) is never used in game engine
            //    std::transform(entry.FileName.begin(), entry.FileName.end(), entry.FileName.begin(), tolower);
            //    entry.FileNameHash = GenHash(entry.FileName);
            
            size_t index = entry.FileName.find_last_of('.');
            std::string extension = entry.FileName.substr(index);
            
            std::string_view FileNameNoExtension(entry.FileName);
            FileNameNoExtension.remove_suffix(FileNameNoExtension.size() - index);
            
            entry.ShortNameHash = GenHash(FileNameNoExtension);
            
            if (extension == ".ydr")
            {
                //    Ydrentries[GenHash(entry.FileName.substr(0, entry.FileName.length() - 4) + "_lod")] = &entry; //WHY????
                //    Ydrentries[entry.FileNameHash] = &entry;
                entries[ydr][entry.ShortNameHash] = &entry;
            }
            else if (extension == ".ydd")
            {
                //    Yddentries[entry.FileNameHash] = &entry;
                entries[ydd][entry.ShortNameHash] = &entry;
            }
            else if (extension == ".ycd")
            {
                //    Yddentries[entry.FileNameHash] = &entry;
                entries[ycd][entry.ShortNameHash] = &entry;
            }
            else if (extension == ".yft")
            {
                //    Yftentries[entry.FileNameHash] = &entry;
                entries[yft][entry.ShortNameHash] = &entry;
            }
            else if (extension == ".ytd")
            {
                //    Ytdentries[entry.FileNameHash] = &entry;
                entries[ytd][entry.ShortNameHash] = &entry;
            }
            else if (extension == ".ybn")
            {
                //    Ybnentries[entry.FileNameHash] = &entry;
                entries[ybn][entry.ShortNameHash] = &entry;
            }
            else if (extension == ".ymap")
            {
                //    Ymapentries[entry.FileNameHash] = &entry;
                entries[ymap][entry.ShortNameHash] = &entry;
            }
            else if (extension == ".ytyp")
            {
                //    Ytypentries[entry.FileNameHash] = &entry;
                //    Ytypentries[entry.ShortNameHash] = &entry;
                
                std::vector<uint8_t> outputBuffer(entry.UncompressedFileSize);
                extractFileResource(entry, outputBuffer.data(), outputBuffer.size());
                
                memstream stream(outputBuffer.data(), outputBuffer.size());
                
                YtypLoader file(stream);
                for (auto& def : file.ArchetypeDefs)
                {
                    if (def->getType() == 2)  //MLO
                    {
                        MloDictionary[def->BaseArchetypeDef.assetName] = file.fwEntityDefs;
                    }
                    archetypes[def->BaseArchetypeDef.assetName] = def;
                }
            }
            else if (extension == ".ynd")
            {
                std::vector<uint8_t> Buffer(entry.SystemSize + entry.GraphicsSize);
                extractFileResource(entry, &Buffer[0], Buffer.size());
                
                memstream stream(Buffer.data(), Buffer.size());
                std::unique_ptr<YndLoader> loader = std::make_unique<YndLoader>(stream);
                
                FileNameNoExtension.remove_prefix(5);
                int nodeID = std::stoi(FileNameNoExtension.data());
                nodes[nodeID] = std::move(loader);
                
                //    Yndentries[entry.FileNameHash] = &entry;
                entries[ynd][entry.ShortNameHash] = &entry;
            }
            else if (extension == ".ynv")
            {
                /*std::vector<uint8_t> Buffer(entry.SystemSize + entry.GraphicsSize);
                 extractFileResource(entry, &Buffer[0], Buffer.size());
                 
                 memstream stream(Buffer.data(), Buffer.size());
                 std::unique_ptr<YnvLoader> loader = std::make_unique<YnvLoader>(stream);*/
                //    Ynventries[entry.FileNameHash] = &entry;
                entries[ynv][entry.ShortNameHash] = &entry;
            }
            else if (extension == ".ysc")
            {
                //    Yscentries[entry.FileNameHash] = &entry;
                entries[ysc][entry.ShortNameHash] = &entry;
            }
        }
        for (auto& entry : rpfFile->binaryEntries)
        {
            size_t index = entry.FileName.find_last_of('.');
            std::string extension = entry.FileName.substr(index);
            
            std::string_view FileNameNoExtension(entry.FileName);
            FileNameNoExtension.remove_suffix(FileNameNoExtension.size() - index);
            
            entry.ShortNameHash = GenHash(FileNameNoExtension);
            
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
            
            else if (extension == ".awc")
            {
                //    Yscentries[entry.FileNameHash] = &entry;
                audios[entry.ShortNameHash] = &entry;
                
                /*static bool tested = false;
                 if (!tested)
                 {
                 tested = true;
                 std::vector<uint8_t> outputBuffer(entry.FileUncompressedSize);
                 //extractFileBinary(entry, outputBuffer);
                 auto& rpf = entry.File->rpf;
                 rpf.seekg(entry.FileOffset);
                 rpf.read((char*)&outputBuffer[0], entry.FileUncompressedSize);
                 
                 memstream stream(outputBuffer.data(), outputBuffer.size());
                 AwcLoader loader(stream);
                 }*/
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
    /*const int WATER_WORLD_SIZE = 10000.f;
     const int WATER_HQ_DATA_SIZE = 128;
     
     waterPosition.resize(25000);
     for (auto& water : WaterQuads)
     {
     auto wX = static_cast<int>((water.minX + water.maxX + WATER_WORLD_SIZE / 2.f) /
     (WATER_WORLD_SIZE / WATER_HQ_DATA_SIZE));
     auto wY = static_cast<int>((water.minY + water.maxY + WATER_WORLD_SIZE / 2.f) /
     (WATER_WORLD_SIZE / WATER_HQ_DATA_SIZE));
     
     int i = (wX * WATER_HQ_DATA_SIZE) + wY;
     int x = i < 0 ? 0 : i;
     
     waterPosition[i] = &water;
     }*/
    /*bool has_collision = false;
     size_t countBuvket = entries[ydd].bucket_count();
     for (size_t bucket = 0; bucket < entries[ydd].bucket_count(); bucket++) {
     
     if (entries[ydd].bucket_size(bucket) > 1) {
     has_collision = true;
     break;
     }
     }*/
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
        gtxdEntries[GenHash(childName)] = GenHash(ParentName);
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

            waterQuads.push_back(waterQuad);
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
    std::unique_ptr<RpfFile> file = std::make_unique<RpfFile>(&rpf, FullPath_, FileName_, FileSize_, FileOffset);

    for (auto& BinaryFileEntry : file->binaryEntries)
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

    rpf.read((char*)&tempBuffer[0], entry.FileSize);

    if (entry.File->isAESEncrypted)  //	HAPPENS WITH SOME YMF FILES
        GTAEncryption::getInstance().decryptAES(&tempBuffer[0], entry.FileSize);
    else
        GTAEncryption::getInstance().decryptNG(&tempBuffer[0], entry.FileSize, entry.FileName, entry.FileUncompressedSize);

    if (entry.FileSize > 0)  //awc
        GTAEncryption::getInstance().decompressBytes(&tempBuffer[0], entry.FileSize, output.data(), output.size());
}

void GameData::extractFileResource(RpfResourceFileEntry& entry, uint8_t* AllocatedMem, uint64_t AllocatedSize)
{
    auto& rpf = entry.File->rpf;

    rpf.seekg(entry.FileOffset);

    if (entry.FileSize > 40 * 1024 * 1024)
    {
        printf("ERROR BUFFER SIZE\n");
    }

    rpf.read((char*)&tempBuffer[0], entry.FileSize);

    if (entry.IsEncrypted)
    {
        if (entry.File->isAESEncrypted)
            GTAEncryption::getInstance().decryptAES(&tempBuffer[0], entry.FileSize);
        else
            GTAEncryption::getInstance().decryptNG(&tempBuffer[0], entry.FileSize, entry.FileName, entry.FileSize + 0x10);
    }

    GTAEncryption::getInstance().decompressBytes(&tempBuffer[0], entry.FileSize, AllocatedMem, AllocatedSize);
}
