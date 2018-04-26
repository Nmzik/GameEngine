#pragma once

#include "Mesh.h"
#include "fstream"
#include <iostream>
#include <algorithm>
#include "glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/transform.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "membuf.h"
#include "GtaEncryption.h"
#include "YtdLoader.h"
#include "YbnLoader.h"

#define SYSTEM_BASE 0x50000000
#define GRAPHICS_BASE 0x60000000

#define TranslatePTR(NAME)                       \
if ((NAME & SYSTEM_BASE) == SYSTEM_BASE) { \
	NAME = NAME & ~0x50000000; \
}    \
if ((NAME & GRAPHICS_BASE) == GRAPHICS_BASE) { \
	NAME = NAME & ~0x60000000; \
} \

struct ResourcePointerList64 {
	uint64_t EntriesPointer;
	uint16_t EntriesCount;
	uint16_t EntriesCapacity;
};

struct ResourceFileBase {
	uint32_t FileVFT;
	uint32_t FileUnknown;
	uint64_t FilePagesInfoPointer;
};

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

struct DrawableBase {
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

struct VertexBuffer {
	uint32_t VFT;
	uint32_t Unknown_4h; // 0x00000001
	uint16_t VertexStride;
	uint16_t Unknown_Ah;
	uint32_t Unknown_Ch; // 0x00000000
	uint64_t DataPointer1;
	uint32_t VertexCount;
	uint32_t Unknown_1Ch; // 0x00000000
	uint64_t DataPointer2;
	uint32_t Unknown_28h; // 0x00000000
	uint32_t Unknown_2Ch; // 0x00000000
	uint64_t InfoPointer;
	uint32_t Unknown_38h; // 0x00000000
	uint32_t Unknown_3Ch; // 0x00000000
	uint32_t Unknown_40h; // 0x00000000
	uint32_t Unknown_44h; // 0x00000000
	uint32_t Unknown_48h; // 0x00000000
	uint32_t Unknown_4Ch; // 0x00000000
	uint32_t Unknown_50h; // 0x00000000
	uint32_t Unknown_54h; // 0x00000000
	uint32_t Unknown_58h; // 0x00000000
	uint32_t Unknown_5Ch; // 0x00000000
	uint32_t Unknown_60h; // 0x00000000
	uint32_t Unknown_64h; // 0x00000000
	uint32_t Unknown_68h; // 0x00000000
	uint32_t Unknown_6Ch; // 0x00000000
	uint32_t Unknown_70h; // 0x00000000
	uint32_t Unknown_74h; // 0x00000000
	uint32_t Unknown_78h; // 0x00000000
	uint32_t Unknown_7Ch; // 0x00000000
};

struct IndexBuffer {
	uint32_t VFT;
	uint32_t Unknown_4h; // 0x00000001
	uint32_t IndicesCount;
	uint32_t Unknown_Ch; // 0x00000000
	uint64_t IndicesPointer;
	uint32_t Unknown_18h; // 0x00000000
	uint32_t Unknown_1Ch; // 0x00000000
	uint32_t Unknown_20h; // 0x00000000
	uint32_t Unknown_24h; // 0x00000000
	uint32_t Unknown_28h; // 0x00000000
	uint32_t Unknown_2Ch; // 0x00000000
	uint32_t Unknown_30h; // 0x00000000
	uint32_t Unknown_34h; // 0x00000000
	uint32_t Unknown_38h; // 0x00000000
	uint32_t Unknown_3Ch; // 0x00000000
	uint32_t Unknown_40h; // 0x00000000
	uint32_t Unknown_44h; // 0x00000000
	uint32_t Unknown_48h; // 0x00000000
	uint32_t Unknown_4Ch; // 0x00000000
	uint32_t Unknown_50h; // 0x00000000
	uint32_t Unknown_54h; // 0x00000000
	uint32_t Unknown_58h; // 0x00000000
	uint32_t Unknown_5Ch; // 0x00000000
};

struct VertexDeclaration {
	uint32_t Flags;
	uint16_t Stride;
	uint8_t Unknown_6h;
	uint8_t Count;
	uint64_t Types;
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

class YdrLoader
{
	friend class YbnLoader;

	Material* material;
	YtdLoader* Ytd;
	YbnLoader* ybnfile;
	std::vector<Material*> materials;
public:
	std::vector<Mesh> meshes;
	YtdLoader * externalYtd;
	bool isVisible = false;
	uint32_t time;
	bool Loaded = false;
	YdrLoader(memstream& file, int32_t systemSize, btDiscreteDynamicsWorld* world, bool isYft = false);
	~YdrLoader();

	void UploadMeshes();

	void Draw(Shader* shader);
};

