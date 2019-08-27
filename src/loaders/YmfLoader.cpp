#include "YmfLoader.h"
#include <algorithm>

uint8_t YmfLoader::DetectType(memstream& stream)
{
    uint32_t signature = *(uint32_t*)stream.read(sizeof(uint32_t));
    endSwap(&signature);

    if (signature == 0x30464252)
    {
        printf("RBF\n");
        return 0;
    }
    else if (signature == 1347635534)
    {
        //	printf("PSO\n");
        return 1;
    }
    return 2;
}

YmfLoader::YmfLoader(memstream& stream)
{
    uint8_t type = DetectType(stream);

    if (type != 1)
    {
        //	printf("ERROR YMF\n");
        //	CAUSES ERROR IN ZLIB ALSO
        return;
    }

    stream.seekg(0);

    while (stream.tellg() < stream.bufferSize)
    {
        uint32_t section = *(uint32_t*)stream.read(sizeof(uint32_t));
        endSwap(&section);

        uint32_t length = *(uint32_t*)stream.read(sizeof(uint32_t));
        endSwap(&length);

        stream.seekCur(-8);

        switch (section)
        {
            case PSIN:
            {
                //	printf("PSIN\n");
                _PsoDataSection.Read(stream);
                break;
            }
            case PMAP:
                //	printf("PMAP\n");
                _PsoDataMapSection.Read(stream);
                break;
            case PSCH:
                //	printf("PSCH\n");
                stream.seekCur(length);
                break;
            case PSIG:
                //	printf("PSIG\n");
                stream.seekCur(length);
                break;
            case STRF:
                //	printf("STRF\n");
                stream.seekCur(length);
                break;
            case STRS:
                //	printf("STRS\n");
                stream.seekCur(length);
                break;
            case STRE:
                //	printf("STRE\n");
                stream.seekCur(length);
                break;
            case CHKS:
                //	printf("CHKS\n");
                stream.seekCur(length);
                break;
            default:
                //	printf("SECTION NOT FOUND\n");
                break;
        }
    }

    struct Array_Structure  //16 bytes - pointer for a structure array
    {
        uint32_t Pointer;
        uint32_t Unk0;
        uint16_t Count1;
        uint16_t Count2;
        uint32_t Unk1;
    };

    struct CPackFileMetaData  //96 bytes, Type:0
    {
        Array_Structure MapDataGroups;       //0   MapDataGroups: Array: 0  {256: Structure: 0: 3260758307}
        Array_Structure HDTxdBindingArray;   //16   HDTxdBindingArray: Array: 16: 2  {256: Structure: 0: CHDTxdAssetBinding}
        Array_Structure imapDependencies;    //32   imapDependencies: Array: 32: 4  {256: Structure: 0: 3501026914}
        Array_Structure imapDependencies_2;  //48   imapDependencies_2: Array: 48: 6  {256: Structure: 0: 3240050401}
        Array_Structure itypDependencies_2;  //64   itypDependencies_2: Array: 64: 8  {256: Structure: 0: 1515605584}
        Array_Structure Interiors;
    };

    int main = _PsoDataMapSection.RootId - 1;
    stream.seekg(_PsoDataMapSection.Entries[main].Offset);

    CPackFileMetaData* cpak = (CPackFileMetaData*)stream.read(sizeof(CPackFileMetaData));

    struct CInteriorBoundsFiles  //24 bytes, Type:0   // Interiors
    {
        uint32_t Name;     //0   Name: INT_0Bh: 0
        uint32_t Unused0;  //4
        Array_uint Bounds;
    };
    if (cpak->Interiors.Count1 > 0)
    {
        // assert("");
        /* endSwap(&cpak->Interiors.Pointer);
        uint32_t PointerIndex = (cpak->Interiors.Pointer & 0xFFF) - 1;
       endSwap(&PointerIndex);
        stream.seekg(PointerIndex);

        std::vector<CInteriorBoundsFiles*> tests;
        for (int i = 0; i < cpak->Interiors.Count1; i++)
        {
            CInteriorBoundsFiles* test = (CInteriorBoundsFiles*)stream.read(sizeof(CInteriorBoundsFiles));
            endSwap(&test->Name);
			//
            endSwap(&test->Bounds.Pointer);
            uint32_t entryid = test->Bounds.Pointer & 0xFFF;

            uint32_t entryoffset = (test->Bounds.Pointer >> 12) & 0xFFFFF;

			
            uint32_t arrentry = _PsoDataMapSection.Entries[(int)entryid - 1].Offset;
                        int totoffset = arrentry + (int)entryoffset;
            std::vector<uint32_t> Hashes;
            Hashes.resize(test->Bounds.Count1);
            memcpy(&Hashes[0], &_PsoDataSection.Data[totoffset], test->Bounds.Count1);
            //tests.push_back(test);
        }*/
    }

    for (int i = 0; i < _PsoDataMapSection.EntriesCount; i++)
    {
        if (_PsoDataMapSection.Entries[i].NameHash == 0x59869c63)
        {  //	SD => HD TEXTURES

            stream.seekg(_PsoDataMapSection.Entries[i].Offset);

            for (int j = 0; j < _PsoDataMapSection.Entries[i].Length / sizeof(CHDTxdAssetBinding); j++)
            {
                CHDTxdAssetBinding* texture = (CHDTxdAssetBinding*)stream.read(sizeof(CHDTxdAssetBinding));
                HDtextures.push_back(texture);
            }
        }

        else if (_PsoDataMapSection.Entries[i].NameHash == 947288719)
        {  //	SD => HD TEXTURES

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
