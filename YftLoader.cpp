#include "YftLoader.h"

YftLoader::YftLoader(memstream& file, glm::vec3 position, glm::quat rotation, glm::vec3 scale, uint32_t hash, btDiscreteDynamicsWorld* world) : Hash(hash)
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

	struct ResourcePointerList64 {
		uint64_t EntriesPointer;
		uint16_t EntriesCount;
		uint16_t EntriesCapacity;
	};

	struct {
		uint32_t Unknown_10h; // 0x00000000
		uint32_t Unknown_14h; // 0x00000000
		uint32_t Unknown_18h; // 0x00000000
		uint32_t Unknown_1Ch; // 0x00000000
		uint32_t Unknown_20h;
		uint32_t Unknown_24h;
		uint32_t Unknown_28h;
		uint32_t Unknown_2Ch;
		uint64_t DrawablePointer;
		uint64_t Unknown_28h_Pointer;
		uint64_t Unknown_30h_Pointer;
		uint32_t Count0;
		uint32_t Unknown_4Ch;
		uint32_t Unknown_50h; // 0x00000000
		uint32_t Unknown_54h; // 0x00000000
		uint64_t NamePointer;
		ResourcePointerList64 Clothes;
		uint32_t Unknown_70h; // 0x00000000
		uint32_t Unknown_74h; // 0x00000000
		uint32_t Unknown_78h; // 0x00000000
		uint32_t Unknown_7Ch; // 0x00000000
		uint32_t Unknown_80h; // 0x00000000
		uint32_t Unknown_84h; // 0x00000000
		uint32_t Unknown_88h; // 0x00000000
		uint32_t Unknown_8Ch; // 0x00000000
		uint32_t Unknown_90h; // 0x00000000
		uint32_t Unknown_94h; // 0x00000000
		uint32_t Unknown_98h; // 0x00000000
		uint32_t Unknown_9Ch; // 0x00000000
		uint32_t Unknown_A0h; // 0x00000000
		uint32_t Unknown_A4h; // 0x00000000
		uint64_t Unknown_A8h_Pointer;
		uint32_t Unknown_B0h; // 0x00000000
		uint32_t Unknown_B4h; // 0x00000000
		uint32_t Unknown_B8h;
		uint32_t Unknown_BCh;
		uint32_t Unknown_C0h;
		uint32_t Unknown_C4h;
		uint32_t Unknown_C8h;
		uint32_t Unknown_CCh;
		uint32_t Unknown_D0h;
		uint32_t Unknown_D4h;
		uint8_t Unknown_D8h;
		uint8_t Count3;
		uint16_t Unknown_DAh;
		uint32_t Unknown_DCh; // 0x00000000
		uint64_t Unknown_E0h_Pointer;
		uint32_t Unknown_E8h; // 0x00000000
		uint32_t Unknown_ECh; // 0x00000000
		uint64_t PhysicsLODGroupPointer;
		uint64_t Unknown_F8h_Pointer;
		uint32_t Unknown_100h; // 0x00000000
		uint32_t Unknown_104h; // 0x00000000
		uint32_t Unknown_108h; // 0x00000000
		uint32_t Unknown_10Ch; // 0x00000000
		ResourceSimpleList64Ptr LightAttributesPtr;
		//public LightAttributes_s[] LightAttributes;
		uint64_t Unknown_120h_Pointer;
		uint32_t Unknown_128h; // 0x00000000
		uint32_t Unknown_12Ch; // 0x00000000
	} FragType;

	file.read((char*)&FragType, sizeof(FragType));

	if ((FragType.DrawablePointer & SYSTEM_BASE) == SYSTEM_BASE) {
		FragType.DrawablePointer = FragType.DrawablePointer & ~0x50000000;
	}
	if ((FragType.DrawablePointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		FragType.DrawablePointer = FragType.DrawablePointer & ~0x60000000;
	}

	file.seekg(FragType.DrawablePointer);

	file.read((char*)&ResourceFileBase, sizeof(ResourceFileBase));

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
		printf("YBN INSIDE YFT\n");
		YbnLoader* loader = new YbnLoader(world, file, hash);
	}

	//Shader stuff
	if ((drawBase.ShaderGroupPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		drawBase.ShaderGroupPointer = drawBase.ShaderGroupPointer & ~0x50000000;
	}
	if ((drawBase.ShaderGroupPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		drawBase.ShaderGroupPointer = drawBase.ShaderGroupPointer & ~0x60000000;
	}

	file.seekg(drawBase.ShaderGroupPointer);

	ShaderGroup shaderGroup;

	file.read((char*)&shaderGroup, sizeof(ShaderGroup));

	if (shaderGroup.TextureDictionaryPointer != 0) {
		if ((shaderGroup.TextureDictionaryPointer & SYSTEM_BASE) == SYSTEM_BASE) {
			shaderGroup.TextureDictionaryPointer = shaderGroup.TextureDictionaryPointer & ~0x50000000;
		}
		if ((shaderGroup.TextureDictionaryPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
			shaderGroup.TextureDictionaryPointer = shaderGroup.TextureDictionaryPointer & ~0x60000000;
		}

		file.seekg(shaderGroup.TextureDictionaryPointer);
		printf("YTD INSIDE YFT\n");
		YtdFile* loader = new YtdFile(file, hash);
	}

	if ((shaderGroup.ShadersPointer & SYSTEM_BASE) == SYSTEM_BASE) {
		shaderGroup.ShadersPointer = shaderGroup.ShadersPointer & ~0x50000000;
	}
	if ((shaderGroup.ShadersPointer & GRAPHICS_BASE) == GRAPHICS_BASE) {
		shaderGroup.ShadersPointer = shaderGroup.ShadersPointer & ~0x60000000;
	}

	file.seekg(shaderGroup.ShadersPointer);

	for (int i = 0; i < shaderGroup.ShadersCount1; i++)
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

		file.read((char*)&shaderFX, sizeof(ShaderFX));

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

			file.seekg(pos);
		}
		file.seekg(posOriginal);
	}
	/*struct {
		uint32_t Unknown_0A8h;
		uint32_t Unknown_0ACh;
		public SharpDX.Matrix Unknown_0B0h;
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
	} FragDrawable;*/
}


YftLoader::~YftLoader()
{
}

glm::mat4 & YftLoader::GetMat4()
{
	return ModelMatrix;
}

void YftLoader::Draw()
{
	for (auto& mesh : meshes) {
		mesh->Draw();
	}
}
