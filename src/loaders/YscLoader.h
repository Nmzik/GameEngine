#pragma once
#include <stack>
#include <string>
#include <vector>

#include "FileType.h"

enum ScriptType
{
    Int32 = 0,
    Float,
    String,
    Undefined
};

struct ScriptParameter
{
    ScriptType type;
    union {
        int32_t integer;
        float floatValue;
        char* string;
    };

    ScriptParameter()
        : type(Undefined)
        , integer(-1)
    {
    }

    ScriptParameter(ScriptType VarType, int32_t value)
        : type(VarType)
        , integer(value)
    {
    }

    ScriptParameter(ScriptType VarType, float value)
        : type(VarType)
        , floatValue(value)
    {
    }

    std::string GetVariable()
    {
        switch (type)
        {
            case Int32:
                return std::to_string(integer);
            case Float:
                return std::to_string(floatValue);
            default:
                return std::string("UNIMPLEMENTED, ERROR in CONVERSION");
        }
    }
};

class YscLoader : public FileType
{
    struct ScriptHeader
    {
        int32_t Magic;
        int32_t Unknown;
        int32_t SubHeader;  //wtf?
        int32_t Unknown1;
        int32_t CodeBlocksOffset;
        int32_t Unknown2;
        int32_t GlobalsVersion;  //Not sure if this is the globals version
        int32_t CodeLength;      //Total length of code
        int32_t ParameterCount;  //Count of paremeters to the script
        int32_t StaticsCount;
        int32_t GlobalsCount;
        int32_t NativesCount;  //Native count * 4 = total block length
        int32_t StaticsOffset;
        int32_t Unknown3;
        int32_t GlobalsOffset;
        int32_t Unknown4;
        int32_t NativesOffset;
        int32_t Unknown5;
        int32_t Null1;  //unknown
        int32_t Unknown6;
        int32_t Null2;  //Unknown
        int32_t Unknown7;
        int32_t NameHash;  //Hash of the script name at ScriptNameOffset
        int32_t Null3;
        int32_t ScriptNameOffset;
        int32_t Unknown8;
        int32_t StringsOffset;  //Offset of the string table
        int32_t Unknown9;
        int32_t StringsSize;  //Total length of the string block
        int32_t Unknown10;
        int32_t Null4;
        int32_t Unknown11;
    };

    ScriptHeader* header;
    std::vector<uint8_t> data;
    std::stack<ScriptParameter> localStack;

public:
    std::vector<uint8_t> Code;
    std::vector<uint64_t> natives;
    std::vector<uint8_t> Strings;
    std::vector<ScriptParameter> Statics;
    ///
    //bool condition;
    std::vector<ScriptParameter> locals;
    std::stack<int32_t> calls;
    bool paused;
    bool finished;
    uint32_t ip;
    std::string ScriptName;

    inline void pushToStack(ScriptParameter param)
    {
        localStack.push(param);
    }

    inline ScriptParameter popFromStack()
    {
        ScriptParameter top = localStack.top();
        localStack.pop();
        return top;
    }

    inline ScriptParameter getTopValueFromUnmodifiedStack()
    {
        return localStack.top();
    }

    bool isStackEmpty() const
    {
        return localStack.size() == 0;
    }

    void Init(memstream& file);
};
