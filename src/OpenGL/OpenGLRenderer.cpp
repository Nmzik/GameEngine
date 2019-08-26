#include "Shader.h"
#include "../CBuilding.h"
#include "../Camera.h"
#include "../Model.h"
//#include "../YdrLoader.h"
#include "OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer(NativeWindow* window)
    : nativeWindow(window)
{
    nativeWindow->initializeContext();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_MULTISAMPLE);
    glDisable(GL_DITHER);

    glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);

    mainShader = new Shader("assets/shaders/gbuffer");

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &uboGlobal);
    glBindBuffer(GL_UNIFORM_BUFFER, uboGlobal);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &uboModel);
    glBindBuffer(GL_UNIFORM_BUFFER, uboModel);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //Default texture
    glGenTextures(1, &textures[0]);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    float pixels1[] = {0.85f, 0.79f, 0.79f, 0.85f, 0.79f, 0.79f, 0.85f, 0.79f, 0.79f, 0.85f, 0.79f, 0.79f};
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels1);
}

OpenGLRenderer::~OpenGLRenderer()
{
}

void OpenGLRenderer::beginFrame()
{
}

void OpenGLRenderer::endFrame()
{
}

void OpenGLRenderer::presentFrame()
{
    nativeWindow->swapBuffers();
}

VertexBufferHandle OpenGLRenderer::createVertexBuffer(uint32_t size, const uint8_t* pointer)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, size, pointer, GL_STATIC_DRAW);

    VertexBufferHandle handle;

    //find free
    BOOL found = false;
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

IndexBufferHandle OpenGLRenderer::createIndexBuffer(uint32_t size, const uint8_t* pointer)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, pointer, GL_STATIC_DRAW);

    IndexBufferHandle handle;

    //find free
    BOOL found = false;
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

TextureHandle OpenGLRenderer::createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format)
{
    unsigned int InternalFormat;
    unsigned int type;
    bool compressed = false;

    switch (format)
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
        default:
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

    //	QUALITY
    uint8_t TextureQuality = 0;  //	Which is the base mipmap texture to load
    /*bool Ultra = true;
	if (Ultra)
	 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);*/

    //	0 = High Res
    //	1 = Half Res
    //	2 = Quarter Res
    //	UGLY
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
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);*/

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

void OpenGLRenderer::removeVertexBuffer(VertexBufferHandle handle)
{
    glBindBuffer(GL_VERTEX_ARRAY, 0);
    glDeleteBuffers(1, &vertexBuffers[handle.id]);
}

void OpenGLRenderer::removeIndexbuffer(IndexBufferHandle handle)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &indexBuffers[handle.id]);
}

void OpenGLRenderer::removeTexture(TextureHandle handle)
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

//#define GL_DEBUG

#ifdef GL_DEBUG
#define GL_CHECK(call)                              \
    call;                                           \
    {                                               \
        GLenum err = glGetError();                  \
        if (err != 0)                               \
        {                                           \
            printf("GL error 0x%x %u\n", err, err); \
            DebugBreak();                           \
        }                                           \
    }
#else
#define GL_CHECK(call) call
#endif

void OpenGLRenderer::renderDrawable(YdrLoader* drawable)
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
                case VertexType::Default:
                    layout = DefaultAttrib;
                    break;
                default:
                    continue;
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

void OpenGLRenderer::renderBuilding(CBuilding* building)
{
    renderDrawable(building->ydr);
}

void OpenGLRenderer::renderPed(CPed* ped)
{
    for (auto& ydr : ped->playerModel)
    {
        //renderDrawable(ydr);
    }
}

void OpenGLRenderer::renderVehicle(CVehicle* vehicle)
{
    renderDrawable(vehicle->getDrawable()->ydr);
}

void OpenGLRenderer::renderWorld(GameWorld* world, Camera* curCamera)
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
    }*/

    endFrame();

    presentFrame();
}