#pragma once
#include "FileType.h"
#include "YbnLoader.h"
#include "YtdLoader.h"

#include "../Model.h"

struct grmShaderParameter
{
    uint8_t DataType;
    uint8_t Unknown_1h;
    uint16_t Unknown_2h;
    uint32_t Unknown_4h;
    uint64_t DataPointer;

    void Resolve(memstream& file)
    {
        //DataPointer.Resolve(file);
    }
};

struct grmShaderFx
{
    pgPtr<grmShaderParameter> parameters;
    uintptr_t shaderHash;
    uint8_t ParameterCount;
    uint8_t drawBucket;  // 2, 0,
    uint8_t m_pad;
    uint8_t m_hasComment;
    uint16_t m_parameterSize;
    uint16_t m_parameterDataSize;
    uintptr_t spsHash;        //	2918136469, 2635608835, 2247429097
    uint32_t drawBucketMask;  //	65284, 65281
    uint8_t m_instanced;
    uint8_t m_pad2[2];
    uint8_t TextureParametersCount;
    uint32_t Unknown_28h;  // 0x00000000
    uint32_t Unknown_2Ch;  // 0x00000000

    void Resolve(memstream& file)
    {
        parameters.Resolve(file);

        for (int i = 0; i < ParameterCount; i++)
        {
            (*parameters)[i].Resolve(file);
        }
    }

    grmShaderParameter* getParameters()
    {
        return *parameters;
    }
};

struct grmShaderGroup : datBase
{
    pgPtr<TextureDictionary> TextureDictionaryPointer;
    pgObjectArray<grmShaderFx> Shaders;
    uint32_t Unknown_1Ch;  // 0x00000000
    uint32_t Unknown_20h;  // 0x00000000
    uint32_t Unknown_24h;  // 0x00000000
    uint32_t Unknown_28h;  // 0x00000000
    uint32_t Unknown_2Ch;  // 0x00000000
    uint32_t Unknown_30h;
    uint32_t Unknown_34h;  // 0x00000000
    uint32_t Unknown_38h;  // 0x00000000
    uint32_t Unknown_3Ch;  // 0x00000000

    void Resolve(memstream& file)
    {
        if (*TextureDictionaryPointer)
        {
            TextureDictionaryPointer.Resolve(file);
            TextureDictionaryPointer->Resolve(file);
        }

        Shaders.Resolve(file);
    }
};

struct rmcDrawableBase : pgBase
{
    pgPtr<grmShaderGroup> ShaderGroupPointer;

    inline void Resolve(memstream& file)
    {
        ShaderGroupPointer.Resolve(file);
        ShaderGroupPointer->Resolve(file);
    };
};

struct VertexDeclaration
{
    uint32_t Flags;
    uint16_t Stride;
    uint8_t Unknown_6h;
    uint8_t Count;
    uint64_t Types;
};

struct grcVertexBuffer : datBase
{
    uint16_t VertexStride;
    uint16_t Unknown_Ah;
    uint32_t Unknown_Ch;  // 0x00000000
    pgPtr<uint8_t> DataPointer1;
    uint32_t VertexCount;
    uint32_t Unknown_1Ch;  // 0x00000000
    uint64_t DataPointer2;
    uint32_t Unknown_28h;  // 0x00000000
    uint32_t Unknown_2Ch;  // 0x00000000
    VertexDeclaration* InfoPointer;
    uint32_t Unknown_38h;  // 0x00000000
    uint32_t Unknown_3Ch;  // 0x00000000
    uint32_t Unknown_40h;  // 0x00000000
    uint32_t Unknown_44h;  // 0x00000000
    uint32_t Unknown_48h;  // 0x00000000
    uint32_t Unknown_4Ch;  // 0x00000000
    uint32_t Unknown_50h;  // 0x00000000
    uint32_t Unknown_54h;  // 0x00000000
    uint32_t Unknown_58h;  // 0x00000000
    uint32_t Unknown_5Ch;  // 0x00000000
    uint32_t Unknown_60h;  // 0x00000000
    uint32_t Unknown_64h;  // 0x00000000
    uint32_t Unknown_68h;  // 0x00000000
    uint32_t Unknown_6Ch;  // 0x00000000
    uint32_t Unknown_70h;  // 0x00000000
    uint32_t Unknown_74h;  // 0x00000000
    uint32_t Unknown_78h;  // 0x00000000
    uint32_t Unknown_7Ch;  // 0x00000000

    void Resolve(memstream& file)
    {
        DataPointer1.Resolve(file);
        InfoPointer = (VertexDeclaration*)&file.data[(uint64_t)InfoPointer & ~0x50000000];
    }
};

struct grcIndexBuffer : datBase
{
    uint32_t IndicesCount;
    uint32_t Unknown_Ch;  // 0x00000000
    pgPtr<uint8_t> IndicesPointer;
    uint32_t Unknown_18h;  // 0x00000000
    uint32_t Unknown_1Ch;  // 0x00000000
    uint32_t Unknown_20h;  // 0x00000000
    uint32_t Unknown_24h;  // 0x00000000
    uint32_t Unknown_28h;  // 0x00000000
    uint32_t Unknown_2Ch;  // 0x00000000
    uint32_t Unknown_30h;  // 0x00000000
    uint32_t Unknown_34h;  // 0x00000000
    uint32_t Unknown_38h;  // 0x00000000
    uint32_t Unknown_3Ch;  // 0x00000000
    uint32_t Unknown_40h;  // 0x00000000
    uint32_t Unknown_44h;  // 0x00000000
    uint32_t Unknown_48h;  // 0x00000000
    uint32_t Unknown_4Ch;  // 0x00000000
    uint32_t Unknown_50h;  // 0x00000000
    uint32_t Unknown_54h;  // 0x00000000
    uint32_t Unknown_58h;  // 0x00000000
    uint32_t Unknown_5Ch;  // 0x00000000

    void Resolve(memstream& file)
    {
        IndicesPointer.Resolve(file);
    }
};

struct grmGeometry : datBase
{
    uint32_t Unknown_8h;   // 0x00000000
    uint32_t Unknown_Ch;   // 0x00000000
    uint32_t Unknown_10h;  // 0x00000000
    uint32_t Unknown_14h;  // 0x00000000
    pgPtr<grcVertexBuffer> VertexBufferPointer;
    uint32_t m_unk1[6];
    pgPtr<grcIndexBuffer> IndexBufferPointer;
    uint32_t m_unk2[6];
    uint32_t IndicesCount;
    uint32_t TrianglesCount;
    uint16_t VerticesCount;
    uint16_t Unknown_62h;  // 0x0003
    uint32_t Unknown_64h;  // 0x00000000
    uint64_t BoneIdsPointer;
    uint16_t VertexStride;
    uint16_t Count1;
    uint32_t Unknown_74h;  // 0x00000000
    uint64_t VertexDataPointer;
    uint32_t m_unk3[6];

    void Resolve(memstream& file)
    {
        VertexBufferPointer.Resolve(file);
        IndexBufferPointer.Resolve(file);

        VertexBufferPointer->Resolve(file);
        IndexBufferPointer->Resolve(file);
    }
};

struct grmModel : datBase
{
    pgObjectArray<grmGeometry> m_geometries;
    uint64_t BoundsPointer;
    pgPtr<uint16_t> ShaderMapping;
    uint32_t Unknown_28h;
    uint32_t Unknown_2Ch;

    void Resolve(memstream& file)
    {
        m_geometries.Resolve(file);

        ShaderMapping.Resolve(file);
    }

    inline uint16_t* getShaderMappings()
    {
        return *ShaderMapping;
    }
};

enum EBoneFlags : uint16_t
{
    None = 0,
    RotX = 0x1,
    RotY = 0x2,
    RotZ = 0x4,
    LimitRotation = 0x8,
    TransX = 0x10,
    TransY = 0x20,
    TransZ = 0x40,
    LimitTranslation = 0x80,
    ScaleX = 0x100,
    ScaleY = 0x200,
    ScaleZ = 0x400,
    LimitScale = 0x800,
    Unk0 = 0x1000,
    Unk1 = 0x2000,
    Unk2 = 0x4000,
    Unk3 = 0x8000,
};

struct SkeletonBoneTag : pgBase
{
    uint32_t BoneTag;
    uint32_t BoneIndex;
    pgPtr<SkeletonBoneTag> LinkedTagPointer;

    inline void Resolve(memstream& file)
    {
        /*if (*LinkedTagPointer)
        {
            LinkedTagPointer.Resolve(file);
            //LinkedTagPointer->Resolve(file);
        }*/
    }
};

struct Bone : pgBase
{
    glm::quat Rotation;
    glm::vec3 Translation;
    uint32_t Unknown_1Ch;  // 0x00000000 RHW?
    glm::vec3 Scale;
    float Unknown_2Ch;
    int16_t NextSiblingIndex;  //limb end index? IK chain?
    int16_t ParentIndex;
    uint32_t Unknown_34h;  // 0x00000000
    uint64_t NamePointer;
    EBoneFlags Flags;
    int16_t Index;
    uint16_t Tag;
    int16_t Index2;        //same as Index?
    uint32_t Unknown_48h;  // 0x00000000
    uint32_t Unknown_4Ch;  // 0x00000000

    inline void Resolve(memstream& file)
    {
    }
};

typedef uint32_t FlagsUint;

struct Skeleton : pgBase
{
    pgObjectArray<SkeletonBoneTag> BoneTagsPointer;
    FlagsUint Unknown_1Ch;
    pgPtr<Bone> BonesPointer;
    pgPtr<glm::mat4> TransformationsInvertedPointer;
    pgPtr<glm::mat4> TransformationsPointer;
    pgPtr<int16_t> ParentIndicesPointer;
    pgPtr<int16_t> ChildIndicesPointer;
    uint32_t Unknown_48h;  // 0x00000000
    uint32_t Unknown_4Ch;  // 0x00000000
    uint32_t Unknown_50h;
    uint32_t Unknown_54h;
    uint32_t Unknown_58h;
    uint16_t Unknown_5Ch;  // 0x0001
    uint16_t BonesCount;
    uint16_t ChildIndicesCount;
    uint16_t Unknown_62h;  // 0x0000
    uint32_t Unknown_64h;  // 0x00000000
    uint32_t Unknown_68h;  // 0x00000000
    uint32_t Unknown_6Ch;  // 0x00000000

    inline void Resolve(memstream& file)
    {
        BoneTagsPointer.Resolve(file);

        BonesPointer.Resolve(file);
        BonesPointer->Resolve(file);

        TransformationsInvertedPointer.Resolve(file);
        TransformationsPointer.Resolve(file);
        ParentIndicesPointer.Resolve(file);
        ChildIndicesPointer.Resolve(file);
    }
};

struct rmcDrawable : rmcDrawableBase
{
    pgPtr<Skeleton> SkeletonPointer;
    glm::vec3 BoundingCenter;
    float BoundingSphereRadius;
    glm::vec4 BoundingBoxMin;
    glm::vec4 BoundingBoxMax;
    pgPtr<pgObjectArray<grmModel>> DrawableModels[4];
    float LodGroupHigh;
    float LodGroupMed;
    float LodGroupLow;
    float LodGroupVlow;
    uint32_t Unknown_80h;
    uint32_t Unknown_84h;
    uint32_t Unknown_88h;
    uint32_t Unknown_8Ch;
    uint64_t JointsPointer;
    uint32_t Unknown_98h;
    uint32_t Unknown_9Ch;
    uint64_t DrawableModelsX;

    inline void Resolve(memstream& file)
    {
        rmcDrawableBase::Resolve(file);

        if (*SkeletonPointer)
        {
            SkeletonPointer.Resolve(file);
            SkeletonPointer->Resolve(file);
        }

        for (int i = 0; i < 4; i++)
        {
            if (*DrawableModels[i])
            {
                DrawableModels[i].Resolve(file);
                DrawableModels[i]->Resolve(file);
            }
        }
    }
};

struct CLightAttr
{
    // structure data
    uint32_t Unknown_0h;  // 0x00000000
    uint32_t Unknown_4h;  // 0x00000000
    float PositionX;
    float PositionY;
    float PositionZ;
    uint32_t Unknown_14h;  // 0x00000000
    uint8_t ColorR;
    uint8_t ColorG;
    uint8_t ColorB;
    uint8_t Flashiness;
    float Intensity;
    uint32_t Flags;
    uint16_t BoneId;
    uint8_t Type;
    uint8_t GroupId;
    uint32_t TimeFlags;
    float Falloff;
    float FalloffExponent;
    float CullingPlaneNormalX;
    float CullingPlaneNormalY;
    float CullingPlaneNormalZ;
    float CullingPlaneOffset;
    uint8_t ShadowBlur;
    uint8_t Unknown_45h;
    uint16_t Unknown_46h;
    uint32_t Unknown_48h;  // 0x00000000
    float VolumeIntensity;
    float VolumeSizeScale;
    uint8_t VolumeOuterColorR;
    uint8_t VolumeOuterColorG;
    uint8_t VolumeOuterColorB;
    uint8_t LightHash;
    float VolumeOuterIntensity;
    float CoronaSize;
    float VolumeOuterExponent;
    uint8_t LightFadeDistance;
    uint8_t ShadowFadeDistance;
    uint8_t SpecularFadeDistance;
    uint8_t VolumetricFadeDistance;
    float ShadowNearClip;
    float CoronaIntensity;
    float CoronaZBias;
    float DirectionX;
    float DirectionY;
    float DirectionZ;
    float TangentX;
    float TangentY;
    float TangentZ;
    float ConeInnerAngle;
    float ConeOuterAngle;
    float ExtentX;
    float ExtentY;
    float ExtentZ;
    uint32_t ProjectedTextureHash;
    uint32_t Unknown_A4h;  // 0x00000000

    inline void Resolve(memstream& file)
    {
    }
};

struct gtaDrawable : rmcDrawable
{
    uint64_t NamePointer;
    pgObjectArray<CLightAttr> LightAttributes;
    //uint32_t Unknown_BCh;  // 0x00000000
    uint32_t Unknown_C0h;  // 0x00000000
    uint32_t Unknown_C4h;  // 0x00000000
    pgPtr<phBound> BoundPointer;

    inline void Resolve(memstream& file)
    {
        rmcDrawable::Resolve(file);

        LightAttributes.Resolve(file);

        if (*BoundPointer)
        {
            BoundPointer.Resolve(file);
        }
    }
};

struct FragDrawable : rmcDrawable
{
    uint32_t Unknown_0A8h;
    uint32_t Unknown_0ACh;
    glm::mat4 Unknown_0B0h;
    uint64_t BoundPointer;
    uint64_t Unknown_0F8h_Pointer;
    uint16_t Count1;
    uint16_t Count2;
    uint32_t Unknown_104h;  // 0x00000000
    uint64_t Unknown_108h_Pointer;
    uint16_t Count3;
    uint16_t Count4;
    uint32_t Unknown_114h;  // 0x00000000
    uint32_t Unknown_118h;  // 0x00000000
    uint32_t Unknown_11Ch;  // 0x00000000
    uint32_t Unknown_120h;  // 0x00000000
    uint32_t Unknown_124h;  // 0x00000000
    uint32_t Unknown_128h;  // 0x00000000
    uint32_t Unknown_12Ch;  // 0x00000000
    uint64_t NamePointer;
    uint32_t Unknown_138h;  // 0x00000000
    uint32_t Unknown_13Ch;  // 0x00000000
    uint32_t Unknown_140h;  // 0x00000000
    uint32_t Unknown_144h;  // 0x00000000
    uint32_t Unknown_148h;  // 0x00000000
    uint32_t Unknown_14Ch;  // 0x00000000

    inline void Resolve(memstream& file)
    {
        rmcDrawable::Resolve(file);

        if (BoundPointer != 0)
        {
            SYSTEM_BASE_PTR(BoundPointer);
        }
    }
};

class YdrLoader : public FileType
{
    YtdLoader* ytd;
    BaseRenderer* renderer = nullptr;
    rmcDrawable* drawable;

public:
    std::vector<Model> models;
    YbnLoader* ybn;

    std::unordered_map<uint16_t, Bone> bonesMap;

    YdrLoader()
        : ytd(nullptr)
        , ybn(nullptr)
    {
    }
    ~YdrLoader();

    void loadDrawable(rmcDrawable* drawable, bool isYft, BaseRenderer* _renderer, memstream& file);
    void init(memstream& file) override;
    void finalize(BaseRenderer* _renderer, memstream& file) override;
};
