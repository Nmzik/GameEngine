#include "YddLoader.h"



YddLoader::YddLoader(memstream& file, glm::vec3 position, glm::quat rotation, glm::vec3 scale, uint32_t hash) : Hash(hash)
{
	ModelMatrix = glm::translate(glm::mat4(), position) * glm::toMat4(rotation) * glm::scale(glm::mat4(), scale);

	ResourceFileBase resourceFileBase;
	file.read((char*)&resourceFileBase, sizeof(ResourceFileBase));

	uint64_t SYSTEM_BASE = 0x50000000;
	uint64_t GRAPHICS_BASE = 0x60000000;

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
	} DrawableDictionary;

	file.read((char*)&DrawableDictionary, sizeof(DrawableDictionary));

	if ((DrawableDictionary.HashesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		DrawableDictionary.HashesPointer = DrawableDictionary.HashesPointer & ~0x50000000;
	}
	if ((DrawableDictionary.HashesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		DrawableDictionary.HashesPointer = DrawableDictionary.HashesPointer & ~0x60000000;
	}

	file.seekg(DrawableDictionary.HashesPointer);

	Hashes.resize(DrawableDictionary.HashesCount1);

	file.read((char*)&Hashes[0], sizeof(uint32_t) * DrawableDictionary.HashesCount1);

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

		file.read((char*)&resourceFileBase, sizeof(ResourceFileBase));

		DrawableBase DrawBase;
		file.read((char*)&DrawBase, sizeof(DrawableBase));

		uint64_t pos1 = file.tellg();

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

			//LOAD YDR FROM YDD BY HASH //Load specific YDR from YDD through hash
			if (Hashes[i] == Hash) {
				uint64_t posOriginal = file.tellg();

				if ((data_pointer & SYSTEM_BASE) == SYSTEM_BASE) {
					data_pointer = data_pointer & ~0x50000000;
				}
				if ((data_pointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
					data_pointer = data_pointer & ~0x60000000;
				}

				file.seekg(data_pointer);

				DrawableModel DrawModel;
				file.read((char*)&DrawModel, sizeof(DrawableModel));

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
					file.read((char*)&vertbuffer, sizeof(VertexBuffer));

					if ((vertbuffer.DataPointer1 & SYSTEM_BASE) == SYSTEM_BASE) {
						vertbuffer.DataPointer1 = vertbuffer.DataPointer1 & ~0x50000000;
					}
					if ((vertbuffer.DataPointer1 & GRAPHICS_BASE) == GRAPHICS_BASE) {
						vertbuffer.DataPointer1 = vertbuffer.DataPointer1 & ~0x60000000;
					}

					if ((vertbuffer.InfoPointer & SYSTEM_BASE) == SYSTEM_BASE) {
						vertbuffer.InfoPointer = vertbuffer.InfoPointer & ~0x50000000;
					}
					if ((vertbuffer.InfoPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
						vertbuffer.InfoPointer = vertbuffer.InfoPointer & ~0x60000000;
					}

					file.seekg(vertbuffer.InfoPointer);

					VertexDeclaration decl;
					file.read((char*)&decl, sizeof(VertexDeclaration));

					if ((drawGeom.IndexBufferPointer & SYSTEM_BASE) == SYSTEM_BASE) {
						drawGeom.IndexBufferPointer = drawGeom.IndexBufferPointer & ~0x50000000;
					}
					if ((drawGeom.IndexBufferPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
						drawGeom.IndexBufferPointer = drawGeom.IndexBufferPointer & ~0x60000000;
					}

					file.seekg(drawGeom.IndexBufferPointer);

					IndexBuffer indexbuffer;
					file.read((char*)&indexbuffer, sizeof(IndexBuffer));

					//INDICES READING
					if ((indexbuffer.IndicesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
						indexbuffer.IndicesPointer = indexbuffer.IndicesPointer & ~0x50000000;
					}
					if ((indexbuffer.IndicesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
						indexbuffer.IndicesPointer = indexbuffer.IndicesPointer & ~0x60000000;
					}

					Mesh* newMesh = new Mesh(file._buffer.p, vertbuffer.DataPointer1, vertbuffer.VertexCount * vertbuffer.VertexStride, indexbuffer.IndicesPointer, indexbuffer.IndicesCount, (VertexType)decl.Flags, 0);
					meshes.push_back(newMesh);

					file.seekg(pos);
				}

				file.seekg(posOriginal);
			}
		}
		file.seekg(DrawablePointer);
	}
}

glm::mat4& YddLoader::GetMat4()
{
	return ModelMatrix;
}

YddLoader::~YddLoader()
{
	for (auto& mesh : meshes)
	{
		delete mesh;
	}
}

void YddLoader::Draw()
{
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i]->Draw();
	}
}