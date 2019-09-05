#include "VulkanRenderer.h"
#include "../CBuilding.h"
#include "../Camera.h"
#include "../Model.h"
#include "../NativeWindow.h"

#if (VULKAN_DEBUG)
#define VK_CHECK(name)                \
    VKResult res = name;              \
    if (name != VkResult::VK_SUCCESS) \
    {                                 \
        printf("ERROR\n");            \
        DebugBreak();                 \
    }
#else
#define VK_CHECK(name) name;
#endif

VulkanRenderer::VulkanRenderer(NativeWindow* window)
    : nativeWindow(window)
{
    nativeWindow->initializeContext();

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "GameEngine";
    appInfo.engineVersion = 1;
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instInfo = {};
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pApplicationInfo = &appInfo;

    VK_CHECK(vkCreateInstance(&instInfo, NULL, &inst));

    uint32_t numDevices;
    VK_CHECK(vkEnumeratePhysicalDevices(inst, &numDevices, NULL));
}

VulkanRenderer::~VulkanRenderer()
{
}

/*void VulkanRenderer::beginFrame()
{
}

void VulkanRenderer::endFrame()
{
}

void VulkanRenderer::presentFrame()
{
    nativeWindow->swapBuffers();
}

VertexBufferHandle VulkanRenderer::createVertexBuffer(uint32_t size, const uint8_t* pointer)
{
    /*GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, size, pointer, GL_STATIC_DRAW);
	
    VertexBufferHandle handle;

    //find free
    /*BOOL found = false;
    for (int i = 0; i < gpuBufferSize; i++)
    {
        if (vertexBuffers[i] == 0)
        {
            vertexBuffers[i] = vbo;
            handle.id = i;
            found = true;
            break;
        }
    }

    assert(found);

    return handle;
}

IndexBufferHandle VulkanRenderer::createIndexBuffer(uint32_t size, const uint8_t* pointer)
{
    IndexBufferHandle handle;

    //find free
    bool found = false;
    for (int i = 0; i < gpuBufferSize; i++)
    {
        if (indexBuffers[i] == 0)
        {
            indexBuffers[i] = vbo;
            handle.id = i;
            found = true;
            break;
        }
    }

    assert(found);

    return handle;
}

TextureHandle VulkanRenderer::createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format)
{
    unsigned int InternalFormat;
    unsigned int type;
    bool compressed = false;

    /*switch (format)
    {
        case D3DFMT_DXT1:
            compressed = true;
            InternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case D3DFMT_DXT3:
            compressed = true;
            InternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case D3DFMT_DXT5:
            compressed = true;
            InternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        case D3DFMT_BC7:
            compressed = true;
            InternalFormat = GL_COMPRESSED_RGBA_BPTC_UNORM;
            break;
        case D3DFMT_ATI1:
            compressed = true;
            InternalFormat = GL_COMPRESSED_RED_RGTC1;
            break;
        case D3DFMT_ATI2:
            compressed = true;
            InternalFormat = GL_COMPRESSED_RG_RGTC2;
            break;
        case D3DFMT_A8R8G8B8:
            InternalFormat = GL_BGRA;
            type = GL_UNSIGNED_INT_8_8_8_8_REV;
            break;
        /*case D3DFMT_A1R5G5B5:  // NOTHING???!!!
            InternalFormat = GL_BGRA;
            type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
            break;
        case D3DFMT_A8:  // COLOR BLACK ALWAYS?
            //return;
            InternalFormat = GL_RED;
            type = GL_UNSIGNED_BYTE;
            break;
        case D3DFMT_A8B8G8R8:  // CANT Check!
            //return;
            InternalFormat = GL_RGBA;
            type = GL_UNSIGNED_INT_8_8_8_8_REV;
            break;
        case D3DFMT_L8:  // FONTS?
            //return;
            InternalFormat = GL_RED;
            type = GL_UNSIGNED_BYTE;
            break;*/
/* default:
            return TextureHandle{0};
            printf("UNSUPPORTED FORMAT\n");
            break;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexStorage2D(GL_TEXTURE_2D, levels, InternalFormat, width, height);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, texture->Levels - 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, levels <= 1 ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //	TextureQuality = texture->Levels < TextureQuality ? 0: TextureQuality;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, TextureQuality);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, levels);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 0);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 12);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    /*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (compressed)
    {
        unsigned int blockSize = (InternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || InternalFormat == GL_COMPRESSED_RED_RGTC1) ? 8 : 16;

        for (int level = 0; level < levels; ++level)
        {
            unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

            if (level >= TextureQuality)
                glCompressedTexSubImage2D(
                    GL_TEXTURE_2D, level, 0, 0, width, height, InternalFormat, size, pointer);

            pointer += size;
            width /= 2;
            height /= 2;
        }
    }
    else
    {
        for (unsigned int level = 0; level < levels; ++level)
        {
            unsigned int size = ((width + 1) >> 1) * ((height + 1) >> 1) * 4;

            if (level >= TextureQuality)
                glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height, InternalFormat, type, pointer);

            pointer += size;
            width /= 2;
            height /= 2;
        }
    }

    TextureHandle handle;

    //find free
    BOOL found = false;
    for (int i = 0; i < gpuBufferSize; i++)
    {
        if (textures[i] == 0)
        {
            textures[i] = textureID;
            handle.id = i;
            found = true;
            break;
        }
    }

    assert(found);

    return handle;
}

void VulkanRenderer::removeVertexBuffer(VertexBufferHandle handle)
{
    /*glBindBuffer(GL_VERTEX_ARRAY, 0);
    glDeleteBuffers(1, &vertexBuffers[handle.id]);
}

void VulkanRenderer::removeIndexbuffer(IndexBufferHandle handle)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &indexBuffers[handle.id]);
}

void VulkanRenderer::removeTexture(TextureHandle handle)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &textures[handle.id]);
}

static const GLenum s_attribType[] =
    {
        GL_UNSIGNED_BYTE,  // Uint8
        GL_HALF_FLOAT,     // Half
        GL_FLOAT,          // Float
};

void VulkanRenderer::renderDrawable(YdrLoader* drawable)
{
    int curTexture = 0;
    for (auto& model : drawable->models)
    {
        if ((model.Unk_2Ch & 1) == 0)
        {
            continue;  //	PROXIES
        }
        for (auto& geometry : model.geometries)
        {
            mainShader->use();

            GL_CHECK(glBindVertexArray(VAO));

            GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[geometry.getVertexBufferHandle().id]));
            GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[geometry.getIndexBufferHandle().id]));

            VertexLayout layout;

            switch (geometry.type)
            {
                case Default:
                    layout = DefaultAttrib;
                    break;
                case DefaultEx:
                    layout = DefaultExAttrib;
                    break;
                case PNCCT:
                    layout = PNCCTAttrib;
                    break;
                case PNCCTTTT:
                    layout = PNCCTTTTAttrib;
                    break;
                case PCCNCCTTX:
                    layout = PCCNCCTTXAttrib;
                    break;
                case PCCNCCT:
                    layout = PCCNCCTAttrib;
                    break;
                case PNCTTTX:
                    layout = PNCTTTXAttrib;
                    break;
                case PNCTTX:
                    layout = PNCTTXAttrib;
                    break;
                case PNCTTTX_2:
                    layout = PNCTTTX_2Attrib;
                    break;
                case PNCTTTX_3:
                    layout = PNCTTTX_3Attrib;
                    break;
                case PNCCTTX:
                    layout = PNCCTTXAttrib;
                    break;
                case PNCCTTX_2:
                    layout = PNCCTTX_2Attrib;
                    break;
                case PNCCTTTX:
                    layout = PNCCTTTXAttrib;
                    break;
                case PCCNCCTX:
                    layout = PCCNCCTXAttrib;
                    break;
                case PCCNCTX:
                    layout = PCCNCTXAttrib;
                    break;
                case PCCNCT:
                    layout = PCCNCTAttrib;
                    break;
                case PNCCTT:
                    layout = PNCCTTAttrib;
                    break;
                case PNCCTX:
                    layout = PNCCTXAttrib;
                    break;
                case PCT:
                    layout = PCTAttrib;
                    break;
                case PT:
                    layout = PTAttrib;
                    break;
                case PTT:
                    layout = PTTAttrib;
                    break;
                case PNC:
                    layout = PNCAttrib;
                    break;
                case PC:
                    layout = PCAttrib;
                    break;
                case PCC:
                    layout = PCCAttrib;
                    break;
                case PCCH2H4:
                    layout = PCCH2H4Attrib;
                    break;
                case PNCH2:
                    layout = PNCH2Attrib;
                    break;
                case PNCTTTTX:
                    layout = PNCTTTTXAttrib;
                    break;
                /*case PNCTTTT:
                    layout = PNCTTTTAttrib;
                    break;
                case PCCNCCTT:
                    layout = PCCNCCTTAttrib;
                    break;
                case PCTT:
                    layout = PCTTAttrib;
                    break;
                case PCCCCT:
                    layout = PCCCCTAttrib;
                    break;
                case PCCNC:
                    layout = PCCNCAttrib;
                    break;
                case PCCNCTT:
                    layout = PCCNCTTAttrib;
                    break;
                case PCCNCTTX:
                    layout = PCCNCTTXAttrib;
                    break;
                case PCCNCTTT:
                    layout = PCCNCTTTAttrib;
                    break;
                /*case PNCTT:
                    layout = PNCTTAttrib;
                    break;
                case PNCTTT:
                    layout = PNCTTTAttrib;
                    break;
                default:
                    break;
            }

            for (int i = 0; i < layout.size; i++)
            {
                GL_CHECK(glEnableVertexAttribArray(layout.attributes[i].index));
                GL_CHECK(glVertexAttribPointer(layout.attributes[i].index, layout.attributes[i].size, s_attribType[(int)layout.attributes[i].type], GL_FALSE, layout.stride, (void*)(uintptr_t)layout.attributes[i].offset));
            }

            GL_CHECK(glActiveTexture(GL_TEXTURE0));
            if (geometry.getTextureHandle().id == 0)
                glBindTexture(GL_TEXTURE_2D, textures[0]);
            else
                glBindTexture(GL_TEXTURE_2D, textures[geometry.getTextureHandle().id]);

            GL_CHECK(glDrawElements(GL_TRIANGLES, geometry.getIndexCount(), GL_UNSIGNED_SHORT, 0));
        }
    }
}

void VulkanRenderer::renderBuilding(CBuilding* building)
{
    renderDrawable(building->ydr);
}

void VulkanRenderer::renderPed(CPed* ped)
{
    for (auto& ydr : ped->playerModel)
    {
        //renderDrawable(ydr);
    }
}

void VulkanRenderer::renderVehicle(CVehicle* vehicle)
{
    renderDrawable(vehicle->getDrawable()->ydr);
}

void VulkanRenderer::renderWorld(GameWorld* world, Camera* curCamera)
{
    beginFrame();

    glm::mat4 view = curCamera->getViewMatrix();
    glm::mat4 projection = curCamera->getProjection();
    glm::mat4 ProjectionView = projection * view;

    curCamera->updateFrustum(ProjectionView);

    GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, uboGlobal));
    GL_CHECK(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &ProjectionView[0]));

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    GL_CHECK(glEnable(GL_CULL_FACE));

    GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboGlobal));
    GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboModel));

    for (auto& object : world->renderList)
    {
        GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, uboModel));
        GL_CHECK(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &object->getMatrix()[0]));
        GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, 0));

        switch (object->getType())
        {
            case ObjectType::Building:
            {
                CBuilding* building = static_cast<CBuilding*>(object);
                renderBuilding(building);
                break;
            }
            case ObjectType::Vehicle:
            {
                CVehicle* vehicle = static_cast<CVehicle*>(object);
                renderVehicle(vehicle);
                break;
            }
            case ObjectType::Ped:
            {
                CPed* ped = static_cast<CPed*>(object);
                renderPed(ped);
                break;
            }
            default:
                break;
        }
    }

    GL_CHECK(glDisable(GL_CULL_FACE));

    /*if (RenderDebugWorld)
    {
        world->getPhysicsSystem()->getPhysicsWorld()->debugDrawWorld();
        glm::mat4 DefaultMatrix(1.0f);
        glBindBuffer(GL_UNIFORM_BUFFER, uboModel);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &DefaultMatrix[0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, defaultTexture);
        world->getPhysicsSystem()->debug.render();
    }

    endFrame();

    presentFrame();
}*/