#pragma once

#include "FileType.h"
#include "Mesh.h"

struct ShaderGroup {
	uint32_t VFT;
	uint32_t Unknown_4h; // 0x00000001
	uint64_t TextureDictionaryPointer;
	uint64_t ShadersPointer;
	uint16_t ShadersCount1;
	uint16_t ShadersCount2;
	uint32_t Unknown_1Ch; // 0x00000000
	uint32_t Unknown_20h; // 0x00000000
	uint32_t Unknown_24h; // 0x00000000
	uint32_t Unknown_28h; // 0x00000000
	uint32_t Unknown_2Ch; // 0x00000000
	uint32_t Unknown_30h;
	uint32_t Unknown_34h; // 0x00000000
	uint32_t Unknown_38h; // 0x00000000
	uint32_t Unknown_3Ch; // 0x00000000
};

struct ShaderFX {
	uint64_t ParametersPointer;
	uint32_t Name; //530103687, 2401522793, 1912906641
	uint32_t Unknown_Ch; // 0x00000000
	uint8_t ParameterCount;
	uint8_t Unknown_11h; // 2, 0, 
	uint16_t Unknown_12h; // 32768
	uint32_t Unknown_14h; //10485872, 17826000, 26214720
	uint32_t FileName; //2918136469, 2635608835, 2247429097
	uint32_t Unknown_1Ch; // 0x00000000
	uint32_t Unknown_20h; //65284, 65281
	uint16_t Unknown_24h; //0
	uint8_t Unknown_26h; //0
	uint8_t TextureParametersCount;
	uint32_t Unknown_28h; // 0x00000000
	uint32_t Unknown_2Ch; // 0x00000000
};

struct ShaderParameter
{
	uint8_t DataType;
	uint8_t Unknown_1h;
	uint16_t Unknown_2h;
	uint32_t Unknown_4h;
	uint64_t DataPointer;
};

struct LightAttributes_s
{
	// structure data
	uint32_t Unknown_0h; // 0x00000000
	uint32_t Unknown_4h; // 0x00000000
	float PositionX;
	float PositionY;
	float PositionZ;
	uint32_t Unknown_14h; // 0x00000000
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
	uint32_t Unknown_48h; // 0x00000000
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
	uint32_t Unknown_A4h; // 0x00000000
};

struct Drawable {
	uint64_t NamePointer;
	uint64_t LightAttributesPointer;
	uint16_t LightAttributesCount1;
	uint16_t LightAttributesCount2;
	uint32_t Unknown_BCh; // 0x00000000
	uint32_t Unknown_C0h; // 0x00000000
	uint32_t Unknown_C4h; // 0x00000000
	uint64_t BoundPointer;
};

struct FragDrawable {
	uint32_t Unknown_0A8h;
	uint32_t Unknown_0ACh;
	glm::mat4 Unknown_0B0h;
	uint64_t BoundPointer;
	uint64_t Unknown_0F8h_Pointer;
	uint16_t Count1;
	uint16_t Count2;
	uint32_t Unknown_104h; // 0x00000000
	uint64_t Unknown_108h_Pointer;
	uint16_t Count3;
	uint16_t Count4;
	uint32_t Unknown_114h; // 0x00000000
	uint32_t Unknown_118h; // 0x00000000
	uint32_t Unknown_11Ch; // 0x00000000
	uint32_t Unknown_120h; // 0x00000000
	uint32_t Unknown_124h; // 0x00000000
	uint32_t Unknown_128h; // 0x00000000
	uint32_t Unknown_12Ch; // 0x00000000
	uint64_t NamePointer;
	uint32_t Unknown_138h; // 0x00000000
	uint32_t Unknown_13Ch; // 0x00000000
	uint32_t Unknown_140h; // 0x00000000
	uint32_t Unknown_144h; // 0x00000000
	uint32_t Unknown_148h; // 0x00000000
	uint32_t Unknown_14Ch; // 0x00000000
};

struct DrawableBase : ResourceFileBase {
	uint64_t ShaderGroupPointer;
	uint64_t SkeletonPointer;
	glm::vec3 BoundingCenter;
	float BoundingSphereRadius;
	glm::vec4 BoundingBoxMin;
	glm::vec4 BoundingBoxMax;
	uint64_t DrawableModelsHighPointer;
	uint64_t DrawableModelsMediumPointer;
	uint64_t DrawableModelsLowPointer;
	uint64_t DrawableModelsVeryLowPointer;
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
	uint64_t DrawableModelsXPointer;
};

struct pgPtrRepresentation
{
	uint32_t pointer : 28;
	uint32_t blockType : 4;
};

template <typename T>
class pgPtr {
public:
	union
	{
		uint64_t on_disk;
		T* pointer;
	};

	T* operator->() const
	{
		return (T*)pointer;
	}
};

struct DrawableGeometry {
	uint32_t VFT;
	uint32_t Unknown_4h; // 0x00000001
	uint32_t Unknown_8h; // 0x00000000
	uint32_t Unknown_Ch; // 0x00000000
	uint32_t Unknown_10h; // 0x00000000
	uint32_t Unknown_14h; // 0x00000000
	uint64_t VertexBufferPointer;
	uint32_t Unknown_20h; // 0x00000000
	uint32_t Unknown_24h; // 0x00000000
	uint32_t Unknown_28h; // 0x00000000
	uint32_t Unknown_2Ch; // 0x00000000
	uint32_t Unknown_30h; // 0x00000000
	uint32_t Unknown_34h; // 0x00000000
	uint64_t IndexBufferPointer;
	uint32_t Unknown_40h; // 0x00000000
	uint32_t Unknown_44h; // 0x00000000
	uint32_t Unknown_48h; // 0x00000000
	uint32_t Unknown_4Ch; // 0x00000000
	uint32_t Unknown_50h; // 0x00000000
	uint32_t Unknown_54h; // 0x00000000
	uint32_t IndicesCount;
	uint32_t TrianglesCount;
	uint16_t VerticesCount;
	uint16_t Unknown_62h; // 0x0003
	uint32_t Unknown_64h; // 0x00000000
	uint64_t BoneIdsPointer;
	uint16_t VertexStride;
	uint16_t Count1;
	uint32_t Unknown_74h; // 0x00000000
	uint64_t VertexDataPointer;
	uint32_t Unknown_80h; // 0x00000000
	uint32_t Unknown_84h; // 0x00000000
	uint32_t Unknown_88h; // 0x00000000
	uint32_t Unknown_8Ch; // 0x00000000
	uint32_t Unknown_90h; // 0x00000000
	uint32_t Unknown_94h; // 0x00000000

	void Resolve(memstream2 & file) {
		//VertexBufferPointer = (VertexBuffer*)&file.data[(uint64_t)VertexBufferPointer & ~0x50000000];
		//IndexBufferPointer = (VertexBuffer*)&file.data[(uint64_t)VertexBufferPointer & ~0x50000000];
	}
};

struct DrawableModel {
	uint32_t VFT;
	uint32_t Unknown_4h;
	uint64_t GeometriesPointer;
	uint16_t GeometriesCount1;
	uint16_t GeometriesCount2;
	uint32_t Unknown_14h;
	uint64_t BoundsPointer;
	uint64_t ShaderMappingPointer;
	uint32_t Unknown_28h;
	uint32_t Unknown_2Ch;
};

class YtdLoader;
class YbnLoader;

class YdrLoader : public FileType
{
	YtdLoader* Ytd = nullptr;
	YbnLoader* ybnfile = nullptr;
public:
	YdrLoader * next;
	std::vector<Mesh>* meshes;
	bool isYft = false;

	void Init(memstream2& file, int32_t systemSize) override;
	void Remove();

	void UploadMeshes();
};

static Pool<YdrLoader, 6000> YdrPool;
