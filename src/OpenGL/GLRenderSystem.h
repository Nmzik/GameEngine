#pragma once
#include <set>
#include "RenderSystem.h"

#include "GLRenderContext.h"

class GLRenderSystem : public RenderSystem
{
public:
    GLRenderSystem() = default;
    ~GLRenderSystem();

    //RenderContext
    virtual RenderContext* CreateRenderContext(const RenderContextDescriptor& desc) override;
    virtual void Release(RenderContext& renderContext) override;

    //Command buffers
    virtual CommandBuffer* CreateCommandBuffer() override;
    virtual void Release(CommandBuffer& commandBuffer) override;

    //Buffers
    virtual Buffer* CreateBuffer(const BufferDescriptor& desc, const void* initialData = nullptr) override;
    virtual void Release(Buffer& buffer) override;

    //Textures
    virtual Texture* CreateTexture(const TextureDescriptor& textureDesc) override;
    virtual void Release(Texture& texture) override;

    //Shader
    virtual Shader* CreateShader(const ShaderDescriptor& desc) override;
    virtual void Release(Shader& shader) override;
    //ShaderProgram
    virtual ShaderProgram* CreateShaderProgram(const ShaderProgramDescriptor& desc) override;
    virtual void Release(ShaderProgram& shaderProgram) override;

	std::set<RenderContext*> RenderContexts;
    std::set<CommandBuffer*> commandBuffers;
    std::set<Buffer*> Buffers;
    std::set<Texture*> Textures;
    std::set<Shader*> Shaders;
};
