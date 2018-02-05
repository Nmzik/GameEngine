#include "YddLoader.h"



YddLoader::YddLoader()
{
	std::ifstream file("C:\\Users\\nmzik\\Desktop\\g_m_y_ballasout_01.ydd", std::ios::binary);

	if (!file.is_open()) {
		printf("NOT FOUND!");
	}

	struct {
		uint32_t FileVFT;
		uint32_t FileUnknown;
		uint64_t FilePagesInfoPointer;
	} ResourceFileBase;
	file.read((char*)&ResourceFileBase, sizeof(ResourceFileBase));

	uint64_t SYSTEM_BASE = 0x50000000;
	uint64_t GRAPHICS_BASE = 0x60000000;

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
		std::vector<uint8_t> VertexData;
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
		std::vector<uint16_t> Indices;
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
		VertexBuffer* vertexBuffer;
		IndexBuffer* indexBuffer;
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
		std::vector<DrawableGeometry*> Geometries;
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
		std::vector<DrawableModel> DrawableModels;
	};

	struct {
		uint32_t Unknown_10h; // 0x00000000
		uint32_t Unknown_14h; // 0x00000000
		uint32_t Unknown_18h; // 0x00000001
		uint32_t Unknown_1Ch; // 0x00000000
		uint64_t HashesPointer;
		uint16_t HashesCount1;
		uint16_t HashesCount2;
		uint32_t Unknown_2Ch; // 0x00000000
		uint64_t DrawablesPointer;
		uint16_t DrawablesCount1;
		uint16_t DrawablesCount2;
		uint32_t Unknown_3Ch; // 0x00000000
		std::vector<DrawableBase> DrawableBases;
	} DrawableDictionary;

	file.read((char*)&DrawableDictionary, sizeof(DrawableDictionary) - 24);

	if ((DrawableDictionary.DrawablesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		DrawableDictionary.DrawablesPointer = DrawableDictionary.DrawablesPointer & ~0x50000000;
	}
	if ((DrawableDictionary.DrawablesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		DrawableDictionary.DrawablesPointer = DrawableDictionary.DrawablesPointer & ~0x60000000;
	}

	file.seekg(DrawableDictionary.DrawablesPointer);

	for (int i = 0; i < DrawableDictionary.DrawablesCount1; i++)
	{
		uint64_t DataPointer;
		file.read((char*)&DataPointer, sizeof(uint64_t));

		uint64_t DrawablePointer = file.tellg();

		if ((DataPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			DataPointer = DataPointer & ~0x50000000;
		}
		if ((DataPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			DataPointer = DataPointer & ~0x60000000;
		}

		file.seekg(DataPointer);

		struct {
			uint32_t FileVFT;
			uint32_t FileUnknown;
			uint64_t FilePagesInfoPointer;
		} ResourceFileBase;
		file.read((char*)&ResourceFileBase, sizeof(ResourceFileBase));

		DrawableBase DrawBase;
		file.read((char*)&DrawBase, sizeof(DrawableBase) - 24);

		uint64_t pos1 = file.tellg();

		//SKIP SOME STUFF - NEED FIX POINTER STUFF ...
		//DrawableBase.DrawableModelsXPointer = 1342305584;
		if ((DrawBase.DrawableModelsXPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			DrawBase.DrawableModelsXPointer = DrawBase.DrawableModelsXPointer & ~0x50000000;
		}
		if ((DrawBase.DrawableModelsXPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			DrawBase.DrawableModelsXPointer = DrawBase.DrawableModelsXPointer & ~0x60000000;
		}

		file.seekg(DrawBase.DrawableModelsXPointer);

		struct {
			uint64_t EntriesPointer;
			uint16_t EntriesCount;
			uint16_t EntriesCapacity;
		} ResourcePointerList64;

		file.read((char*)&ResourcePointerList64, sizeof(ResourcePointerList64));

		//uint32_t garbage_value; //fix
		//file.read((char*)&garbage_value, sizeof(uint32_t));

		for (int i = 0; i < ResourcePointerList64.EntriesCount; i++)
		{
			uint64_t data_pointer;
			file.read((char*)&data_pointer, sizeof(data_pointer));
			uint64_t posOriginal = file.tellg();

			if ((data_pointer & SYSTEM_BASE) == SYSTEM_BASE) {
				data_pointer = data_pointer & ~0x50000000;
			}
			if ((data_pointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
				data_pointer = data_pointer & ~0x60000000;
			}

			file.seekg(data_pointer);

			DrawableModel DrawModel;
			file.read((char*)&DrawModel, sizeof(DrawableModel) - 24);

			//POINTER GO
			uint64_t pos2 = file.tellg();

			if ((DrawModel.GeometriesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
				DrawModel.GeometriesPointer = DrawModel.GeometriesPointer & ~0x50000000;
			}
			if ((DrawModel.GeometriesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
				DrawModel.GeometriesPointer = DrawModel.GeometriesPointer & ~0x60000000;
			}

			file.seekg(DrawModel.GeometriesPointer);

			for (int i = 0; i < DrawModel.GeometriesCount1; i++) //no difference btween geometriescount1 and 2
			{
				uint64_t data_pointer;
				file.read((char*)&data_pointer, sizeof(data_pointer));
				uint64_t pos = file.tellg();

				if ((data_pointer & SYSTEM_BASE) == SYSTEM_BASE) {
					data_pointer = data_pointer & ~0x50000000;
				}
				if ((data_pointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
					data_pointer = data_pointer & ~0x60000000;
				}

				file.seekg(data_pointer);

				DrawableGeometry* drawGeom = new DrawableGeometry();

				file.read((char*)drawGeom, sizeof(DrawableGeometry) - 16);

				if ((drawGeom->VertexBufferPointer & SYSTEM_BASE) == SYSTEM_BASE) {
					drawGeom->VertexBufferPointer = drawGeom->VertexBufferPointer & ~0x50000000;
				}
				if ((drawGeom->VertexBufferPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
					drawGeom->VertexBufferPointer = drawGeom->VertexBufferPointer & ~0x60000000;
				}

				file.seekg(drawGeom->VertexBufferPointer);

				drawGeom->vertexBuffer = new VertexBuffer();
				file.read((char*)drawGeom->vertexBuffer, sizeof(VertexBuffer) - 24);

				if ((drawGeom->vertexBuffer->DataPointer1 & SYSTEM_BASE) == SYSTEM_BASE) {
					drawGeom->vertexBuffer->DataPointer1 = drawGeom->vertexBuffer->DataPointer1 & ~0x50000000;
				}
				if ((drawGeom->vertexBuffer->DataPointer1 & GRAPHICS_BASE) == GRAPHICS_BASE) {
					drawGeom->vertexBuffer->DataPointer1 = drawGeom->vertexBuffer->DataPointer1 & ~0x60000000;
				}

				/*struct Meshdata {
				glm::vec3 vertices;
				glm::vec3 normals;
				uint8_t textcoord1[4];
				glm::vec2 idk;
				glm::vec3 idk2;
				float idk3;
				};*/

				file.seekg(drawGeom->vertexBuffer->DataPointer1);

				drawGeom->vertexBuffer->VertexData.resize(drawGeom->vertexBuffer->VertexCount * drawGeom->vertexBuffer->VertexStride);
				file.read((char*)&drawGeom->vertexBuffer->VertexData[0], drawGeom->vertexBuffer->VertexCount * drawGeom->vertexBuffer->VertexStride);

				if ((drawGeom->IndexBufferPointer & SYSTEM_BASE) == SYSTEM_BASE) {
					drawGeom->IndexBufferPointer = drawGeom->IndexBufferPointer & ~0x50000000;
				}
				if ((drawGeom->IndexBufferPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
					drawGeom->IndexBufferPointer = drawGeom->IndexBufferPointer & ~0x60000000;
				}

				file.seekg(drawGeom->IndexBufferPointer);

				drawGeom->indexBuffer = new IndexBuffer();
				file.read((char*)drawGeom->indexBuffer, sizeof(IndexBuffer) - 24);

				//drawGeom->indexBuffer->Indices = new uint16_t[drawGeom->indexBuffer->IndicesCount];

				//INDICES READING
				if ((drawGeom->indexBuffer->IndicesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
					drawGeom->indexBuffer->IndicesPointer = drawGeom->indexBuffer->IndicesPointer & ~0x50000000;
				}
				if ((drawGeom->indexBuffer->IndicesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
					drawGeom->indexBuffer->IndicesPointer = drawGeom->indexBuffer->IndicesPointer & ~0x60000000;
				}

				file.seekg(drawGeom->indexBuffer->IndicesPointer);

				drawGeom->indexBuffer->Indices.resize(drawGeom->indexBuffer->IndicesCount * sizeof(uint16_t));
				file.read((char*)&drawGeom->indexBuffer->Indices[0], sizeof(uint16_t) * drawGeom->indexBuffer->IndicesCount);

				DrawModel.Geometries.push_back(drawGeom);

				file.seekg(pos);
			}
			DrawBase.DrawableModels.push_back(DrawModel);
		}
		DrawableDictionary.DrawableBases.push_back(DrawBase);
		file.seekg(DrawablePointer);
	}

	file.close();

	for (auto& drawBase : DrawableDictionary.DrawableBases)
	{
		for (auto& DrawModel : drawBase.DrawableModels)
		{
			for (auto& Geometry : DrawModel.Geometries)
			{
				Mesh mesh(Geometry->vertexBuffer->VertexData, Geometry->indexBuffer->Indices, Geometry->vertexBuffer->VertexStride);
				meshes.push_back(mesh);
			}
		}
	}
}

glm::mat4 YddLoader::GetMat4()
{
	glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0, 20, 0));
	return model;
}

YddLoader::~YddLoader()
{
}

void YddLoader::Draw()
{
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw();
	}
}