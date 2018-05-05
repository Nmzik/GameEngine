#include "YdrLoader.h"

YdrLoader::YdrLoader(memstream& file, int32_t systemSize, btDiscreteDynamicsWorld* world, bool isYft)
{
	Material material(0, 0, 0, 0);

	ResourceFileBase resourceFileBase;
	file.read((char*)&resourceFileBase, sizeof(ResourceFileBase));

	DrawableBase drawBase;
	file.read((char*)&drawBase, sizeof(drawBase));

	if (drawBase.DrawableModelsXPointer != 0) {

		//READ COLLISION DATA FROM YDR
		if (isYft) {
			FragDrawable fragDrawable;
			file.read((char*)&fragDrawable, sizeof(FragDrawable));

			if (fragDrawable.BoundPointer != 0) {
				TranslatePTR(fragDrawable.BoundPointer);
				file.seekg(fragDrawable.BoundPointer);

				//ybnfile = new YbnLoader(world, file);
			}
		}
		else {
			Drawable drawable;
			file.read((char*)&drawable, sizeof(Drawable));

			if (drawable.LightAttributesPointer != 0) {
				TranslatePTR(drawable.LightAttributesPointer);

				file.seekg(drawable.LightAttributesPointer);

				std::vector<LightAttributes_s> lightAttributes_s;
				lightAttributes_s.resize(drawable.LightAttributesCount1);

				for (int i = 0; i < drawable.LightAttributesCount1; i++)
				{
					LightAttributes_s light;
					file.read((char*)&light, sizeof(LightAttributes_s));
					lightAttributes_s.push_back(light);
				}
			}
			if (drawable.BoundPointer != 0) {
				TranslatePTR(drawable.BoundPointer);
				file.seekg(drawable.BoundPointer);

				//ybnfile = new YbnLoader(world, file);
			}
		}

		//Shader stuff

		TranslatePTR(drawBase.ShaderGroupPointer);

		file.seekg(drawBase.ShaderGroupPointer);

		ShaderGroup _ShaderGroup;

		file.read((char*)&_ShaderGroup, sizeof(ShaderGroup));

		if (_ShaderGroup.TextureDictionaryPointer != 0) {
			TranslatePTR(_ShaderGroup.TextureDictionaryPointer);
			file.seekg(_ShaderGroup.TextureDictionaryPointer);
			Ytd = new YtdLoader(file, systemSize);
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

			std::vector<uint32_t> ShaderHashes;
			ShaderHashes.resize(shaderFX.TextureParametersCount);

			std::vector<uint32_t> TexturesHashes;

			int offset = 0;

			for (int i = 0; i < shaderFX.ParameterCount; i++)
			{
				ShaderParameter param;
				file.read((char*)&param, sizeof(ShaderParameter));

				switch (param.DataType)
				{
				case 0:

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

					break;
				case 1: //SOME OTHER SHIT OTHER THAN TEXTURE
					offset += 16;
					TexturesHashes.push_back(0);
					break;
				default:
					offset += 16 * param.DataType;
					TexturesHashes.push_back(0); //NOT ERROR
					break;
				}
			}

			file.seekg(offset, std::ios::cur);

			if (shaderFX.TextureParametersCount > 0)
				file.read((char*)&ShaderHashes[0], sizeof(uint32_t) * shaderFX.TextureParametersCount);

			uint32_t DiffuseSampler = 0;
			uint32_t BumpSampler = 0;
			uint32_t SpecularSampler = 0;
			uint32_t DetailSampler = 0;

			for (int i = 0; i < ShaderHashes.size(); i++)
			{
				if (ShaderHashes[i] == 4059966321) { //DiffuseSampler
					DiffuseSampler = TexturesHashes[i];
				}
				if (ShaderHashes[i] == 1186448975) { //BumpSampler
					BumpSampler = TexturesHashes[i];
				}
				if (ShaderHashes[i] == 1619499462) { //SpecularSampler
					SpecularSampler = TexturesHashes[i];
				}
				if (ShaderHashes[i] == 3393362404) { //DetailSampler
					DetailSampler = TexturesHashes[i];
				}
			}

			Material newMat(DiffuseSampler, BumpSampler, SpecularSampler, DetailSampler);
			materials.push_back(newMat);

			file.seekg(posOriginal);

		}

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

			//Optimization
			meshes.reserve(drawModel.GeometriesCount1);

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

				//FIX VertexDeclaration
				switch (decl.Types)
				{
					/*case 8598872888530528662: //YDR - 0x7755555555996996
						break;*/
				case 216172782140628998:  //YFT - 0x030000000199A006
					switch (decl.Flags)
					{
					case 16473:
						decl.Flags = VertexType::PCCH2H4;
						break;  //  PCCH2H4 
					}
					break;
				case 216172782140612614:  //YFT - 0x0300000001996006  PNCH2H4
					switch (decl.Flags)
					{
					case 89:
						decl.Flags = VertexType::PNCH2;
						break;     //  PNCH2
					}
					break;
				}

				TranslatePTR(drawGeom.IndexBufferPointer);

				file.seekg(drawGeom.IndexBufferPointer);

				IndexBuffer indexbuffer;
				file.read((char*)&indexbuffer, sizeof(IndexBuffer));

				TranslatePTR(indexbuffer.IndicesPointer);

				if (materials.size() == 0) {
					meshes.emplace_back(file._buffer.p, vertbuffer.DataPointer1, vertbuffer.VertexCount * vertbuffer.VertexStride, indexbuffer.IndicesPointer, indexbuffer.IndicesCount, (VertexType)decl.Flags, material);
				}
				else {
					meshes.emplace_back(file._buffer.p, vertbuffer.DataPointer1, vertbuffer.VertexCount * vertbuffer.VertexStride, indexbuffer.IndicesPointer, indexbuffer.IndicesCount, (VertexType)decl.Flags, materials[ShaderMapping[i]]);

				}

				file.seekg(pos);
			}
			file.seekg(posOriginal);
		}
	}
}


YdrLoader::~YdrLoader()
{
	if (ybnfile)
		delete ybnfile;
	delete Ytd;
	for (auto& mesh : meshes)
	{
		mesh.Cleanup();
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

void YdrLoader::Draw(Shader* shader)
{
	for (auto& mesh : meshes) {
		mesh.Draw(shader);
	}
}
