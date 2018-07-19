#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <btBulletDynamicsCommon.h>
#include "Mesh.h"
#include "fstream"
#include <iostream>
#include <algorithm>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/norm.hpp"
#include "membuf.h"
#include "GTAEncryption.h"

#define SYSTEM_BASE 0x50000000
#define GRAPHICS_BASE 0x60000000

inline void TranslatePTR(uint64_t& pointer) {
	if ((pointer & SYSTEM_BASE) == SYSTEM_BASE) {
		pointer = pointer & ~0x50000000;
	}
	if ((pointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		pointer = pointer & ~0x60000000;
	}
}

#define SYSTEM_BASE_PTR(NAME) NAME = NAME & ~0x50000000;
#define GRAPHICS_BASE_PTR(NAME) NAME = NAME & ~0x60000000;

struct Array_Structure //16 bytes - pointer for a structure array
{
	uint32_t Pointer;
	uint32_t Unk0;
	uint16_t Count1;
	uint16_t Count2;
	uint32_t Unk1;
};
struct Array_uint //16 bytes - pointer for a uint array
{
	uint32_t Pointer;
	uint32_t Unk0;
	uint16_t Count1;
	uint16_t Count2;
	uint32_t Unk1;
};
struct Array_float //16 bytes - pointer for a float array
{
	uint32_t Pointer;
	uint32_t Unk0;
	uint16_t Count1;
	uint16_t Count2;
	uint32_t Unk1;
};
struct Array_byte //16 bytes - pointer for a byte array
{
	uint32_t Pointer;
	uint32_t Unk0;
	uint16_t Count1;
	uint16_t Count2;
	uint32_t Unk1;
};
struct CharPointer //16 bytes - pointer for a char array
{
	uint32_t Pointer;
	uint32_t Unk0;
	uint16_t Count1;
	uint16_t Count2;
	uint32_t Unk1;
};

struct Array_StructurePointer //16 bytes - pointer for a structure pointer array
{
	uint32_t Pointer;
	uint32_t Unk0;
	uint16_t Count1;
	uint16_t Count2;
	uint32_t Unk1;
};

enum Unk_1264241711 //entity lodLevel
	: int //Key:1856311430
{
	LODTYPES_DEPTH_HD = 0,
	LODTYPES_DEPTH_LOD = 1,
	LODTYPES_DEPTH_SLOD1 = 2, //RARELY USED
	LODTYPES_DEPTH_SLOD2 = 3, //RARELY USED
	LODTYPES_DEPTH_SLOD3 = 4, //thanks Tadden :D //REAL SLOD
	LODTYPES_DEPTH_ORPHANHD = 5, //NOT USED? || ACTUALLY USED FOR INTERIORS
	LODTYPES_DEPTH_SLOD4 = 6, //USED ONLY FOR GRASS
};

enum Unk_648413703 //entity priorityLevel
	: int //Key:2200357711
{
	PRI_REQUIRED = 0,  //1943361227
	PRI_OPTIONAL_HIGH = 1, //3993616791
	PRI_OPTIONAL_MEDIUM = 2, //515598709
	PRI_OPTIONAL_LOW = 3, //329627604
};

struct CEntityDef //128 bytes, Key:1825799514
{
	uint32_t Unused0;//0
	uint32_t Unused1;//4
	uint32_t archetypeName; //8   8: Hash: 0: archetypeName
	uint32_t flags; //12   12: UnsignedInt: 0: flags
	uint32_t guid; //16   16: UnsignedInt: 0: guid
	uint32_t Unused2;//20
	uint32_t Unused3;//24
	uint32_t Unused4;//28
	glm::vec3 position; //32   32: Float_XYZ: 0: position
	float Unused5;//44
	glm::quat rotation; //48   48: Float_XYZW: 0: rotation
	float scaleXY; //64   64: Float: 0: 2627937847
	float scaleZ; //68   68: Float: 0: 284916802
	int32_t parentIndex; //72   72: SignedInt: 0: parentIndex
	float lodDist; //76   76: Float: 0: lodDist
	float childLodDist; //80   80: Float: 0: childLodDist//3398912973
	Unk_1264241711 lodLevel; //84   84: IntEnum: 1264241711: lodLevel  //LODTYPES_DEPTH_
	uint32_t numChildren; //88   88: UnsignedInt: 0: numChildren//2793909385
	Unk_648413703 priorityLevel; //92   92: IntEnum: 648413703: priorityLevel//647098393
	Array_StructurePointer extensions; //96   96: Array: 0: extensions  {0: StructurePointer: 0: 256}
	int32_t ambientOcclusionMultiplier; //112   112: SignedInt: 0: ambientOcclusionMultiplier//415356295
	int32_t artificialAmbientOcclusion; //116   116: SignedInt: 0: artificialAmbientOcclusion//599844163
	uint32_t tintValue; //120   120: UnsignedInt: 0: tintValue//1015358759
	uint32_t Unused6;//124
};

struct ResourceSimpleList64Ptr
{
	// structure data
	uint64_t EntriesPointer;
	uint16_t EntriesCount;
	uint16_t EntriesCapacity;
	uint32_t Unused1;
};

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
class YbnLoader;

class FileType {
	public:
	FileType() {

	}
	~FileType() {

	}

	virtual void Init(memstream2& file, int32_t systemSize, btDiscreteDynamicsWorld* world) {

	}

	virtual void Finalize() {

	}

	virtual void Init(memstream2& file, int32_t systemSize) {

	}

	virtual void Init(memstream2& file) {

	}

	virtual void Finalize(btDiscreteDynamicsWorld* world) {

	}
};

class YdrLoader : public FileType
{
	YtdLoader* Ytd = nullptr;
public:
	YdrLoader * next;
	YbnLoader* ybnfile = nullptr;
	std::vector<Mesh> meshes;
	bool isYft = false;
	bool isVisible = false;
	uint32_t RefCount = 0;
	bool Loaded = false;

	void Init(memstream2& file, int32_t systemSize, btDiscreteDynamicsWorld* world) override;
	void Remove();

	void UploadMeshes();
};

class YdrPool
{
public:
	static YdrPool& getPool() {
		static YdrPool pool;
		return pool;
	}

	uint32_t num;

	YdrPool();
	~YdrPool();

	YdrLoader* Load();
	void Remove(YdrLoader* ymap);

	YdrLoader ydrs[6000];

private:
	YdrLoader * firstAvailable_;
};

