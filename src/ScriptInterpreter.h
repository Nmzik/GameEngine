#pragma once
#include <functional>
#include <unordered_map>
#include "loaders/YscLoader.h"
#include <glm/ext.hpp>

class GameData;

enum Instruction  //opcodes reversed from gta v default.xex
{
    Nop = 0,
    iAdd,          //1
    iSub,          //2
    iMult,         //3
    iDiv,          //4
    iMod,          //5
    iNot,          //6
    iNeg,          //7
    iCmpEq,        //8
    iCmpNe,        //9
    iCmpGt,        //10
    iCmpGe,        //11
    iCmpLt,        //12
    iCmpLe,        //13
    fAdd,          //14
    fSub,          //15
    fMult,         //16
    fDiv,          //17
    fMod,          //18
    fNeg,          //19
    fCmpEq,        //20
    fCmpNe,        //21
    fCmpGt,        //22
    fCmpGe,        //23
    fCmpLt,        //24
    fCmpLe,        //25
    vAdd,          //26
    vSub,          //27
    vMult,         //28
    vDiv,          //29
    vNeg,          //30
    And,           //31
    Or,            //32
    Xor,           //33
    ItoF,          //34
    FtoI,          //35
    FtoV,          //36
    iPushByte1,    //37
    iPushByte2,    //38
    iPushByte3,    //39
    iPushInt,      //40
    fPush,         //41
    dupInstr,      //42
    pop,           //43
    Native,        //44
    Enter,         //45
    Return,        //46
    pGet,          //47
    pSet,          //48
    pPeekSet,      //49
    ToStack,       //50
    FromStack,     //51
    pArray1,       //52
    ArrayGet1,     //53
    ArraySet1,     //54
    pFrame1,       //55
    GetFrame1,     //56
    SetFrame1,     //57
    pStatic1,      //58
    StaticGet1,    //59
    StaticSet1,    //60
    Add1,          //61
    Mult1,         //62
    pStructStack,  //63
    pStruct1,      //64
    GetStruct1,    //65
    SetStruct1,    //66
    iPushShort,    //67
    Add2,          //68
    Mult2,         //69
    pStruct2,      //70
    GetStruct2,    //71
    SetStruct2,    //72
    pArray2,       //73
    ArrayGet2,     //74
    ArraySet2,     //75
    pFrame2,       //76
    GetFrame2,     //77
    SetFrame2,     //78
    pStatic2,      //79
    StaticGet2,    //80
    StaticSet2,    //81
    pGlobal2,      //82
    GlobalGet2,    //83
    GlobalSet2,    //84
    Jump,          //85
    JumpFalse,     //86
    JumpNe,        //87
    JumpEq,        //88
    JumpLe,        //89
    JumpLt,        //90
    JumpGe,        //91
    JumpGt,        //92
    Call,          //93
    pGlobal3,      //94
    GlobalGet3,    //95
    GlobalSet3,    //96
    iPushI24,      //97
    Switch,        //98
    PushString,    //99
    GetHash,       //100
    StrCopy,       //101
    ItoS,          //102
    StrConCat,     //103
    StrConCatInt,  //104
    MemCopy,       //105
    Catch,         //106	 //No handling of these as Im unsure exactly how they work
    Throw,         //107 //No script files in the game use these opcodes
    pCall,         //108
    iPush_n1,      //109
    iPush_0,       //110
    iPush_1,       //111
    iPush_2,       //112
    iPush_3,       //113
    iPush_4,       //114
    iPush_5,       //115
    iPush_6,       //116
    iPush_7,       //117
    fPush_n1,      //118
    fPush_0,       //119
    fPush_1,       //120
    fPush_2,       //121
    fPush_3,       //122
    fPush_4,       //123
    fPush_5,       //124
    fPush_6,       //125
    fPush_7        //126
};

using ScriptParams = std::vector<ScriptParameter>;

class ScriptInterpreter;

class ScriptArguments
{
    ScriptParams* parameters;
    YscLoader* script;
    ScriptInterpreter* interpreter;

public:
    ScriptArguments(ScriptParams* p, YscLoader* scriptFile, ScriptInterpreter* scriptInterpreter)
        : parameters(p)
        , script(scriptFile)
        , interpreter(scriptInterpreter)
    {
    }

    ScriptParams* getParams() const
    {
        return parameters;
    }

    ScriptInterpreter* getInterpreter() const
    {
        return interpreter;
    }

    YscLoader* getScript() const
    {
        return script;
    }
};

struct functionTable
{
    int numArguments;
    std::function<void(const ScriptArguments&)> function;
};

class Game;

class ScriptInterpreter
{
    GameData* data;
    Game* game;

    void executeThread(YscLoader& file);

    std::unordered_map<uint64_t, functionTable> functions;

public:
    ScriptInterpreter(GameData* gameData, Game* _game);
    ~ScriptInterpreter();

    bool startThread(uint32_t ScriptHash);
    void execute();

    std::unordered_map<uint32_t, YscLoader*> LoadedScripts;

    Game* getGame() const
    {
        return game;
    }
};
