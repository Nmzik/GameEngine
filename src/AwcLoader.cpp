#include "AwcLoader.h"
#include "GTAEncryption.h"

#include <xaudio2.h>

struct AwcChunkInfo
{
    uint64_t RawVal;
    uint8_t Tag;
    int32_t Size;
    int32_t Offset;
};

struct AwcChannelChunkInfo
{
    uint32_t Unk0;
    uint32_t ChunkSize;
    uint32_t ChannelCount;
};

struct AwcChannelChunkItemInfo
{
    uint32_t Id;
    uint32_t Samples;
    uint16_t Unk0;
    uint16_t SamplesPerSecond;
    uint8_t Unk1;
    uint8_t RoundSize;
    uint16_t Unk2;
};

struct AwcFormatChunk
{
    enum CodecFormat : uint8_t
    {
        PCM = 0,
        ADPCM = 4
    };
    uint32_t Samples;
    int32_t LoopPoint;
    uint16_t SamplesPerSecond;
    int16_t Headroom;
    uint16_t LoopBegin;
    uint16_t LoopEnd;
    uint16_t PlayEnd;
    uint8_t PlayBegin;
    CodecFormat Codec;
};

struct AwcChannelChunkHeader
{
    uint32_t StartChunk;
    uint32_t ChunkCount;
    uint32_t SamplesToSkip;  //mostly 0
    uint32_t SamplesPerChunk;
    uint32_t DataSize;
};

AwcLoader::AwcLoader(memstream& file)
{
    uint32_t* Magic = (uint32_t*)file.read(sizeof(uint32_t));

    if (*Magic != 0x54414441 && *Magic != 0x41444154)  //we need to decrypt
    {
        GTAEncryption::getInstance().Decrypt_RSXXTEA((uint8_t*)file.data, file.BufferSize);
    }

    switch (*Magic)
    {
        case 0x54414441:
            printf("LITTLE ENDIAN\n");
            break;
        case 0x41444154:
            printf("BIG ENDIAN\n");
            break;
        default:
            printf("UNDEFINED\n");
            break;
    }
    uint16_t Version = *(uint16_t*)file.read(sizeof(uint16_t));
    uint16_t Flags = *(uint16_t*)file.read(sizeof(uint16_t));
    int32_t StreamCount = *(int32_t*)file.read(sizeof(int32_t));
    int32_t InfoOffset = *(int32_t*)file.read(sizeof(int32_t));

    auto MultiChannel = ((Flags & 4) == 4);

    auto flag0 = ((Flags & 1) == 1);
    auto infoStart = 16 + (flag0 ? (StreamCount * 2) : 0);

    if (MultiChannel)
    {
        printf("UNSUPPORTED AUDIO\n");
    }
    else
    {
        printf("SUPPORTED AUDIO\n");

        for (int i = 0; i < StreamCount; i++)
        {
        }

        std::vector<uint8_t> Audio(29270);
        memcpy(&Audio[0], &file.data[276800], 29270);

        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        IXAudio2* pXAudio2 = NULL;
        HRESULT hr;
        if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
            printf("SOMETHING WRONG");

        IXAudio2MasteringVoice* pMasterVoice = NULL;
        if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice, 1, 28001, 0, 0, nullptr, AudioCategory_GameEffects)))
            printf("SOMETHING WRONG");

        WAVEFORMATEX waveformStruct{
            WAVE_FORMAT_PCM,
            1,  //channels
            28001,
            56002,
            2,
            16,
            0  //?????
        };

        XAUDIO2_BUFFER buffer{0};
        buffer.AudioBytes = 29270;
        buffer.pAudioData = &Audio[0];
        buffer.Flags = XAUDIO2_END_OF_STREAM;

        IXAudio2SourceVoice* pSourceVoice;
        if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&waveformStruct)))
            printf("SOMETHING WRONG");

        if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
            printf("SOMETHING WRONG");

        if (FAILED(hr = pSourceVoice->Start(0)))
            printf("SOMETHING WRONG");

        printf("DONE");
    }
}

AwcLoader::~AwcLoader()
{
}
