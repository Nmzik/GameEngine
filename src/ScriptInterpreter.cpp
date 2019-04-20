#include "ScriptInterpreter.h"

#include "GTAEncryption.h"
#include "Game.h"
#include "GameData.h"
#include "GameWorld.h"
#include "ResourceManager.h"

#define DEBUG_PRINTF

#ifdef DEBUG_PRINTF
#define printf printf
#else
#define printf \
    if (false) printf
#endif  // DEBUGPRINTF

namespace GAMEPLAY
{
    void NETWORK_SET_SCRIPT_IS_SAFE_FOR_NETWORK_GAME(const ScriptArguments& args)
    {
        printf("NETWORK_SET_SCRIPT_IS_SAFE_FOR_NETWORK_GAME");
    }

    void GET_GAME_TIMER(const ScriptArguments& args)
    {
        std::vector<ScriptParameter>* param = args.getParams();

        printf("GET_GAME_TIMER");

        int32_t var = (int32_t)(args.getInterpreter()->getGame()->gameTime * 1000.f);

        args.getScript()->pushToStack({Int32, var});
    }

}  // namespace GAMEPLAY

namespace PLAYER
{
    void SET_PLAYER_MODEL(const ScriptArguments& args)
    {
        std::vector<ScriptParameter>* param = args.getParams();

        printf("SET_PLAYER_MODEL %d 0x%x", (*param)[1].integer, (*param)[0].integer);
    }

    void PLAYER_ID(const ScriptArguments& args)
    {
        printf("PLAYER_ID");

        args.getScript()->pushToStack({Int32, 0});
    }

    void PLAYER_PED_ID(const ScriptArguments& args)
    {
        printf("PLAYER_PED_ID");

        args.getScript()->pushToStack({Int32, 0});
    }

    void SET_PLAYER_CONTROL(const ScriptArguments& args)
    {
        printf("SET_PLAYER_CONTROL");
    }
}  // namespace PLAYER

namespace ENTITY
{
    void IS_ENTITY_DEAD(const ScriptArguments& args)
    {
        printf("IS_ENTITY_DEAD");

        args.getScript()->pushToStack({Int32, 0});
    }

    void SET_ENTITY_COORDS(const ScriptArguments& args)
    {
        printf("SET_ENTITY_COORDS");

        std::vector<ScriptParameter>* param = args.getParams();
        glm::vec3 pos((*param)[6].floatValue, (*param)[5].floatValue, (*param)[4].floatValue);

        args.getInterpreter()->getGame()->getWorld()->getCurrentPlayer()->setPosition(pos);
    }

    void SET_ENTITY_HEADING(const ScriptArguments& args)
    {
        printf("SET_ENTITY_HEADING");
    }
}  // namespace ENTITY

namespace SCRIPT
{
    void REQUEST_SCRIPT(const ScriptArguments& args)
    {
        std::vector<ScriptParameter>* param = args.getParams();

        printf("REQUEST_SCRIPT STRING:%s", (char*)&args.getScript()->Strings[(*param)[0].integer]);

        args.getInterpreter()->startThread(GenHash((char*)&args.getScript()->Strings[(*param)[0].integer]));
    }

    void HAS_SCRIPT_LOADED(const ScriptArguments& args)
    {
        ScriptParams* param = args.getParams();

        printf("HAS_SCRIPT_LOADED %s", (char*)&args.getScript()->Strings[(*param)[0].integer]);

        if (args.getInterpreter()->LoadedScripts[GenHash((char*)&args.getScript()->Strings[(*param)[0].integer])]->Loaded)
            args.getScript()->pushToStack({Int32, 1});  //WE ASSUME IT WAS LOADED
        else
            args.getScript()->pushToStack({Int32, 0});
    }

    void SHUTDOWN_LOADING_SCREEN(const ScriptArguments& args)
    {
        std::vector<ScriptParameter>* param = args.getParams();

        args.getScript()->finished = true;
        args.getScript()->paused = true;

        printf("SHUTDOWN_LOADING_SCREEN");
    }
}  // namespace SCRIPT

namespace SYSTEM
{
    void WAIT(const ScriptArguments& args)
    {
        ScriptParams* param = args.getParams();

        args.getScript()->paused = true;

        printf("WAIT %d", (*param)[0].integer);
    }

    void START_NEW_SCRIPT(const ScriptArguments& args)
    {
        ScriptParams* param = args.getParams();

        printf("START_NEW_SCRIPT %s %d", (char*)&args.getScript()->Strings[(*param)[1].integer], (*param)[0].integer);

        args.getScript()->pushToStack({Int32, 1});
    }

    void VMAG(const ScriptArguments& args)
    {
        ScriptParams* param = args.getParams();

        glm::vec3 vec((*param)[2].floatValue, (*param)[1].floatValue, (*param)[0].floatValue);
        printf("VMAG %s", glm::to_string(vec).c_str());

        args.getScript()->pushToStack({Float, glm::length(vec)});
    }
}  // namespace SYSTEM

namespace STREAMING
{
    void REQUEST_IPL(const ScriptArguments& args)
    {
        std::vector<ScriptParameter>* param = args.getParams();

        printf("REQUEST_IPL STRING:%s", (char*)&args.getScript()->Strings[(*param)[0].integer]);
    }

    void REMOVE_IPL(const ScriptArguments& args)
    {
        std::vector<ScriptParameter>* param = args.getParams();
        printf("REMOVE_IPL ID STRING:%s", (char*)&args.getScript()->Strings[(*param)[0].integer]);
    }

    void REQUEST_MODEL(const ScriptArguments& args)
    {
        std::vector<ScriptParameter>* param = args.getParams();

        printf("REQUEST_MODEL HASH 0x%x", (*param)[0].integer);
    }

    void NEW_LOAD_SCENE_START_SPHERE(const ScriptArguments& args)
    {
        std::vector<ScriptParameter>* param = args.getParams();

        printf("NEW_LOAD_SCENE_START_SPHERE 0x%x", (*param)[0].integer);

        args.getScript()->pushToStack({Int32, 1});
    }

    void HAS_MODEL_LOADED(const ScriptArguments& args)
    {
        std::vector<ScriptParameter>* param = args.getParams();

        printf("HAS_MODEL_LOADED HASH 0x%x", (*param)[0].integer);
        //true
        args.getScript()->pushToStack({Int32, 1});
    }

    void IS_NEW_LOAD_SCENE_LOADED(const ScriptArguments& args)
    {
        std::vector<ScriptParameter>* param = args.getParams();

        printf("IS_NEW_LOAD_SCENE_LOADED");

        args.getScript()->pushToStack({Int32, 0});
    }

}  // namespace STREAMING

namespace UI
{
    void DISPLAY_RADAR(const ScriptArguments& args)
    {
        std::vector<ScriptParameter>* param = args.getParams();

        printf("DISPLAY_RADAR 0x%x", (*param)[0].integer);
    }
}  // namespace UI

namespace UNKNOWN
{
    void UNKNOWN1(const ScriptArguments& args)
    {
        std::vector<ScriptParameter>* param = args.getParams();
        printf("UNKNOWN1");
    }
}  // namespace UNKNOWN

ScriptInterpreter::ScriptInterpreter(GameData* gameData, Game* _game)
    : data(gameData)
    , game(_game)
{
    functions.insert({0x5341E3E598550C46, {0, GAMEPLAY::NETWORK_SET_SCRIPT_IS_SAFE_FOR_NETWORK_GAME}});
    functions.insert({0xfd34717937104f1c, {0, GAMEPLAY::GET_GAME_TIMER}});   //
    functions.insert({0xD75A4240F0AD8613, {1, SCRIPT::REQUEST_SCRIPT}});     //
    functions.insert({0xB1FDFCECA75CE4A7, {1, SCRIPT::HAS_SCRIPT_LOADED}});  //
    functions.insert({0x3F79D277EE09D761, {0, SCRIPT::SHUTDOWN_LOADING_SCREEN}});
    functions.insert({0x4EDE34FBADD967A6, {1, SYSTEM::WAIT}});
    functions.insert({0xE81651AD79516E48, {2, SYSTEM::START_NEW_SCRIPT}});
    functions.insert({0x652D2EEEF1D3E62C, {3, SYSTEM::VMAG}});
    functions.insert({0x1639295c71657bcf, {1, STREAMING::REQUEST_IPL}});
    functions.insert({0x981516504eae59e2, {1, STREAMING::REMOVE_IPL}});
    functions.insert({0xcacebbbead8f262e, {1, STREAMING::REQUEST_MODEL}});
    functions.insert({0x7fc99ccc73354033, {1, STREAMING::HAS_MODEL_LOADED}});
    functions.insert({0x543a213bf1ab6832, {5, STREAMING::NEW_LOAD_SCENE_START_SPHERE}});
    functions.insert({0xd3512cb88bb3513f, {0, STREAMING::IS_NEW_LOAD_SCENE_LOADED}});
    functions.insert({0x8422f72077ab2d3e, {2, UNKNOWN::UNKNOWN1}});
    functions.insert({0xd4e735f4b6a956ac, {0, PLAYER::PLAYER_ID}});
    functions.insert({0xa19140a5c42d8715, {0, PLAYER::PLAYER_PED_ID}});
    functions.insert({0x41b27626566b361, {2, PLAYER::SET_PLAYER_MODEL}});
    functions.insert({0x962cf6a9ea6fdcac, {3, PLAYER::SET_PLAYER_CONTROL}});
    functions.insert({0x7112137033807390, {1, ENTITY::IS_ENTITY_DEAD}});
    functions.insert({0x4299736016aece26, {8, ENTITY::SET_ENTITY_COORDS}});
    functions.insert({0x92ddcf53aa47a63d, {2, ENTITY::SET_ENTITY_HEADING}});
    functions.insert({0xdc374da4dc566237, {1, UI::DISPLAY_RADAR}});
}

ScriptInterpreter::~ScriptInterpreter()
{
}

void ScriptInterpreter::executeThread(YscLoader& file)
{
    printf("\n---------------------");

    uint32_t& ip = file.ip;

    //we assume that main function is first
    //TODO

    while (!file.paused)
    {
        printf("\nADDRESS %d OPCODE %d ", ip, file.Code[ip]);
        switch (file.Code[ip])
        {
            case Nop:
                printf("Nop");
                ip++;
                break;
            case iAdd:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Int32 || var2.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("iAdd %d %d", var2.integer, var1.integer);
                file.pushToStack({Int32, var2.integer + var1.integer});

                ip++;
            }
            break;
            case iSub:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Int32 || var2.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("iSub %d %d", var2.integer, var1.integer);
                file.pushToStack({Int32, var2.integer - var1.integer});

                ip++;
            }
            break;
            case iMult:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Int32 || var2.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("iMult %d %d", var2.integer, var1.integer);
                file.pushToStack({Int32, var2.integer * var1.integer});

                ip++;
            }
            break;
            case iDiv:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Int32 || var2.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("iDiv %d %d", var2.integer, var1.integer);
                file.pushToStack({Int32, var2.integer / var1.integer});

                ip++;
            }
            break;
            case iMod:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Int32 || var2.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("iMod %d %d", var2.integer, var1.integer);
                file.pushToStack({Int32, var2.integer % var1.integer});

                ip++;
            }
            break;
            case iNot:  //Convert variable to opposite one
            {
                ScriptParameter var = file.popFromStack();

                if (var.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                file.pushToStack({Int32, !var.integer});
                printf("iNot %d", var.integer);
                ip++;
            }
            break;
            case iNeg:
            {
                ScriptParameter var = file.popFromStack();

                if (var.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                file.pushToStack({Int32, -var.integer});
                printf("iNeg %d", var.integer);
                ip++;
            }
            break;
            case iCmpEq:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Int32 || var2.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("iCmpEq %d %d", var2.integer, var1.integer);
                file.pushToStack({Int32, var2.integer == var1.integer});

                ip++;
            }
            break;
            case iCmpNe:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Int32 || var2.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("iCmpNe %d %d", var2.integer, var1.integer);
                file.pushToStack({Int32, var2.integer != var1.integer});

                ip++;
            }
            break;
            case iCmpGt:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Int32 || var2.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("iCmpGt %d %d", var2.integer, var1.integer);
                file.pushToStack({Int32, var2.integer > var1.integer});

                ip++;
            }
            break;
            case iCmpGe:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Int32 || var2.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("iCmpGe %d %d", var2.integer, var1.integer);
                file.pushToStack({Int32, var2.integer >= var1.integer});

                ip++;
            }
            break;
            case iCmpLt:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Int32 || var2.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("iCmpLt %d %d", var2.integer, var1.integer);
                file.pushToStack({Int32, var2.integer < var1.integer});

                ip++;
            }
            break;
            case iCmpLe:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Int32 || var2.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("iCmpLe %d %d", var2.integer, var1.integer);
                file.pushToStack({Int32, var2.integer <= var1.integer});

                ip++;
            }
            break;
            case fAdd:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Float || var2.type != Float)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("fAdd %f %f", var1.floatValue, var2.floatValue);
                file.pushToStack({Float, var2.floatValue + var1.floatValue});

                ip++;
            }
            break;
            case fSub:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Float || var2.type != Float)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("fSub %f %f", var1.floatValue, var2.floatValue);
                file.pushToStack({Float, var2.floatValue - var1.floatValue});

                ip++;
            }
            break;
            case fMult:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Float || var2.type != Float)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("fMult %f %f", var2.floatValue, var1.floatValue);
                file.pushToStack({Float, var2.floatValue * var1.floatValue});

                ip++;
            }
            break;
            case fDiv:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Float || var2.type != Float)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("fDiv %f %f", var2.floatValue, var1.floatValue);
                file.pushToStack({Float, var2.floatValue / var1.floatValue});

                ip++;
            }
            break;
            case fMod:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Float || var2.type != Float)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("fMod UNIMPLEMENTED %f %f", var2.floatValue, var1.floatValue);
                //file.pushToStack(f2 % f1);

                ip++;
            }
            break;
            case fNeg:
            {
                ScriptParameter var1 = file.popFromStack();

                if (var1.type != Float)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("fNeg %f", var1.floatValue);
                file.pushToStack({Float, -var1.floatValue});

                ip++;
            }
            break;
            case fCmpEq:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Float || var2.type != Float)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("fCmpEq %f %f", var2.floatValue, var1.floatValue);
                file.pushToStack({Float, var2.floatValue == var1.floatValue});

                ip++;
            }
            break;
            case fCmpNe:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Float || var2.type != Float)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("fCmpNe %f %f", var2.floatValue, var1.floatValue);
                file.pushToStack({Float, var2.floatValue != var1.floatValue});

                ip++;
            }
            break;
            case fCmpGt:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Float || var2.type != Float)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("fCmpGt %f %f", var2.floatValue, var1.floatValue);
                file.pushToStack({Float, var2.floatValue > var1.floatValue});

                ip++;
            }
            break;
            case fCmpGe:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Float || var2.type != Float)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("fCmpGe %f %f", var2.floatValue, var1.floatValue);
                file.pushToStack({Float, var2.floatValue >= var1.floatValue});

                ip++;
            }
            break;
            case fCmpLt:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Float || var2.type != Float)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("fCmpLt %f %f", var2.floatValue, var1.floatValue);
                file.pushToStack({Float, var2.floatValue < var1.floatValue});

                ip++;
            }
            break;
            case fCmpLe:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Float || var2.type != Float)
                {
                    assert("");
                }

                printf("fCmpLe %f %f", var2.floatValue, var1.floatValue);
                file.pushToStack({Float, var2.floatValue <= var1.floatValue});

                ip++;
            }
            break;
            case vAdd:
            {
                glm::vec3 var1;

                for (int i = 0; i < 3; i++)
                {
                    var1[i] = file.popFromStack().floatValue;
                }
                glm::vec3 var2;
                for (int i = 0; i < 3; i++)
                {
                    var2[i] = file.popFromStack().floatValue;
                }

                printf("vAdd %s %s", glm::to_string(var2).c_str(), glm::to_string(var1).c_str());

                glm::vec3 result = var2 + var1;

                for (int i = 0; i < 3; i++)
                {
                    file.pushToStack({Float, result[i]});
                }

                ip++;
            }
            break;
            case vSub:
            {
                glm::vec3 var1;

                for (int i = 0; i < 3; i++)
                {
                    var1[i] = file.popFromStack().floatValue;
                }
                glm::vec3 var2;
                for (int i = 0; i < 3; i++)
                {
                    var2[i] = file.popFromStack().floatValue;
                }

                printf("vSub %s %s", glm::to_string(var2).c_str(), glm::to_string(var1).c_str());

                glm::vec3 result = var2 - var1;

                for (int i = 0; i < 3; i++)
                {
                    file.pushToStack({Float, result[i]});
                }

                ip++;
            }
            break;
            case vMult:
            {
                glm::vec3 var1;

                for (int i = 0; i < 3; i++)
                {
                    var1[i] = file.popFromStack().floatValue;
                }
                glm::vec3 var2;
                for (int i = 0; i < 3; i++)
                {
                    var2[i] = file.popFromStack().floatValue;
                }

                printf("vMult %s %s", glm::to_string(var2).c_str(), glm::to_string(var1).c_str());

                glm::vec3 result = var2 * var1;

                for (int i = 0; i < 3; i++)
                {
                    file.pushToStack({Float, result[i]});
                }

                ip++;
            }
            break;
            case vDiv:
            {
                glm::vec3 var1;

                for (int i = 0; i < 3; i++)
                {
                    var1[i] = file.popFromStack().floatValue;
                }
                glm::vec3 var2;
                for (int i = 0; i < 3; i++)
                {
                    var2[i] = file.popFromStack().floatValue;
                }

                printf("vDiv %s %s", glm::to_string(var2).c_str(), glm::to_string(var1).c_str());

                glm::vec3 result = var2 / var1;

                for (int i = 0; i < 3; i++)
                {
                    file.pushToStack({Float, result[i]});
                }

                ip++;
            }
            break;
            case vNeg:
            {
                glm::vec3 var1;

                for (int i = 0; i < 3; i++)
                {
                    var1[i] = file.popFromStack().floatValue;
                }

                printf("vNeg %s", glm::to_string(var1).c_str());

                glm::vec3 result = -var1;

                for (int i = 0; i < 3; i++)
                {
                    file.pushToStack({Float, result[i]});
                }

                ip++;
            }
            break;
            case And:
            {
                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Int32 || var2.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("And %d %d", var1.integer, var2.integer);

                file.pushToStack({Int32, var1.integer && var2.integer});
                ip++;
            }
            break;
            case Or:
                ip++;
                break;
            case Xor:
                ip++;
                break;
            case ItoF:
                ip++;
                break;
            case FtoI:
                ip++;
                break;
            case FtoV:  //float to Vector
            {
                ScriptParameter var = file.popFromStack();

                if (var.type != Float)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("FtoV %f", var.floatValue);

                for (int i = 0; i < 3; i++)
                {
                    file.pushToStack({Float, var.floatValue});
                }
                ip++;
            }
            break;
            case iPushByte1:
            {
                int32_t var = file.Code[ip + 1];
                printf("iPushByte1 %d", var);
                file.pushToStack({Int32, var});
                ip += 2;
            }

            break;
            case iPushByte2:
                ip += 3;
                break;
            case iPushByte3:
                ip += 4;
                break;
            case iPushInt:
            {
                //HEX VALUE
                int32_t var = (file.Code[ip + 4] << 24) | (file.Code[ip + 3] << 16) | (file.Code[ip + 2] << 8) | file.Code[ip + 1];
                printf("iPushInt %d", var);
                file.pushToStack({Int32, var});
                ip += 5;
            }

            break;
            case fPush:
            {
                float var;
                memcpy(&var, &file.Code[ip + 1], 4);
                printf("fPush %f", var);
                file.pushToStack({Float, var});
                ip += 5;
            }
            break;
            case dup:  //duplicate?
            {
                printf("DUP");
                ScriptParameter var = file.getTopValueFromUnmodifiedStack();
                file.pushToStack(var);
                ip++;
            }
            break;
            case pop:
            {
                ScriptParameter var = file.popFromStack();
                printf("POP %d", var.integer);
                ip++;
            }

            break;
            case Native:
            {
                uint16_t offset = file.Code[ip + 3] << 8 | file.Code[ip + 2];
                endSwap(&offset);
                printf("Native %#08llx ", file.natives[offset]);

                auto it = functions.find(file.natives[offset]);
                if (it != functions.end())
                {
                    printf("FUNCTION HAS %d arguments ", it->second.numArguments);

                    ScriptParams params;

                    for (int i = 0; i < it->second.numArguments; i++)
                    {
                        params.push_back(file.popFromStack());
                    }

                    ScriptArguments args(&params, &file, this);

                    it->second.function(args);
                }
                else
                {
                    printf("FUNCTION IS NOT IMPLEMENTED");
                }

                ip += 4;
            }
            break;
            case Enter:
            {
                printf("FUNCTION ENTERED");

                int parametersCount = file.Code[ip + 1];

                int tmp1 = file.Code[ip + 2], tmp2 = file.Code[ip + 3];
                int vcount = (tmp2 << 0x8) | tmp1;
                int NameFunctionLen = file.Code[ip + 4];
                //file.locals.resize(vcount);

                for (int i = 0; i < vcount; i++)
                {
                    file.locals.push_back({Int32, 0});
                }

                for (int i = 0; i < parametersCount; i++)
                {
                    int32_t var = file.popFromStack().integer;
                    file.locals.insert(file.locals.begin(), ScriptParameter(Int32, 0));
                }

                ip += 5;
            }
            break;
            case Return:
            {
                int test = file.Code[ip + 2];
                int numberOfArgumentsToPop = file.Code[ip + 1];
                //assert(numberOfArgumentsToPop > 0);
                if (file.calls.empty())
                {
                    file.finished = true;
                    file.paused = true;
                }
                else
                {
                    int32_t popped = file.calls.top();
                    file.calls.pop();
                    ip = popped;
                }
                switch (test)
                {
                    case 0:
                        printf("default return");
                        break;
                    case 1:
                        printf("default something");
                        break;
                    default:
                        break;
                }
                printf("RETURN");
                //ip += 3;
            }
            break;
            case pGet:
                printf("UNIMPLEMENTED pGet");
                ip++;
                break;
            case pSet:
                printf("UNIMPLEMENTED pSet");
                ip++;
                break;
            case pPeekSet:
                printf("UNIMPLEMENTED pPeekSet");
                ip++;
                break;
            case ToStack:
                printf("UNIMPLEMENTED ToStack");
                ip++;
                break;
            case FromStack:
            {
                ScriptParameter pointer = file.popFromStack();
                ScriptParameter count = file.popFromStack();

                for (int i = 0; i < count.integer; i++)
                {
                    ScriptParameter var = file.popFromStack();
                }

                printf("FromStack %d %d", pointer.integer, count.integer);
                ip++;
            }
            break;
            case pArray1:
            {
                int32_t imm = file.Code[ip + 1];

                ScriptParameter arrayLoc = file.popFromStack();
                ScriptParameter index = file.popFromStack();

                file.pushToStack({Int32, arrayLoc.integer + index.integer + imm});

                printf("pArray1 %d %d %d", arrayLoc.integer, index.integer, imm);
                ip += 2;
            }
            break;
            case ArrayGet1:
                printf("UNIMPLEMENTED ArrayGet1");
                ip += 2;
                break;
            case ArraySet1:
            {
                uint32_t var = file.Code[ip + 1];
                ScriptParameter arrayLoc = file.popFromStack();
                ScriptParameter index = file.popFromStack();
                ScriptParameter value = file.popFromStack();

                printf("ArraySet1 %d %d %d", arrayLoc.integer, index.integer + var, value.integer);
                ip += 2;
            }
            break;
            case pFrame1:
            {
                int32_t var = file.Code[ip + 1];
                printf("pFrame1 %d", var);
                file.pushToStack(file.locals[var]);
                ip += 2;
            }
            break;
            case GetFrame1:
            {
                int32_t var = file.Code[ip + 1];
                printf("GetFrame1 %d", var);
                file.pushToStack(file.locals[var]);
                ip += 2;
            }
            break;
            case SetFrame1:
            {
                int32_t var = file.Code[ip + 1];
                printf("SetFrame1 %d", var);
                file.locals[var] = file.popFromStack();
                ip += 2;
            }
            break;
            case pStatic1:
            {
                int32_t var = file.Code[ip + 1];
                printf("pStatic1 %d POINTER NEED", var);
                file.pushToStack({Int32, var});
                ip += 2;
            }
            break;
            case StaticGet1:
                ip += 2;
                break;
            case StaticSet1:
            {
                int32_t var1 = file.Code[ip + 1];
                ScriptParameter var2 = file.popFromStack();
                file.Statics[var1] = var2;
                printf("StaticSet1 %d %s", var1, var2.GetVariable().c_str());
                ip += 2;
            }
            break;
            case Add1:
            {
                int32_t var1 = file.Code[ip + 1];
                ScriptParameter var2 = file.popFromStack();

                if (var2.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("Add1 %d %s", var1, var2.GetVariable().c_str());
                file.pushToStack({Int32, var1 + var2.integer});
                ip += 2;
            }
            break;
            case Mult1:
                ip += 2;
                break;
            case pStructStack:
                ip += 2;
                break;
            case pStruct1:
            {
                int32_t var = file.Code[ip + 1];
                printf("pStruct1 %d", var);
                //push
                ip += 2;
            }
            break;
            case GetStruct1:
                printf("UNIMPLEMENTED GetStruct1");
                ip += 2;
                break;
            case SetStruct1:
                printf("UNIMPLEMENTED SetStruct1");
                ip += 2;
                break;
            case iPushShort:
            {
                int16_t var = (file.Code[ip + 2] << 8) | file.Code[ip + 1];
                file.pushToStack({Int32, var});
                printf("iPushShort %d", var);
                ip += 3;
            }

            break;
            case Add2:
            {
                ScriptParameter var = file.popFromStack();

                if (var.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                int16_t imm = (file.Code[ip + 2] << 8) | file.Code[ip + 1];
                file.pushToStack({Int32, imm + var.integer});

                printf("Add2 %d %s", imm, var.GetVariable().c_str());
                ip += 3;
            }
            break;
            case Mult2:
                ip += 3;
                break;
            case pStruct2:
                ip += 3;
                break;
            case GetStruct2:
                ip += 3;
                break;
            case SetStruct2:
                ip += 3;
                break;
            case pArray2:
                ip += 3;
                break;
            case ArrayGet2:
                ip += 3;
                break;
            case ArraySet2:
                ip += 3;
                break;
            case pFrame2:
                ip += 3;
                break;
            case GetFrame2:
                ip += 3;
                break;
            case SetFrame2:
                ip += 3;
                break;
            case pStatic2:
                printf("UNIMPLEMENTED pStatic2");
                ip += 3;
                break;
            case StaticGet2:
                printf("UNIMPLEMENTED StaticGet2");
                ip += 3;
                break;
            case StaticSet2:
            {
                int32_t var1 = (file.Code[ip + 2] << 8) | file.Code[ip + 1];
                ScriptParameter var2 = file.popFromStack();
                file.Statics[var1] = var2;
                printf("StaticSet2 %d %s", var1, var2.GetVariable().c_str());
                ip += 3;
            }
            break;
            case pGlobal2:
                printf("UNIMPLEMENTED pGlobal2");
                ip += 3;
                break;
            case GlobalGet2:
                printf("UNIMPLEMENTED GlobalGet2");
                ip += 3;
                break;
            case GlobalSet2:
                printf("UNIMPLEMENTED GlobalSet2");
                ip += 3;
                break;
            case Jump:
            {
                int32_t offset = (int16_t)((file.Code[ip + 2] << 8) | file.Code[ip + 1]) + ip + 3;
                printf("Jump %d", offset);
                ip = offset;
            }
            break;
            case JumpFalse:
            {
                int16_t offset = (file.Code[ip + 2] << 8) | file.Code[ip + 1];

                ScriptParameter condition = file.popFromStack();

                printf("JumpFalse COND %d OFFSET %d ", condition.integer, offset);
                if (!condition.integer)  //0
                {
                    ip = ip + 3 + offset;
                    printf("IS EXECUTED %d", ip);
                }
                else  // 1
                {
                    ip += 3;
                }
            }
            break;
            case JumpNe:
                ip += 3;
                break;
            case JumpEq:
                ip += 3;
                break;
            case JumpLe:
                ip += 3;
                break;
            case JumpLt:
                ip += 3;
                break;
            case JumpGe:
            {
                int16_t offset = (file.Code[ip + 2] << 8) | file.Code[ip + 1];

                ScriptParameter var1 = file.popFromStack();
                ScriptParameter var2 = file.popFromStack();

                if (var1.type != Int32 || var2.type != Int32)
                {
                    assert("DIFFERENT TYPE");
                }

                printf("JumpGe %d %d OFFSET %d ", var1.integer, var2.integer, offset);
                if (var2.integer < var1.integer)
                {
                    ip = ip + 3 + offset;
                    printf("IS EXECUTED %d", ip);
                }
                else  // 1
                {
                    ip += 3;
                }
            }
            break;
            case JumpGt:
                ip += 3;
                break;
            case Call:
            {
                file.calls.push(ip + 4);  //PUSH RETURN IP VALUE
                uint32_t offset = file.Code[ip + 3] << 16 | file.Code[ip + 2] << 8 | file.Code[ip + 1];
                printf("Call %d", offset);
                ip = offset;
            }
            break;
            case pGlobal3:
            {
                int32_t var = file.Code[ip + 3] << 16 | file.Code[ip + 2] << 8 | file.Code[ip + 1];
                file.pushToStack({Int32, var});
                printf("pGlobal3 %d", var);
                ip += 4;
            }
            break;
            case GlobalGet3:
            {
                printf("GlobalGet3 UNIMPLEMENTED");
                ip += 4;
            }
            break;
            case GlobalSet3:
            {
                printf("GlobalSet3 UNIMPLEMENTED");
                ip += 4;
            }
            break;
            case iPushI24:
            {
                int32_t val = file.Code[ip + 3] << 16 | file.Code[ip + 2] << 8 | file.Code[ip + 1];
                printf("iPushI24 %d", val);
                file.pushToStack({Int32, val});
                ip += 4;
            }
            break;
            case Switch:
            {
                int32_t var1 = file.Code[ip + 1];
                ip += var1 * 6 + 1;
            }
            break;
            case PushString:
            {
                //TOP STACK CONTAINS INDEX TO String
                printf("PushString %s", (char*)&file.Strings[file.getTopValueFromUnmodifiedStack().integer]);
                ip++;
            }
            break;
            case GetHash:
                break;
            case StrCopy:
                ip += 2;
                break;
            case ItoS:
                ip += 2;
                break;
            case StrConCat:
                ip += 2;
                break;
            case StrConCatInt:
                ip += 2;
                break;
            case MemCopy:
                break;
            case Catch:
                break;
            case Throw:
                break;
            case pCall:
                break;
            case iPush_n1:
            case iPush_0:
            case iPush_1:
            case iPush_2:
            case iPush_3:
            case iPush_4:
            case iPush_5:
            case iPush_6:
            case iPush_7:
            {
                //INSTRUCTION CODE MINUS CONSTANT VALUE 110
                int32_t var = file.Code[ip] - 110;
                file.pushToStack({Int32, var});
                printf("iPush %d", var);
                ip++;
            }
            break;
            case fPush_n1:
            case fPush_0:
            case fPush_1:
            case fPush_2:
            case fPush_3:
            case fPush_4:
            case fPush_5:
            case fPush_6:
            case fPush_7:
            {
                //INSTRUCTION CODE MINUS CONSTANT VALUE 110
                float var = file.Code[ip] - 119;
                file.pushToStack({Float, var});
                printf("fPush %f", var);
                ip++;
            }
            break;
            default:
                printf("ERROR\n");
                break;
        }
    }

    if (file.paused)
        file.paused = false;
}

bool ScriptInterpreter::startThread(uint32_t ScriptHash)
{
    LoadedScripts.insert({ScriptHash, game->getWorld()->getResourceManager()->GetYsc(ScriptHash)});
    return true;
}

void ScriptInterpreter::execute()
{
    for (auto it = LoadedScripts.begin(); it != LoadedScripts.end();)
    {
        if ((it->second)->Loaded)
        {
            executeThread(*it->second);
            if ((it->second)->finished)
            {
                it = LoadedScripts.erase(it);
            }
            else
            {
                ++it;
            }
        }
        else
            ++it;
    }
}
