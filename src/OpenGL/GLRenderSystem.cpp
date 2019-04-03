#include "GLRenderSystem.h"

GLRenderSystem::~GLRenderSystem()
{
}

RenderContext* GLRenderSystem::CreateRenderContext(const RenderContextDescriptor& desc)
{
    std::unique_ptr<GLRenderContext> context = std::make_unique<GLRenderContext>(desc);
    //auto pointer = context.get();
    //RenderContexts.emplace(std::move(context));

    return context.get();
}

void GLRenderSystem::Release(RenderContext& renderContext)
{
}

CommandBuffer* GLRenderSystem::CreateCommandBuffer()
{
    return nullptr;
}

void GLRenderSystem::Release(CommandBuffer& commandBuffer)
{
}

Buffer* GLRenderSystem::CreateBuffer(const BufferDescriptor& desc, const void* initialData)
{
    return nullptr;
}

void GLRenderSystem::Release(Buffer& buffer)
{
}

Texture* GLRenderSystem::CreateTexture(const TextureDescriptor& textureDesc)
{
    return nullptr;
}

void GLRenderSystem::Release(Texture& texture)
{
}

Shader* GLRenderSystem::CreateShader(const ShaderDescriptor& desc)
{
    return nullptr;
}

void GLRenderSystem::Release(Shader& shader)
{
}

ShaderProgram* GLRenderSystem::CreateShaderProgram(const ShaderProgramDescriptor& desc)
{
    return nullptr;
}

void GLRenderSystem::Release(ShaderProgram& shaderProgram)
{
}
