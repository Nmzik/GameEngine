#include "YdrLoader.h"

#include <algorithm>

#include "../GTAEncryption.h"

void YdrLoader::init(memstream& file)
{
    gtaDrawable* GTAdrawable = (gtaDrawable*)file.read(sizeof(gtaDrawable));
    GTAdrawable->Resolve(file);

    /*if (GTAdrawable->BoundPointer)
    {
        uint64_t origPos = file.tellg();
        file.seekg(GTAdrawable->BoundPointer);
         
        ybn = GlobalPool::GetInstance()->ybnPool.create();
        ybn->init(file);
        file.seekg(origPos);
    }*/
    drawable = (rmcDrawable*)GTAdrawable;
    //loadDrawable(GTAdrawable, _renderer, file);
}

void YdrLoader::finalize(BaseRenderer* _renderer, memstream& file)
{
    loadDrawable(drawable, false, _renderer, file);
}

void YdrLoader::loadDrawable(rmcDrawable* drawable, bool isYft, BaseRenderer* _renderer, memstream& file)
{
    renderer = _renderer;

    struct bonesData
    {
        Bone bone;
        Bone* parent;
    };

    if (*drawable->SkeletonPointer)
    {
        if (*drawable->SkeletonPointer->BonesPointer && *drawable->SkeletonPointer->ParentIndicesPointer)
        {
            //int maxcnt = std::min(drawable->SkeletonPointer->BonesCount, drawable->SkeletonPointer->in)
        }
    }

    //	READ COLLISION DATA FROM YDR

    //	Shader stuff
    if (*drawable->ShaderGroupPointer)
    {  //	IF POINTER = 0 NO OBJECTS???

        if (*drawable->ShaderGroupPointer->TextureDictionaryPointer)
        {
            ytd = GlobalPool::GetInstance()->ytdPool.create();

            TextureDictionary* textureDictionary = *drawable->ShaderGroupPointer->TextureDictionaryPointer;

            ytd->finalize(renderer, textureDictionary, file.systemSize);
        }

        for (int i = 0; i < drawable->ShaderGroupPointer->Shaders.size(); i++)
        {
            if (drawable->ShaderGroupPointer->Shaders.Get(i)->TextureParametersCount > 0)
            {
            }
        }
        /*
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
                }*/
    }

    if (isYft)
    {
    }
    else
    {
        gtaDrawable* GTAdrawable = (gtaDrawable*)drawable;

        if (*GTAdrawable->BoundPointer)
        {
            ybn = GlobalPool::GetInstance()->ybnPool.create();

            ybn->finalize(renderer, *GTAdrawable->BoundPointer, file);
        }
    }
    //////////

    models.resize(drawable->DrawableModels[0]->size());

    for (int i = 0; i < drawable->DrawableModels[0]->size(); i++)
    {
        models[i].Unk_2Ch = drawable->DrawableModels[0]->Get(i)->Unknown_2Ch;
        //	Optimization
        models[i].geometries.reserve(drawable->DrawableModels[0]->Get(i)->m_geometries.size());

        uint16_t* shaderMappings = drawable->DrawableModels[0]->Get(i)->getShaderMappings();

        for (int j = 0; j < drawable->DrawableModels[0]->Get(i)->m_geometries.size(); j++)
        {
            grmGeometry* geom = drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j);

            int vertexSize = geom->VertexBufferPointer->VertexCount * geom->VertexBufferPointer->VertexStride;
            const uint8_t* vertexPointer = *geom->VertexBufferPointer->DataPointer1;

            int indicesSize = geom->IndexBufferPointer->IndicesCount * sizeof(uint16_t);
            const uint8_t* indicesPointer = *geom->IndexBufferPointer->IndicesPointer;

            VertexBufferHandle vertexHandle = renderer->createVertexBuffer(vertexSize, vertexPointer);
            IndexBufferHandle indexHandle = renderer->createIndexBuffer(indicesSize, indicesPointer);

            uint32_t shaderIndex = shaderMappings[j];

            uint32_t diffuseHash = 0;

            uint32_t* shaderHashes = (uint32_t*)((uintptr_t) * (drawable->ShaderGroupPointer->Shaders.Get(shaderIndex)->parameters) + drawable->ShaderGroupPointer->Shaders.Get(shaderIndex)->m_parameterSize);

            grmShaderParameter* param = drawable->ShaderGroupPointer->Shaders.Get(shaderIndex)->getParameters();

            for (int k = 0; k < drawable->ShaderGroupPointer->Shaders.Get(shaderIndex)->TextureParametersCount; k++)
            {
                if (shaderHashes[k] == 4059966321 || shaderHashes[k] == 3576369631)
                {
                    assert(param[k].DataType == 0);

                    if (param[k].DataPointer == 0)
                    {
                        diffuseHash = 0;
                    }
                    else
                    {
                        SYSTEM_BASE_PTR(param[k].DataPointer);
                        TextureBase* texBase = (TextureBase*)&file.data[param[k].DataPointer];

                        SYSTEM_BASE_PTR(texBase->NamePointer);
                        char* TextureName = (char*)&file.data[texBase->NamePointer];

                        size_t NameLength = strlen(TextureName);

                        for (int i = 0; i < NameLength; i++)
                        {
                            TextureName[i] = tolower(TextureName[i]);
                        }

                        diffuseHash = GenHash(std::string_view(TextureName, NameLength));
                    }
                }
            }

            TextureHandle texHandle = renderer->getTextureManager()->getTextureHandle(diffuseHash);
            uint32_t vertexLayoutHandle = renderer->getLayoutHandle((VertexType)geom->VertexBufferPointer->InfoPointer->Flags);

            Geometry geometry(vertexHandle, indexHandle, vertexLayoutHandle, geom->IndexBufferPointer->IndicesCount, texHandle, diffuseHash);
            models[i].geometries.push_back(geometry);
        }
    }
}

YdrLoader::~YdrLoader()
{
    if (ytd)
        GlobalPool::GetInstance()->ytdPool.remove(ytd);

    if (ybn)
        GlobalPool::GetInstance()->ybnPool.remove(ybn);

    if (renderer)
    {
        for (auto& model : models)
        {
            for (auto& geom : model.geometries)
            {
                renderer->removeVertexBuffer(geom.getVertexBufferHandle());
                renderer->removeIndexbuffer(geom.getIndexBufferHandle());
                uint32_t textureHash = geom.getDiffuseHash();
                renderer->getTextureManager()->removeTextureHandle(textureHash);
                if (textureHash != 0 && geom.getTextureHandle().id != 0 && renderer->getTextureManager()->getRefCount(textureHash) == 0)
                {
                    renderer->removeTexture(geom.getTextureHandle());
				}
            }
        }
    }
}
