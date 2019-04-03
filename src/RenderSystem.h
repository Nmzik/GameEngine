#pragma once

#include <memory>
#include "Buffer.h"
#include "CommandBuffer.h"
#include "RenderContext.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"

class RenderSystem
{
public:
    //RenderContext
    virtual RenderContext* CreateRenderContext(const RenderContextDescriptor& desc) = 0;
    virtual void Release(RenderContext& renderContext) = 0;

    //Command buffers
    virtual CommandBuffer* CreateCommandBuffer() = 0;
    virtual void Release(CommandBuffer& commandBuffer) = 0;

    //Buffers
    virtual Buffer* CreateBuffer(const BufferDescriptor& desc, const void* initialData = nullptr) = 0;
    virtual void Release(Buffer& buffer) = 0;

    //Textures
    virtual Texture* CreateTexture(const TextureDescriptor& textureDesc) = 0;
    virtual void Release(Texture& texture) = 0;

    /*//RenderPass
    virtual RenderPass* CreateRenderPass(const RenderPassDescriptor& desc) = 0;
    virtual void Release(RenderPass& renderPass) = 0;

    //Render Targets
    virtual RenderTarget* CreateRenderTarget(const RenderTargetDescriptor& desc) = 0;
    virtual void Release(RenderTarget& renderTarget) = 0;*/

    //Shader
    virtual Shader* CreateShader(const ShaderDescriptor& desc) = 0;
    virtual void Release(Shader& shader) = 0;

	//ShaderProgram
    virtual ShaderProgram* CreateShaderProgram(const ShaderProgramDescriptor& desc) = 0;
    virtual void Release(ShaderProgram& shaderProgram) = 0;
};