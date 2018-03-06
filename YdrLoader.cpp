#include "YdrLoader.h"

YdrLoader::YdrLoader(memstream& file, glm::vec3 position, glm::quat rotation, glm::vec3 scale, uint32_t hash, btDiscreteDynamicsWorld* world) : Hash(hash)
{
	ModelMatrix = glm::translate(glm::mat4(), position) * glm::toMat4(rotation) * glm::scale(glm::mat4(), scale);

	std::vector<uint32_t> TexturesHashes;

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

	//std::vector<VertexBuffer> vertexBuffer;
	//std::vector<IndexBuffer> indexBuffer;

	std::vector<DrawableGeometry> Geometries;

	DrawableBase drawBase;

	file.read((char*)&drawBase, sizeof(drawBase));

	struct {
		uint64_t NamePointer;
		uint64_t LightAttributesPointer;
		uint16_t LightAttributesCount1;
		uint16_t LightAttributesCount2;
		uint32_t Unknown_BCh; // 0x00000000
		uint32_t Unknown_C0h; // 0x00000000
		uint32_t Unknown_C4h; // 0x00000000
		uint64_t BoundPointer;
	} Drawable;

	//READ COLLISION DATA FROM YDR
	file.read((char*)&Drawable, sizeof(Drawable));

	if (Drawable.BoundPointer != 0) {
		printf("YBN INSIDE YDR\n");
		YbnLoader* loader = new YbnLoader(world, file, hash);
	}

	BBCenter = position + drawBase.BoundingCenter;
	BBRadius = drawBase.BoundingSphereRadius;

	//Shader stuff
	if ((drawBase.ShaderGroupPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		drawBase.ShaderGroupPointer = drawBase.ShaderGroupPointer & ~0x50000000;
	}
	if ((drawBase.ShaderGroupPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		drawBase.ShaderGroupPointer = drawBase.ShaderGroupPointer & ~0x60000000;
	}

	file.seekg(drawBase.ShaderGroupPointer);

	ShaderGroup _ShaderGroup;

	file.read((char*)&_ShaderGroup, sizeof(ShaderGroup));

	if (_ShaderGroup.TextureDictionaryPointer != 0) {
		if ((_ShaderGroup.TextureDictionaryPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			_ShaderGroup.TextureDictionaryPointer = _ShaderGroup.TextureDictionaryPointer & ~0x50000000;
		}
		if ((_ShaderGroup.TextureDictionaryPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			_ShaderGroup.TextureDictionaryPointer = _ShaderGroup.TextureDictionaryPointer & ~0x60000000;
		}

		file.seekg(_ShaderGroup.TextureDictionaryPointer);
		printf("YTD INSIDE YDR\n");
		YtdFile* loader = new YtdFile(file, hash);
	}

	if ((_ShaderGroup.ShadersPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		_ShaderGroup.ShadersPointer = _ShaderGroup.ShadersPointer & ~0x50000000;
	}
	if ((_ShaderGroup.ShadersPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		_ShaderGroup.ShadersPointer = _ShaderGroup.ShadersPointer & ~0x60000000;
	}

	file.seekg(_ShaderGroup.ShadersPointer);

	for (int i = 0; i < _ShaderGroup.ShadersCount1; i++)
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

		ShaderFX shaderFX;

		file.read((char*)&shaderFX, sizeof(shaderFX));

		if ((shaderFX.ParametersPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			shaderFX.ParametersPointer = shaderFX.ParametersPointer & ~0x50000000;
		}
		if ((shaderFX.ParametersPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			shaderFX.ParametersPointer = shaderFX.ParametersPointer & ~0x60000000;
		}

		file.seekg(shaderFX.ParametersPointer);

		for (int i = 0; i < 1; i++) ///i < shaderFX.ParameterCount
		{
			ShaderParameter param;
			file.read((char*)&param, sizeof(ShaderParameter));

			if (param.DataType == 0) {

				if (param.DataPointer == 0) {
					TexturesHashes.push_back(0);
				}
				else {

					uint64_t Pos = file.tellg();

					if ((param.DataPointer & SYSTEM_BASE) == SYSTEM_BASE) {
						param.DataPointer = param.DataPointer & ~0x50000000;
					}
					if ((param.DataPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
						param.DataPointer = param.DataPointer & ~0x60000000;
					}

					file.seekg(param.DataPointer);

					TextureBase texBase;
					file.read((char*)&texBase, sizeof(TextureBase));

					if ((texBase.NamePointer & SYSTEM_BASE) == SYSTEM_BASE) {
						texBase.NamePointer = texBase.NamePointer & ~0x50000000;
					}
					if ((texBase.NamePointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
						texBase.NamePointer = texBase.NamePointer & ~0x60000000;
					}

					file.seekg(texBase.NamePointer);

					std::string Name;
					std::getline(file, Name, '\0');

					std::transform(Name.begin(), Name.end(), Name.begin(), tolower);
					uint32_t NameHash = GenHash(Name);

					TexturesHashes.push_back(NameHash);

					file.seekg(Pos);
				}
			}
		}

		file.seekg(posOriginal);

	}

	uint64_t pos1 = file.tellg();

	if ((drawBase.DrawableModelsXPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		drawBase.DrawableModelsXPointer = drawBase.DrawableModelsXPointer & ~0x50000000;
	}
	if ((drawBase.DrawableModelsXPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		drawBase.DrawableModelsXPointer = drawBase.DrawableModelsXPointer & ~0x60000000;
	}

	file.seekg(drawBase.DrawableModelsXPointer);

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

		DrawableModel drawModel;

		file.read((char*)&drawModel, sizeof(drawModel));

		if ((drawModel.ShaderMappingPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			drawModel.ShaderMappingPointer = drawModel.ShaderMappingPointer & ~0x50000000;
		}
		if ((drawModel.ShaderMappingPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			drawModel.ShaderMappingPointer = drawModel.ShaderMappingPointer & ~0x60000000;
		}

		file.seekg(drawModel.ShaderMappingPointer);

		std::vector<uint16_t> ShaderMapping;
		ShaderMapping.resize(sizeof(uint16_t) * drawModel.GeometriesCount1);
		file.read((char*)&ShaderMapping[0], sizeof(uint16_t) * drawModel.GeometriesCount1);

		if ((drawModel.GeometriesPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			drawModel.GeometriesPointer = drawModel.GeometriesPointer & ~0x50000000;
		}
		if ((drawModel.GeometriesPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			drawModel.GeometriesPointer = drawModel.GeometriesPointer & ~0x60000000;
		}

		file.seekg(drawModel.GeometriesPointer);

		for (int i = 0; i < drawModel.GeometriesCount1; i++) //no difference btween geometriescount1 and 2
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

			uint32_t test;
			if (TexturesHashes.size() == 0) {
				test = 0;
			}
			else {
				test = TexturesHashes[ShaderMapping[i]];
			}
			Mesh* newMesh = new Mesh(file._buffer.p, vertbuffer.DataPointer1, vertbuffer.VertexCount * vertbuffer.VertexStride, indexbuffer.IndicesPointer, indexbuffer.IndicesCount, (VertexType)decl.Flags, test);
			meshes.push_back(newMesh);

			//Geometries.push_back(drawGeom);
			//vertexBuffer.push_back(vertbuffer);
			//indexBuffer.push_back(indexbuffer);

			file.seekg(pos);
		}
		file.seekg(posOriginal);
	}
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
