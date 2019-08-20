#include "YdrLoader.h"
#include <algorithm>
#include "GTAEncryption.h"

void YdrLoader::init(GameRenderer* _renderer, memstream& file)
{
    renderer = _renderer;
    loaded = true;

    rmcDrawable* drawable = nullptr;
    //	READ COLLISION DATA FROM YDR
    if (isYft)
    {
        FragDrawable* fragDrawable = (FragDrawable*)file.read(sizeof(FragDrawable));
        fragDrawable->Resolve(file);

        drawable = (rmcDrawable*)fragDrawable;

        if (fragDrawable->BoundPointer != 0)
        {
            /*file.seekg(fragDrawable->BoundPointer);

            ybn = std::make_unique<YbnLoader>();
            ybn->Init(file);*/
        }
    }
    else
    {
        gtaDrawable* GTAdrawable = (gtaDrawable*)file.read(sizeof(gtaDrawable));
        GTAdrawable->Resolve(file);

        drawable = (rmcDrawable*)GTAdrawable;

        if (GTAdrawable->BoundPointer)
        {
            /*file.seekg(GTAdrawable->BoundPointer);

            ybn = std::make_unique<YbnLoader>();
            ybn->Init(file);*/
        }
    }

    materials.resize(drawable->ShaderGroupPointer->Shaders.size());

    //	Shader stuff
    if (drawable->ShaderGroupPointer.pointer)
    {  //	IF POINTER = 0 NO OBJECTS???

        if (drawable->ShaderGroupPointer->TextureDictionaryPointer != 0)
        {
            SYSTEM_BASE_PTR(drawable->ShaderGroupPointer->TextureDictionaryPointer);
            file.seekg(drawable->ShaderGroupPointer->TextureDictionaryPointer);
            ytd = GlobalPool::GetInstance()->ytdPool.create();
            ytd->init(renderer, file);
        }

        materials.reserve(drawable->ShaderGroupPointer->Shaders.size());

        for (int is = 0; is < drawable->ShaderGroupPointer->Shaders.size(); is++)
        {
            SYSTEM_BASE_PTR(drawable->ShaderGroupPointer->Shaders.Get(is)->ParametersPointer);
            file.seekg(drawable->ShaderGroupPointer->Shaders.Get(is)->ParametersPointer);

            std::vector<uint32_t> TexturesHashes;
            TexturesHashes.reserve(drawable->ShaderGroupPointer->Shaders.Get(is)->ParameterCount);

            int offset = 0;

            for (int j = 0; j < drawable->ShaderGroupPointer->Shaders.Get(is)->ParameterCount; j++)
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

                            char* TextureName = file.getString();

                            size_t NameLength = strlen(TextureName);

                            for (int i = 0; i < NameLength; i++)
                            {
                                TextureName[i] = tolower(TextureName[i]);
                            }

                            uint32_t NameHash = GenHash(std::string_view(TextureName, NameLength));

                            TexturesHashes.push_back(NameHash);

                            file.seekg(Pos);
                        }

                        break;
                    case 1:  //	SOME OTHER SHIT OTHER THAN TEXTURE
                        offset += 16;
                        TexturesHashes.push_back(0);
                        break;
                    default:
                        offset += 16 * param->DataType;
                        TexturesHashes.push_back(0);  //	NOT ERROR
                        break;
                }
            }

            file.seekCur(offset);

            uint32_t DiffuseSampler = 0;
            uint32_t BumpSampler = 0;
            uint32_t SpecularSampler = 0;
            uint32_t DetailSampler = 0;

            if ((drawable->ShaderGroupPointer->Shaders.Get(is)->TextureParametersCount > 0))
            {
                uint32_t* ShaderHashes = (uint32_t*)file.read(sizeof(uint32_t) * drawable->ShaderGroupPointer->Shaders.Get(is)->TextureParametersCount);
                //
                for (int i = 0; i < drawable->ShaderGroupPointer->Shaders.Get(is)->TextureParametersCount; i++)
                {
                    if (ShaderHashes[i] == 4059966321 || (ShaderHashes[i] == 3576369631))
                    {  //	DiffuseSampler
                        DiffuseSampler = TexturesHashes[i];
                    }
                    else if (ShaderHashes[i] == 1186448975)
                    {  //	BumpSampler
                        BumpSampler = TexturesHashes[i];
                    }
                    else if (ShaderHashes[i] == 1619499462)
                    {  //	SpecularSampler
                        SpecularSampler = TexturesHashes[i];
                    }
                    else if (ShaderHashes[i] == 3393362404)
                    {  //	DetailSampler
                        DetailSampler = TexturesHashes[i];
                    }
                }
            }

            Material newMat{DiffuseSampler, BumpSampler, SpecularSampler, DetailSampler};
            materials.push_back(newMat);
        }
    }
    //////////

    models.resize(drawable->DrawableModels[0]->size());

    for (int i = 0; i < drawable->DrawableModels[0]->size(); i++)
    {
        models[i].Unk_2Ch = drawable->DrawableModels[0]->Get(i)->Unknown_2Ch;
        //	Optimization
        models[i].geometries.reserve(drawable->DrawableModels[0]->Get(i)->m_geometries.size());

        for (int j = 0; j < drawable->DrawableModels[0]->Get(i)->m_geometries.size(); j++)
        {
            if (isYft)
            {
                switch (drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->InfoPointer->Types)
                {
                    case 216172782140628998:  //	YFT - 0x030000000199A006
                        switch (drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->InfoPointer->Flags)
                        {
                            case 16473:
                                drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->InfoPointer->Flags = VertexType::PCCH2H4;
                                break;  //  PCCH2H4
                        }
                        break;
                    case 216172782140612614:  //	YFT - 0x0300000001996006  PNCH2H4
                        switch (drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->InfoPointer->Flags)
                        {
                            case 89:
                                drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->InfoPointer->Flags = VertexType::PNCH2;
                                break;  //  PNCH2
                        }
                        break;
                }
            }

            gpuMemory += drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->VertexCount *
                         drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->VertexStride;
            gpuMemory += drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j)->IndexBufferPointer->IndicesCount * sizeof(uint16_t);

            grmGeometry* geom = drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j);

            int vertexSize = geom->VertexBufferPointer->VertexCount * geom->VertexBufferPointer->VertexStride;
            const uint8_t* vertexPointer = &file.data[geom->VertexBufferPointer->DataPointer1];

            int indicesSize = geom->IndexBufferPointer->IndicesCount * sizeof(uint16_t);
            const uint8_t* indicesPointer = &file.data[geom->IndexBufferPointer->IndicesPointer];

            VertexBufferHandle vertexHandle = renderer->createVertexBuffer(vertexSize, vertexPointer);

            IndexBufferHandle indexHandle = renderer->createIndexBuffer(indicesSize, indicesPointer);

            short test = drawable->DrawableModels[0]->Get(i)->ShaderMappingPointer[j];

            Geometry geometry(vertexHandle, indexHandle, geom->IndexBufferPointer->IndicesCount);
            geometry.type = (VertexType)geom->VertexBufferPointer->InfoPointer->Flags;
            models[i].geometries.push_back(geometry);

            /*models[i].geometries.emplace_back(file.data,
                                              drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j),
                                              (*drawable->DrawableModels[0]->Get(i)->ShaderMappingPointer)[j], drawable->ShaderGroupPointer->Shaders[(*drawable->DrawableModels[0]->Get(i)->ShaderMappingPointer)[j]]->FileName);*/
        }
    }
}

YdrLoader::~YdrLoader()
{
    if (ytd)
        GlobalPool::GetInstance()->ytdPool.remove(ytd);

    if (renderer)
    {
        for (auto& model : models)
        {
            for (auto& geom : model.geometries)
            {
                renderer->removeVertexBuffer(geom.getVertexBufferHandle());
                renderer->removeIndexbuffer(geom.getIndexBufferHandle());
            }
        }
    }
}
