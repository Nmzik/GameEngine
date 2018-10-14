#include "YdrLoader.h"
#include "YbnLoader.h"
#include "YtdLoader.h"
#include "GTAEncryption.h"
#include <algorithm>

void YdrLoader::Init(memstream2 & file, int32_t systemSize)
{
	Loaded = true;

	DrawableBase* drawBase = (DrawableBase*)file.read(sizeof(DrawableBase));

	if (drawBase->DrawableModelsXPointer == 0) {
		printf("");
	}


	//READ COLLISION DATA FROM YDR
	if (isYft) {
		FragDrawable* fragDrawable = (FragDrawable*)file.read(sizeof(FragDrawable));

		if (fragDrawable->BoundPointer != 0) {
			SYSTEM_BASE_PTR(fragDrawable->BoundPointer);
			file.seekg(fragDrawable->BoundPointer);

			//ybnfile = myNew YbnLoader(world, file);
		}
	}
	else {
		Drawable* drawable = (Drawable*)file.read(sizeof(Drawable));

		if (drawable->LightAttributesPointer != 0) {
			SYSTEM_BASE_PTR(drawable->LightAttributesPointer);

			file.seekg(drawable->LightAttributesPointer);

			std::vector<LightAttributes_s> lightAttributes_s;
			lightAttributes_s.resize(drawable->LightAttributesCount1);

			for (int i = 0; i < drawable->LightAttributesCount1; i++)
			{
				LightAttributes_s* light = (LightAttributes_s*)file.read(sizeof(LightAttributes_s));
				//lightAttributes_s.push_back(light);
			}
		}
		if (drawable->BoundPointer != 0) {
			SYSTEM_BASE_PTR(drawable->BoundPointer);
			file.seekg(drawable->BoundPointer);

			ybnfile = YbnPool.getPool().Load();
			ybnfile->Init(file);
			//ybnfile->Finalize(world);
		}
	}

	//Shader stuff

	SYSTEM_BASE_PTR(drawBase->ShaderGroupPointer);

	file.seekg(drawBase->ShaderGroupPointer);

	ShaderGroup* _ShaderGroup = (ShaderGroup*)file.read(sizeof(ShaderGroup));

	if (_ShaderGroup->TextureDictionaryPointer != 0) {
		SYSTEM_BASE_PTR(_ShaderGroup->TextureDictionaryPointer);
		file.seekg(_ShaderGroup->TextureDictionaryPointer);
		Ytd = YtdPool.getPool().Load();
		Ytd->Init(file, systemSize);
	}

	SYSTEM_BASE_PTR(_ShaderGroup->ShadersPointer);
	file.seekg(_ShaderGroup->ShadersPointer);

	std::vector<Material> materials;
	materials.reserve(_ShaderGroup->ShadersCount1);

	for (int i = 0; i < _ShaderGroup->ShadersCount1; i++)
	{
		uint64_t* data_pointer = (uint64_t*)file.read(sizeof(uint64_t));
		uint64_t posOriginal = file.tellg();

		SYSTEM_BASE_PTR(data_pointer[0]);
		file.seekg(data_pointer[0]);

		ShaderFX* shaderFX = (ShaderFX*)file.read(sizeof(ShaderFX));

		SYSTEM_BASE_PTR(shaderFX->ParametersPointer);
		file.seekg(shaderFX->ParametersPointer);

		std::vector<uint32_t> TexturesHashes;

		int offset = 0;

		for (int i = 0; i < shaderFX->ParameterCount; i++)
		{
			ShaderParameter* param = (ShaderParameter*)file.read(sizeof(ShaderParameter));

			switch (param->DataType)
			{
				case 0:

					if (param->DataPointer == 0) {
						TexturesHashes.push_back(0);
					}
					else {

						uint64_t Pos = file.tellg();

						SYSTEM_BASE_PTR(param->DataPointer);

						file.seekg(param->DataPointer);

						TextureBase* texBase = (TextureBase*)file.read(sizeof(TextureBase));

						SYSTEM_BASE_PTR(texBase->NamePointer);

						file.seekg(texBase->NamePointer);

						char* Namearray = (char*)&file.data[texBase->NamePointer];
						std::string Name(&Namearray[0]);

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
					offset += 16 * param->DataType;
					TexturesHashes.push_back(0); //NOT ERROR
					break;
			}
		}

		file.seekCur(offset);

		//if (shaderFX->TextureParametersCount > 0)
			//file.read((char*)&ShaderHashes[0], sizeof(uint32_t) * shaderFX.TextureParametersCount);

		uint32_t DiffuseSampler = 0;
		uint32_t BumpSampler = 0;
		uint32_t SpecularSampler = 0;
		uint32_t DetailSampler = 0;

		if ((shaderFX->TextureParametersCount > 0)) {

			for (int i = 0; i < shaderFX->TextureParametersCount; i++)
			{
				uint32_t* ShaderName = (uint32_t*)file.read(sizeof(uint32_t));

				if (ShaderName[0] == 4059966321 || (ShaderName[0] == 3576369631)) { //DiffuseSampler
					DiffuseSampler = TexturesHashes[i];
				}
				if (ShaderName[0] == 1186448975) { //BumpSampler
					BumpSampler = TexturesHashes[i];
				}
				if (ShaderName[0] == 1619499462) { //SpecularSampler
					SpecularSampler = TexturesHashes[i];
				}
				if (ShaderName[0] == 3393362404) { //DetailSampler
					DetailSampler = TexturesHashes[i];
				}
			}

		}

		Material newMat(DiffuseSampler, BumpSampler, SpecularSampler, DetailSampler);
		materials.push_back(newMat);

		file.seekg(posOriginal);

	}

	SYSTEM_BASE_PTR(drawBase->DrawableModelsXPointer);
	file.seekg(drawBase->DrawableModelsXPointer);

	ResourcePointerList64* resourcePointerList = (ResourcePointerList64*)file.read(sizeof(ResourcePointerList64));

	SYSTEM_BASE_PTR(resourcePointerList->EntriesPointer);
	file.seekg(resourcePointerList->EntriesPointer);

	for (int i = 0; i < resourcePointerList->EntriesCount; i++)
	{
		uint64_t* data_pointer = (uint64_t*)file.read(sizeof(uint64_t));
		uint64_t posOriginal = file.tellg();

		SYSTEM_BASE_PTR(data_pointer[0]);

		file.seekg(data_pointer[0]);

		DrawableModel* drawModel = (DrawableModel*)file.read(sizeof(DrawableModel));

		SYSTEM_BASE_PTR(drawModel->ShaderMappingPointer);

		file.seekg(drawModel->ShaderMappingPointer);

		//uint64_t* testdata = (uint64_t*)file.read(sizeof(uint16_t) * drawModel->GeometriesCount1);
		//uint64_t testdata1 = testdata[0];
		//std::vector<uint16_t*> ShaderMapping;
		//ShaderMapping.resize(sizeof(uint16_t) * drawModel->GeometriesCount1);
		//ShaderMapping.data() = (uint16_t*)file.read(sizeof(uint16_t) * drawModel->GeometriesCount1);
		//file.read((char*)&ShaderMapping[0], sizeof(uint16_t) * drawModel.GeometriesCount1);

		SYSTEM_BASE_PTR(drawModel->GeometriesPointer);

		file.seekg(drawModel->GeometriesPointer);

		//Optimization
		meshes = new std::vector<Mesh>();
		meshes->reserve(drawModel->GeometriesCount1);

		for (int i = 0; i < drawModel->GeometriesCount1; i++) //no difference btween geometriescount1 and 2
		{
			uint64_t* data_pointer = (uint64_t*)file.read(sizeof(uint64_t));
			uint64_t pos = file.tellg();

			SYSTEM_BASE_PTR(data_pointer[0]);

			file.seekg(data_pointer[0]);

			DrawableGeometry* drawGeom = (DrawableGeometry*)file.read(sizeof(DrawableGeometry));

			SYSTEM_BASE_PTR(drawGeom->VertexBufferPointer);

			file.seekg(drawGeom->VertexBufferPointer);

			VertexBuffer* vertbuffer = (VertexBuffer*)file.read(sizeof(VertexBuffer));
			SYSTEM_BASE_PTR(vertbuffer->DataPointer1);

			vertbuffer->InfoPointer = (VertexDeclaration*)&file.data[(uint64_t)vertbuffer->InfoPointer & ~0x50000000];

			//FIX VertexDeclaration
			switch (vertbuffer->InfoPointer->Types)
			{
				/*case 8598872888530528662: //YDR - 0x7755555555996996
				break;*/
				case 216172782140628998:  //YFT - 0x030000000199A006
					switch (vertbuffer->InfoPointer->Flags)
					{
						case 16473:
							vertbuffer->InfoPointer->Flags = VertexType::PCCH2H4;
							break;  //  PCCH2H4 
					}
					break;
				case 216172782140612614:  //YFT - 0x0300000001996006  PNCH2H4
					switch (vertbuffer->InfoPointer->Flags)
					{
						case 89:
							vertbuffer->InfoPointer->Flags = VertexType::PNCH2;
							break;     //  PNCH2
					}
					break;
			}

			SYSTEM_BASE_PTR(drawGeom->IndexBufferPointer);

			file.seekg(drawGeom->IndexBufferPointer);

			IndexBuffer* indexbuffer = (IndexBuffer*)file.read(sizeof(IndexBuffer));

			SYSTEM_BASE_PTR(indexbuffer->IndicesPointer);

			gpuMemory += vertbuffer->VertexCount * vertbuffer->VertexStride;
			gpuMemory += indexbuffer->IndicesCount * sizeof(uint16_t);

			meshes->emplace_back(file.data, vertbuffer->DataPointer1, vertbuffer->VertexCount * vertbuffer->VertexStride, indexbuffer->IndicesPointer, indexbuffer->IndicesCount, (VertexType)vertbuffer->InfoPointer->Flags, materials[file.data[drawModel->ShaderMappingPointer + i * sizeof(uint16_t)]]);

			file.seekg(pos);
		}
		file.seekg(posOriginal);
	}
}

void YdrLoader::Remove()
{
	gpuMemory = 0;
	Loaded = false;
	if (ybnfile) {
		YbnPool.getPool().Remove(ybnfile);
		ybnfile = nullptr;
	}
	if (Ytd) {
		YtdPool.getPool().Remove(Ytd);
		Ytd = nullptr;
	}
	for (auto& mesh : *meshes)
	{
		mesh.Cleanup();
	}
	delete meshes;
}

void YdrLoader::UploadMeshes()
{
	for (auto& mesh : *meshes)
	{
		//mesh->Upload();
	}
	Loaded = true;
}
