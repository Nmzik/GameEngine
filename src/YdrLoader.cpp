#include "YdrLoader.h"
#include "GTAEncryption.h"
#include <algorithm>

void YdrLoader::Init(memstream& file)
{
	Loaded = true;

	rmcDrawable* drawBase = (rmcDrawable*)file.read(sizeof(rmcDrawable));
	drawBase->Resolve(file);

	//	READ COLLISION DATA FROM YDR
	if (isYft)
	{
		FragDrawable* fragDrawable = (FragDrawable*)file.read(sizeof(FragDrawable));

		if (fragDrawable->BoundPointer != 0)
		{
			SYSTEM_BASE_PTR(fragDrawable->BoundPointer);
			file.seekg(fragDrawable->BoundPointer);

			ybn = std::make_unique<YbnLoader>();
			ybn->Init(file);
		}
	}
	else
	{
		gtaDrawable* drawable = (gtaDrawable*)file.read(sizeof(gtaDrawable));

		if (drawable->LightAttributesPointer != 0)
		{
			SYSTEM_BASE_PTR(drawable->LightAttributesPointer);
			file.seekg(drawable->LightAttributesPointer);

			std::vector<CLightAttr > lightAttributes_s;
			lightAttributes_s.resize(drawable->LightAttributesCount1);

			for (int i = 0; i < drawable->LightAttributesCount1; i++)
			{
				CLightAttr * light = (CLightAttr *)file.read(sizeof(CLightAttr));
				//	lightAttributes_s.push_back(light);
			}
		}
		if (drawable->BoundPointer != 0)
		{
			SYSTEM_BASE_PTR(drawable->BoundPointer);
			file.seekg(drawable->BoundPointer);

			ybn = std::make_unique<YbnLoader>();
			ybn->Init(file);
			//	ybnfile->Finalize(world);
		}
	}

	//	Shader stuff
	if (drawBase->ShaderGroupPointer.pointer)
	{ //	IF POINTER = 0 NO OBJECTS???

		if (drawBase->ShaderGroupPointer->TextureDictionaryPointer != 0)
		{
			SYSTEM_BASE_PTR(drawBase->ShaderGroupPointer->TextureDictionaryPointer);
			file.seekg(drawBase->ShaderGroupPointer->TextureDictionaryPointer);
			ytd = std::make_unique<YtdLoader>();
			ytd->Init(file);
		}

		SYSTEM_BASE_PTR(drawBase->ShaderGroupPointer->ShadersPointer);
		file.seekg(drawBase->ShaderGroupPointer->ShadersPointer);

		std::vector<Material> materials;
		materials.reserve(drawBase->ShaderGroupPointer->ShadersCount1);

		for (int i = 0; i < drawBase->ShaderGroupPointer->ShadersCount1; i++)
		{
			uint64_t* data_pointer = (uint64_t*)file.read(sizeof(uint64_t));
			uint64_t posOriginal = file.tellg();

			SYSTEM_BASE_PTR(data_pointer[0]);
			file.seekg(data_pointer[0]);

			grmShaderFx* shaderFX = (grmShaderFx*)file.read(sizeof(grmShaderFx));

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

						if (param->DataPointer == 0)
						{
							TexturesHashes.push_back(0);
						}
						else
						{

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
					case 1: //	SOME OTHER SHIT OTHER THAN TEXTURE
						offset += 16;
						TexturesHashes.push_back(0);
						break;
					default:
						offset += 16 * param->DataType;
						TexturesHashes.push_back(0); //	NOT ERROR
						break;
				}
			}

			file.seekCur(offset);

			//	if (shaderFX->TextureParametersCount > 0)
			//	file.read((char*)&ShaderHashes[0], sizeof(uint32_t) * shaderFX.TextureParametersCount);

			uint32_t DiffuseSampler = 0;
			uint32_t BumpSampler = 0;
			uint32_t SpecularSampler = 0;
			uint32_t DetailSampler = 0;

			if ((shaderFX->TextureParametersCount > 0))
			{

				for (int i = 0; i < shaderFX->TextureParametersCount; i++)
				{
					uint32_t* ShaderName = (uint32_t*)file.read(sizeof(uint32_t));

					if (ShaderName[0] == 4059966321 || (ShaderName[0] == 3576369631))
					{ //	DiffuseSampler
						DiffuseSampler = TexturesHashes[i];
					}
					else if (ShaderName[0] == 1186448975)
					{ //	BumpSampler
						BumpSampler = TexturesHashes[i];
					}
					else if (ShaderName[0] == 1619499462)
					{ //	SpecularSampler
						SpecularSampler = TexturesHashes[i];
					}
					else if (ShaderName[0] == 3393362404)
					{ //	DetailSampler
						DetailSampler = TexturesHashes[i];
					}
				}
			}

			Material newMat(DiffuseSampler, BumpSampler, SpecularSampler, DetailSampler);
			materials.push_back(newMat);

			file.seekg(posOriginal);
		}

		//////////

		models.resize(drawBase->DrawableModels[0]->getSize());

		for (int i = 0; i < drawBase->DrawableModels[0]->getSize(); i++)
		{
			//	Optimization
			models[i].meshes.reserve(drawBase->DrawableModels[0]->Get(i)->m_geometries.getSize());

			for (int j = 0; j < drawBase->DrawableModels[0]->Get(i)->m_geometries.getSize(); j++)
			{
				switch (drawBase->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->InfoPointer->Types)
				{
					/*case 8598872888530528662: //YDR - 0x7755555555996996
					break;*/
					case 216172782140628998: //	YFT - 0x030000000199A006
						switch (drawBase->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->InfoPointer->Flags)
						{
							case 16473: drawBase->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->InfoPointer->Flags = VertexType::PCCH2H4; break; //  PCCH2H4
						}
						break;
					case 216172782140612614: //	YFT - 0x0300000001996006  PNCH2H4
						switch (drawBase->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->InfoPointer->Flags)
						{
							case 89: drawBase->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->InfoPointer->Flags = VertexType::PNCH2; break; //  PNCH2
						}
						break;
				}

				gpuMemory +=
					drawBase->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->VertexCount * drawBase->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->VertexStride;
				gpuMemory += drawBase->DrawableModels[0]->Get(i)->m_geometries.Get(j)->IndexBufferPointer->IndicesCount * sizeof(uint16_t);

				models[i].meshes.emplace_back(file.data, drawBase->DrawableModels[0]->Get(i)->m_geometries.Get(j), materials[(*drawBase->DrawableModels[0]->Get(i)->ShaderMappingPointer)[j]]);
			}
		}
	}
}