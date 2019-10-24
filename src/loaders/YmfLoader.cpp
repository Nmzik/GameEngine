#include "YmfLoader.h"
#include <algorithm>

YmfLoader::YmfLoader(memstream& stream)
{
    uint32_t signature = *(uint32_t*)stream.read(sizeof(uint32_t));
    //endSwap(&signature);

    //stream.seekg(0);

    switch (signature)
    {
        case 0x30464252:
            printf("RBF\n");
            loadRBF(stream);
            break;
        case 1347635534:
            printf("PSO\n");
            loadPSO(stream);
            break;
        default:
            printf("Unrecognized file type\n");
            return;
    }

    uint64_t bufSize = stream.getSize();

    while (stream.tellg() < bufSize)
    {
        uint8_t descriptorIndex = *(uint8_t*)stream.read(sizeof(uint8_t));
        if (descriptorIndex == 0xFF)  // close tag
        {
            uint8_t b = *(uint8_t*)stream.read(sizeof(uint8_t));
            if (b != 0xFF)
                printf("Expected 0xFF but was ");

            if (structures.size() > 0)
            {
                current = structures.top();
                structures.pop();
            }
            else
            {
                if (stream.tellg() != stream.getSize())
                    printf("Expected end of stream but was not.");
                break;
                //return current;
            }
        }
        else if (descriptorIndex == 0xFD)  // bytes
        {
            uint8_t b = *(uint8_t*)stream.read(sizeof(uint8_t));
            if (b != 0xFF)
                printf("Expected 0xFF but was ");
            int32_t dataLength = *(int32_t*)stream.read(sizeof(int32_t));

            uint8_t* data = (uint8_t*)stream.read(sizeof(uint8_t) * dataLength);
            char* testString = (char*)data;

            static bool isFirst = true;
            static int index = 0;
            if (isFirst)
            {
                gtxd.push_back(std::pair("", ""));
                gtxd[index].first = testString;
                isFirst = false;
            }
            else
            {
                gtxd[index].second = testString;
                index++;
                isFirst = true;
            }

            //RbfBytes* bytesValue = new RbfBytes();
            //bytesValue->Value = data;
            //current->Children.push_back(bytesValue);
        }
        else
        {
            uint8_t dataType = *(uint8_t*)stream.read(sizeof(uint8_t));
            if (descriptorIndex == descriptors.size())  // new descriptor + data
            {
                int16_t nameLength = *(int16_t*)stream.read(sizeof(int16_t));
                //var nameBytes = reader.ReadBytes(nameLength);
                //std::string name = Encoding.ASCII.GetString(nameBytes);
                std::string name = stream.getString();
                stream.seekCur(name.size());

                if (name.size() != nameLength)
                    assert(false);

                RbfEntryDescription descriptor;
                descriptor.Name = name;
                descriptor.Type = dataType;
                descriptors.push_back(descriptor);

                ParseElement(stream, descriptors.size() - 1, dataType);
            }
            else  // existing descriptor + data
            {
                if (dataType != descriptors[descriptorIndex].Type)
                {
                    //throw new Exception("Data type does not match. Expected "
                    //    + descriptors[descriptorIndex].Type.ToString() + " but found "
                    //    + dataType.ToString() + ". Descriptor: " + descriptors[descriptorIndex].Name);
                }

                ParseElement(stream, descriptorIndex, dataType);
            }
        }
    }

    /*while (stream.tellg() < bufSize)
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
    /*}

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
    }*/
}

YmfLoader::~YmfLoader()
{
}

void YmfLoader::loadRBF(memstream& stream)
{
}

void YmfLoader::loadPSO(memstream& stream)
{
}

void YmfLoader::ParseElement(memstream& stream, int descriptorIndex, uint8_t dataType)
{
    RbfEntryDescription descriptor = descriptors[descriptorIndex];
    switch (dataType)  //(descriptor.Type)
    {
        case 0:  // open element...
        {
            std::unique_ptr<RbfStructure> structureValue = std::make_unique<RbfStructure>();
            structureValue->Name = descriptor.Name;

            if (current != nullptr)
            {
                current->Children.push_back(structureValue.get());
                structures.push(current);
            }

			current = structureValue.get();
			vectorOfStructures.push_back(std::move(structureValue));

            // 6 bytes
            int16_t x1 = *(int16_t*)stream.read(sizeof(int16_t));
            int16_t x2 = *(int16_t*)stream.read(sizeof(int16_t));
            int16_t x3 = *(int16_t*)stream.read(sizeof(int16_t));
            //if (x1 != 0)
            //    throw new Exception("unexpected");
            //if (x2 != 0)
            //    throw new Exception("unexpected");
            //if (x3 != 0)
            //    throw new Exception("unexpected");
            break;
        }
        case 0x10:
        {
            RbfUint32* intValue = new RbfUint32();
            intValue->Name = descriptor.Name;
            intValue->Value = *(int32_t*)stream.read(sizeof(int32_t));
            current->Children.push_back(intValue);
            break;
        }
        case 0x20:
        {
            RbfBoolean* booleanValue = new RbfBoolean();
            booleanValue->Name = descriptor.Name;
            booleanValue->Value = true;
            current->Children.push_back(booleanValue);
            break;
        }
        case 0x30:
        {
            RbfBoolean* booleanValue = new RbfBoolean();
            booleanValue->Name = descriptor.Name;
            booleanValue->Value = false;
            current->Children.push_back(booleanValue);
            break;
        }
        case 0x40:
        {
            RbfFloat* floatValue = new RbfFloat();
            floatValue->Name = descriptor.Name;
            floatValue->Value = *(float*)stream.read(sizeof(float));
            current->Children.push_back(floatValue);
            break;
        }
        case 0x50:
        {
            RbfFloat3* floatVectorValue = new RbfFloat3();
            floatVectorValue->Name = descriptor.Name;
            floatVectorValue->X = *(float*)stream.read(sizeof(float));
            floatVectorValue->Y = *(float*)stream.read(sizeof(float));
            floatVectorValue->Z = *(float*)stream.read(sizeof(float));
            current->Children.push_back(floatVectorValue);
            break;
        }
        case 0x60:
        {
            int16_t valueLength = *(int16_t*)stream.read(sizeof(int16_t));
            //var valueBytes = reader.ReadBytes(valueLength);
            //var value = Encoding.ASCII.GetString(valueBytes);
            std::string value = stream.getString();
            RbfString* stringValue = new RbfString();
            stringValue->Name = descriptor.Name;
            stringValue->Value = value;
            current->Children.push_back(stringValue);
            break;
        }
        default:
            printf("Unsupported data type.");
    }
}
