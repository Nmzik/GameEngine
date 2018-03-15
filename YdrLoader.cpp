#include "YdrLoader.h"

YdrLoader::YdrLoader(memstream& file, uint32_t hash, btDiscreteDynamicsWorld* world) : Hash(hash)
{
	std::vector<uint32_t> TexturesHashes;

	ResourceFileBase resourceFileBase;
	file.read((char*)&resourceFileBase, sizeof(ResourceFileBase));

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

	if (Drawable.LightAttributesPointer != 0) {
		TranslatePTR(Drawable.LightAttributesPointer);

		file.seekg(Drawable.LightAttributesPointer);

		std::vector<LightAttributes_s> lightAttributes_s;
		lightAttributes_s.resize(Drawable.LightAttributesCount1);

		for (int i = 0; i < Drawable.LightAttributesCount1; i++)
		{
			LightAttributes_s light;
			file.read((char*)&light, sizeof(LightAttributes_s));
			lightAttributes_s.push_back(light);
		}
	}

	if (Drawable.BoundPointer != 0) {
		printf("YBN INSIDE YDR\n");
		//YbnLoader* loader = new YbnLoader(world, file, hash);
	}

	//Shader stuff
	TranslatePTR(drawBase.ShaderGroupPointer);

	file.seekg(drawBase.ShaderGroupPointer);

	ShaderGroup _ShaderGroup;

	file.read((char*)&_ShaderGroup, sizeof(ShaderGroup));

	if (_ShaderGroup.TextureDictionaryPointer != 0) {
		TranslatePTR(_ShaderGroup.TextureDictionaryPointer);

		file.seekg(_ShaderGroup.TextureDictionaryPointer);
		printf("YTD INSIDE YDR\n");
		ytdFile = new YtdFile(file, hash);
	}

	TranslatePTR(_ShaderGroup.ShadersPointer);

	file.seekg(_ShaderGroup.ShadersPointer);

	for (int i = 0; i < _ShaderGroup.ShadersCount1; i++)
	{
		uint64_t data_pointer;
		file.read((char*)&data_pointer, sizeof(data_pointer));
		uint64_t posOriginal = file.tellg();

		TranslatePTR(data_pointer);

		file.seekg(data_pointer);

		ShaderFX shaderFX;

		file.read((char*)&shaderFX, sizeof(shaderFX));

		TranslatePTR(shaderFX.ParametersPointer);

		file.seekg(shaderFX.ParametersPointer);

		for (int i = 0; i < 1; i++) ///i < shaderFX.ParameterCount
		{
			ShaderParameter param;
			file.read((char*)&param, sizeof(ShaderParameter));

			if (param.DataType == 1) { //SOME OTHER SHIT OTHER THAN TEXTURE
				TexturesHashes.push_back(0);
			} else

			if (param.DataType == 0) {

				if (param.DataPointer == 0) {
					TexturesHashes.push_back(0);
				}
				else {

					uint64_t Pos = file.tellg();

					TranslatePTR(param.DataPointer);

					file.seekg(param.DataPointer);

					TextureBase texBase;
					file.read((char*)&texBase, sizeof(TextureBase));

					TranslatePTR(texBase.NamePointer);

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

	TranslatePTR(drawBase.DrawableModelsXPointer);

	file.seekg(drawBase.DrawableModelsXPointer);

	ResourcePointerList64 resourcePointerList;

	file.read((char*)&resourcePointerList, sizeof(ResourcePointerList64));

	TranslatePTR(resourcePointerList.EntriesPointer);

	file.seekg(resourcePointerList.EntriesPointer);

	for (int i = 0; i < resourcePointerList.EntriesCount; i++)
	{
		uint64_t data_pointer;
		file.read((char*)&data_pointer, sizeof(data_pointer));
		uint64_t posOriginal = file.tellg();

		TranslatePTR(data_pointer);

		file.seekg(data_pointer);

		DrawableModel drawModel;

		file.read((char*)&drawModel, sizeof(drawModel));

		TranslatePTR(drawModel.ShaderMappingPointer);

		file.seekg(drawModel.ShaderMappingPointer);

		std::vector<uint16_t> ShaderMapping;
		ShaderMapping.resize(sizeof(uint16_t) * drawModel.GeometriesCount1);
		file.read((char*)&ShaderMapping[0], sizeof(uint16_t) * drawModel.GeometriesCount1);

		TranslatePTR(drawModel.GeometriesPointer);

		file.seekg(drawModel.GeometriesPointer);

		for (int i = 0; i < drawModel.GeometriesCount1; i++) //no difference btween geometriescount1 and 2
		{
			uint64_t data_pointer;
			file.read((char*)&data_pointer, sizeof(data_pointer));
			uint64_t pos = file.tellg();

			TranslatePTR(data_pointer);

			file.seekg(data_pointer);

			DrawableGeometry drawGeom;

			file.read((char*)&drawGeom, sizeof(DrawableGeometry));

			TranslatePTR(drawGeom.VertexBufferPointer);

			file.seekg(drawGeom.VertexBufferPointer);

			VertexBuffer vertbuffer;
			file.read((char*)&vertbuffer, sizeof(VertexBuffer));

			TranslatePTR(vertbuffer.DataPointer1);

			TranslatePTR(vertbuffer.InfoPointer);

			file.seekg(vertbuffer.InfoPointer);

			VertexDeclaration decl;
			file.read((char*)&decl, sizeof(VertexDeclaration));

			TranslatePTR(drawGeom.IndexBufferPointer);

			file.seekg(drawGeom.IndexBufferPointer);

			IndexBuffer indexbuffer;
			file.read((char*)&indexbuffer, sizeof(IndexBuffer));

			TranslatePTR(indexbuffer.IndicesPointer);

			Mesh* newMesh = new Mesh(file._buffer.p, vertbuffer.DataPointer1, vertbuffer.VertexCount * vertbuffer.VertexStride, indexbuffer.IndicesPointer, indexbuffer.IndicesCount, (VertexType)decl.Flags, TexturesHashes[ShaderMapping[i]]);
			meshes.push_back(newMesh);

			file.seekg(pos);
		}
		file.seekg(posOriginal);
	}
}


YdrLoader::~YdrLoader()
{
	delete ytdFile;
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

void YdrLoader::Draw()
{
	for (auto& mesh : meshes) {
		mesh->Draw();
	}
}
