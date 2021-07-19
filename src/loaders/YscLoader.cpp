#include "YscLoader.h"
#include <iterator>

#define SYSTEM_BASE_PTR(NAME) NAME = NAME & ~0x50000000;

uint64_t rotl(uint64_t hash, int rotate)
{
    rotate %= 64;
    return hash << rotate | hash >> (64 - rotate);
}

uint64_t getNativeHash(uint16_t index, const uint64_t nativeTable, uint64_t codeSize)
{
    uint8_t rotate = (index + codeSize) & 0x3F;
    return nativeTable << rotate | nativeTable >> (64 - rotate);
}

void YscLoader::Init(memstream& file)
{
    int32_t RSC7Offset = *(int32_t*)file.read(sizeof(int32_t)) == 0x37435352 ? 0x10 : 0x0;

    file.seekg(RSC7Offset);

    header = (ScriptHeader*)file.read(sizeof(ScriptHeader));

    SYSTEM_BASE_PTR(header->SubHeader);
    SYSTEM_BASE_PTR(header->CodeBlocksOffset);
    SYSTEM_BASE_PTR(header->StaticsOffset);
    SYSTEM_BASE_PTR(header->GlobalsOffset);
    SYSTEM_BASE_PTR(header->NativesOffset);
    SYSTEM_BASE_PTR(header->ScriptNameOffset);
    SYSTEM_BASE_PTR(header->StringsOffset);

    //
    file.seekg(header->ScriptNameOffset + RSC7Offset);
    ScriptName = file.getString();
    //
    int32_t CodeBlocks = (header->CodeLength + 0x3FFF) >> 14;
    file.seekg(header->CodeBlocksOffset + RSC7Offset);

    Statics.resize(header->StaticsCount);

    std::vector<uint32_t> CodeOffsets;
    CodeOffsets.reserve(CodeBlocks);
    for (int i = 0; i < CodeBlocks; i++)
    {
        uint32_t offset = *(uint32_t*)file.read(sizeof(uint32_t));
        SYSTEM_BASE_PTR(offset);
        offset += RSC7Offset;
        file.seekCur(4);
        CodeOffsets.push_back(offset);
    }

    //for each codeblock in codeblocks
    for (int i = 0; i < CodeBlocks; i++)
    {
        int tablesize = ((i + 1) * 0x4000 >= header->CodeLength) ? header->CodeLength % 0x4000 : 0x4000;
        std::copy(&file.data[CodeOffsets[i]], &file.data[CodeOffsets[i]] + tablesize, std::back_inserter(Code));
    }
    //natives
    file.seekg(header->NativesOffset + RSC7Offset);
    natives.reserve(header->NativesCount);
    for (int i = 0; i < header->NativesCount; i++)
    {
        uint64_t hash = *(uint64_t*)file.read(sizeof(uint64_t));
        uint64_t newone1 = getNativeHash(i, hash, header->CodeLength);
        uint64_t newone = rotl(hash, header->CodeLength + i);
        if (newone != newone1)
        {
            printf("");
        }
        natives.push_back(newone);
    }
    //strings
    int32_t StringBlocks = (header->StringsSize + 0x3FFF) >> 14;
    file.seekg(header->StringsOffset + RSC7Offset);

    std::vector<uint32_t> StringsBlocks;
    StringsBlocks.reserve(StringBlocks);
    for (int i = 0; i < StringBlocks; i++)
    {
        uint32_t offset = *(uint32_t*)file.read(sizeof(uint32_t));
        SYSTEM_BASE_PTR(offset);
        offset += RSC7Offset;
        file.seekg(offset);

        StringsBlocks.push_back(offset);
    }

    file.seekg(header->StringsOffset + RSC7Offset);
    Strings.resize(header->StringsSize);

    for (int i = 0, off = 0; i < StringsBlocks.size(); i++, off += 0x4000)
    {
        int tablesize = ((i + 1) * 0x4000 >= header->StringsSize) ? header->StringsSize % 0x4000 : 0x4000;
        file.seekg(StringsBlocks[i]);
        memcpy(&Strings[0], &file.data[file.tellg()], tablesize);
    }

    //SKIP FUNCTION DECLARATION
    //ip = Code[4] + 5;//SUPER STRANGE -- ENTRY POINT???
    paused = false;
    finished = false;

    ip = 0;
}
