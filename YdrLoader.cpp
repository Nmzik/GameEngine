#include "YdrLoader.h"

YdrLoader::YdrLoader(memstream& file, glm::vec3 position, glm::quat rotation, uint32_t hash) : Hash(hash)
{
	ModelMatrix = glm::translate(glm::mat4(), position);
	ModelMatrix *= glm::toMat4(rotation);
	struct {
		uint32_t FileVFT;
		uint32_t FileUnknown;
		uint64_t FilePagesInfoPointer;
	} ResourceFileBase;
	file.read((char*)&ResourceFileBase, sizeof(ResourceFileBase));

	uint64_t SYSTEM_BASE = 0x50000000;
	uint64_t GRAPHICS_BASE = 0x60000000;
	/*uint64_t pos = file.tellg();
	//seek to pointer and then
	if ((ResourceFileBase.FilePagesInfoPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		ResourceFileBase.FilePagesInfoPointer = ResourceFileBase.FilePagesInfoPointer & ~0x50000000;
	}
	if ((ResourceFileBase.FilePagesInfoPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		ResourceFileBase.FilePagesInfoPointer = ResourceFileBase.FilePagesInfoPointer & ~0x60000000;
	}
	printf("%d\n", ResourceFileBase.FilePagesInfoPointer);
	file.seekg(ResourceFileBase.FilePagesInfoPointer);

	uint32_t Unknown_0h;
	uint32_t Unknown_4h;
	uint8_t SystemPagesCount;
	uint8_t GraphicsPagesCount;
	uint16_t Unknown_Ah;
	uint32_t Unknown_Ch;
	uint32_t Unknown_10h;

	file.read((char*)&Unknown_0h, sizeof(Unknown_0h));
	file.read((char*)&Unknown_4h, sizeof(Unknown_4h));
	file.read((char*)&SystemPagesCount, sizeof(SystemPagesCount));
	file.read((char*)&GraphicsPagesCount, sizeof(GraphicsPagesCount));
	file.read((char*)&Unknown_Ah, sizeof(Unknown_Ah));
	file.read((char*)&Unknown_Ch, sizeof(Unknown_Ch));
	file.read((char*)&Unknown_10h, sizeof(Unknown_10h));

	file.seekg(pos);*/

	struct {
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
	} DrawableBase;

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
	};

	//std::vector<VertexBuffer> vertexBuffer;
	//std::vector<IndexBuffer> indexBuffer;

	struct {
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
	} DrawableModel;

	std::vector<DrawableGeometry> Geometries;

	file.read((char*)&DrawableBase, sizeof(DrawableBase));

	uint64_t pos1 = file.tellg();

	if ((DrawableBase.DrawableModelsXPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		DrawableBase.DrawableModelsXPointer = DrawableBase.DrawableModelsXPointer & ~0x50000000;
	}
	if ((DrawableBase.DrawableModelsXPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		DrawableBase.DrawableModelsXPointer = DrawableBase.DrawableModelsXPointer & ~0x60000000;
	}

	file.seekg(DrawableBase.DrawableModelsXPointer);

	struct {
		uint64_t EntriesPointer;
		uint16_t EntriesCount;
		uint16_t EntriesCapacity;
	} ResourcePointerList64;

	file.read((char*)&ResourcePointerList64, sizeof(ResourcePointerList64));

	if ((ResourcePointerList64.EntriesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		ResourcePointerList64.EntriesPointer = ResourcePointerList64.EntriesPointer & ~0x50000000;
	}
	if ((ResourcePointerList64.EntriesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		ResourcePointerList64.EntriesPointer = ResourcePointerList64.EntriesPointer & ~0x60000000;
	}

	file.seekg(ResourcePointerList64.EntriesPointer);

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

		file.read((char*)&DrawableModel, sizeof(DrawableModel));

		if ((DrawableModel.GeometriesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			DrawableModel.GeometriesPointer = DrawableModel.GeometriesPointer & ~0x50000000;
		}
		if ((DrawableModel.GeometriesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			DrawableModel.GeometriesPointer = DrawableModel.GeometriesPointer & ~0x60000000;
		}

		file.seekg(DrawableModel.GeometriesPointer);

		for (int i = 0; i < DrawableModel.GeometriesCount1; i++) //no difference btween geometriescount1 and 2
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

			DrawableGeometry drawGeom;

			file.read((char*)&drawGeom, sizeof(DrawableGeometry));

			if ((drawGeom.VertexBufferPointer & SYSTEM_BASE) == SYSTEM_BASE) {
				drawGeom.VertexBufferPointer = drawGeom.VertexBufferPointer & ~0x50000000;
			}
			if ((drawGeom.VertexBufferPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
				drawGeom.VertexBufferPointer = drawGeom.VertexBufferPointer & ~0x60000000;
			}

			file.seekg(drawGeom.VertexBufferPointer);

			VertexBuffer vertbuffer;
			file.read((char*)&vertbuffer, sizeof(VertexBuffer) - 24);

			if ((vertbuffer.DataPointer1 & SYSTEM_BASE) == SYSTEM_BASE) {
				vertbuffer.DataPointer1 = vertbuffer.DataPointer1 & ~0x50000000;
			}
			if ((vertbuffer.DataPointer1 & GRAPHICS_BASE) == GRAPHICS_BASE) {
				vertbuffer.DataPointer1 = vertbuffer.DataPointer1 & ~0x60000000;
			}

			/*struct Meshdata {
			glm::vec3 vertices;
			glm::vec3 normals;
			uint8_t textcoord1[4];
			glm::vec2 idk;
			glm::vec3 idk2;
			float idk3;
			};*/

			file.seekg(vertbuffer.DataPointer1);

			vertbuffer.VertexData.resize(vertbuffer.VertexCount * vertbuffer.VertexStride);
			file.read((char*)&vertbuffer.VertexData[0], vertbuffer.VertexCount * vertbuffer.VertexStride);

			if ((drawGeom.IndexBufferPointer & SYSTEM_BASE) == SYSTEM_BASE) {
				drawGeom.IndexBufferPointer = drawGeom.IndexBufferPointer & ~0x50000000;
			}
			if ((drawGeom.IndexBufferPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
				drawGeom.IndexBufferPointer = drawGeom.IndexBufferPointer & ~0x60000000;
			}

			file.seekg(drawGeom.IndexBufferPointer);

			IndexBuffer indexbuffer;
			file.read((char*)&indexbuffer, sizeof(IndexBuffer) - 24);

			//drawGeom->indexBuffer->Indices = new uint16_t[drawGeom->indexBuffer->IndicesCount];

			//INDICES READING
			if ((indexbuffer.IndicesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
				indexbuffer.IndicesPointer = indexbuffer.IndicesPointer & ~0x50000000;
			}
			if ((indexbuffer.IndicesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
				indexbuffer.IndicesPointer = indexbuffer.IndicesPointer & ~0x60000000;
			}

			file.seekg(indexbuffer.IndicesPointer);

			indexbuffer.Indices.resize(indexbuffer.IndicesCount * sizeof(uint16_t));
			file.read((char*)&indexbuffer.Indices[0], sizeof(uint16_t) * indexbuffer.IndicesCount);

			printf("%d\n",sizeof(Mesh));
			meshes.emplace_back(new Mesh(vertbuffer.VertexData, indexbuffer.Indices, vertbuffer.VertexStride));

			//Geometries.push_back(drawGeom);
			//vertexBuffer.push_back(vertbuffer);
			//indexBuffer.push_back(indexbuffer);

			file.seekg(pos);
		}
		file.seekg(posOriginal);
	}

	//file.close();
	/*for (int i = 0; i < Geometries.size(); i++)
	{
		sizeVertex  += Geometries[i].vertexBuffer->VertexData.size() * sizeof(uint8_t);
		sizeIndex += Geometries[i].indexBuffer->Indices.size() * sizeof(uint16_t);
	}*/

	//printf("SIZE %d\n", (sizeVertex + sizeIndex)/1024/1024);

	/*for (int i = 0; i < Geometries.size(); i++)
	{
		//Mesh mesh(Geometries[i].vertexBuffer->VertexData, Geometries[i].indexBuffer->Indices, Geometries[i].vertexBuffer->VertexStride);
		meshes.emplace_back(Geometries[i].vertexBuffer->VertexData, Geometries[i].indexBuffer->Indices, Geometries[i].vertexBuffer->VertexStride);
	}*/

	/*for (int i = 0; i < vertexBuffer.size(); i++)
	{
		Mesh mesh(vertexBuffer[i].VertexData, indexBuffer[i].Indices, vertexBuffer[i].VertexStride);
		meshes.push_back(mesh); //CAREFULL
	}*/
}


YdrLoader::~YdrLoader()
{
	for (auto& mesh : meshes)
	{
		delete mesh;
	}
}

void YdrLoader::UploadMeshes()
{
	for (auto& mesh : meshes)
	{
		//mesh->Upload();
	}
	Loaded = true;
}

glm::mat4& YdrLoader::GetMat4()
{
	return ModelMatrix;
	//glm::mat4 model(1.0);
	//glm::mat4 model = glm::translate(glm::mat4(), Position);
	//model *= glm::toMat4(Rotation);
	/*glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	rotation = glm::conjugate(rotation);

	glm::decompose(model, scale, rotation, translation, skew, perspective);

	printf("RETURNED %s ORIGINAL %s\n", glm::to_string(rotation).c_str(), glm::to_string(Rotation).c_str());*/
	//model = glm::rotate(model, glm::degrees(x), glm::vec3(1.0, 0.0, 0.0f));
	//model = glm::scale(model, glm::vec3(1.0f));
	//glm::quat test1 = glm::quat_cast(model);
	//printf("%f %f %f %f\n",test1.x,test1.y,test1.z,test1,w);
	//return model;
}

void YdrLoader::Draw()
{
	for (auto& mesh : meshes) {
		mesh->Draw();
	}
}
