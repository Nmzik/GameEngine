#include "OpenGLRenderer.h"
#include "Shader.h"
#include "opengl.h"
#include "../CBuilding.h"
#include "../Camera.h"
#include "../Model.h"

OpenGLRenderer::OpenGLRenderer(NativeWindow* window)
    : nativeWindow(window)
{
    nativeWindow->initializeContext();

    OpenGL_Init();

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

    uint8_t data[8 * 8 * 4];
    for (int i = 0; i < 8 * 8; ++i)
    {
        data[4 * i] = (255);
        data[4 * i + 1] = (0);
        data[4 * i + 2] = (255);
        data[4 * i + 3] = (1);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    layouts.push_back(Default_Attrib);
    layoutHandles[Default] = 0;
    layouts.push_back(DefaultEx_Attrib);
    layoutHandles[DefaultEx] = 1;
    layouts.push_back(PNCCT_Attrib);
    layoutHandles[PNCCT] = 2;
    layouts.push_back(PNCCTTTT_Attrib);
    layoutHandles[PNCCTTTT] = 3;
    layouts.push_back(PBBNCCTTX_Attrib);
    layoutHandles[PBBNCCTTX] = 4;
    layouts.push_back(PBBNCCT_Attrib);
    layoutHandles[PBBNCCT] = 5;
    layouts.push_back(PNCTTTX_Attrib);
    layoutHandles[PNCTTTX] = 6;
    layouts.push_back(PNCTTX_Attrib);
    layoutHandles[PNCTTX] = 7;
    layouts.push_back(PNCTTTX_2_Attrib);
    layoutHandles[PNCTTTX_2] = 8;
    layouts.push_back(PNCTTTX_3_Attrib);
    layoutHandles[PNCTTTX_3] = 9;
    layouts.push_back(PNCCTTX_Attrib);
    layoutHandles[PNCCTTX] = 10;
    layouts.push_back(PNCCTTX_2_Attrib);
    layoutHandles[PNCCTTX_2] = 11;
    layouts.push_back(PNCCTTTX_Attrib);
    layoutHandles[PNCCTTTX] = 12;
    layouts.push_back(PBBNCCTX_Attrib);
    layoutHandles[PBBNCCTX] = 13;
    layouts.push_back(PBBNCTX_Attrib);
    layoutHandles[PBBNCTX] = 14;
    layouts.push_back(PBBNCT_Attrib);
    layoutHandles[PBBNCT] = 15;
    layouts.push_back(PNCCTT_Attrib);
    layoutHandles[PNCCTT] = 16;
    layouts.push_back(PNCCTX_Attrib);
    layoutHandles[PNCCTX] = 17;
    layouts.push_back(PCT_Attrib);
    layoutHandles[PCT] = 18;
    layouts.push_back(PT_Attrib);
    layoutHandles[PT] = 19;
    layouts.push_back(PTT_Attrib);
    layoutHandles[PTT] = 20;
    layouts.push_back(PNC_Attrib);
    layoutHandles[PNC] = 21;
    layouts.push_back(PC_Attrib);
    layoutHandles[PC] = 22;
    layouts.push_back(PCC_Attrib);
    layoutHandles[PCC] = 23;
    layouts.push_back(PCCH2H4_Attrib);
    layoutHandles[PCCH2H4] = 24;
    layouts.push_back(PNCH2_Attrib);
    layoutHandles[PNCH2] = 25;
    layouts.push_back(PNCTTTTX_Attrib);
    layoutHandles[PNCTTTTX] = 26;
    layouts.push_back(PNCTTTT_Attrib);
    layoutHandles[PNCTTTT] = 27;
    layouts.push_back(PBBNCCTT_Attrib);
    layoutHandles[PBBNCCTT] = 28;
    /*case PCTT:
                    layout = PCTT_Attrib;
                    break;
                case PBBCCT:
                    layout = PBBCCT_Attrib;
                    break;
                case PBBNC:
                    layout = PBBNC_Attrib;
                    break;*/
    layouts.push_back(PBBNCTT_Attrib);
    layoutHandles[PBBNCTT] = 29;
    layouts.push_back(PBBNCTTX_Attrib);
    layoutHandles[PBBNCTTX] = 30;
    layouts.push_back(PBBNCTTT_Attrib);
    layoutHandles[PBBNCTTT] = 31;
    /*case PNCTT:
                    layout = PNCTT_Attrib;
                    break;
                case PNCTTT:
                    layout = PNCTTT_Attrib;
                    break;
                case PBBNCTTTX:
                    layout = PBBNCTTTX_Attrib;
                    break;*/
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
    bool found = false;
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
    bool found = false;
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

uint32_t OpenGLRenderer::getLayoutHandle(VertexType type)
{
    auto iter = layoutHandles.find(type);
    if (iter != layoutHandles.end())
    {
        return iter->second;
    }
    else
    {
        printf("NOT FOUND\n");
        return 0;
    }
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

void OpenGLRenderer::updateGlobalSceneBuffer(glm::mat4& Projection, glm::mat4& View)
{
    GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, uboGlobal));
    glm::mat4 ProjectionView = Projection * View;
    GL_CHECK(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &ProjectionView[0]));

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    //need to fix this
    GL_CHECK(glEnable(GL_CULL_FACE));

    GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboGlobal));
    GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboModel));
}

void OpenGLRenderer::updatePerModelData(glm::mat4& mat)
{
    GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, uboModel));
    GL_CHECK(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &mat[0]));
    GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

static const GLenum s_attribType[] =
    {
        GL_UNSIGNED_BYTE,  // Uint8
        GL_HALF_FLOAT,     // Half
        GL_FLOAT,          // Float
};

void OpenGLRenderer::renderGeom(Geometry& geom)
{
    mainShader->use();

    GL_CHECK(glBindVertexArray(VAO));

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[geom.getVertexBufferHandle().id]));
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[geom.getIndexBufferHandle().id]));

    VertexLayout& layout = layouts[geom.getVertexLayoutHandle()];

    for (int i = 0; i < layout.size; i++)
    {
        GL_CHECK(glEnableVertexAttribArray(layout.attributes[i].index));
        GL_CHECK(glVertexAttribPointer(layout.attributes[i].index, layout.attributes[i].size, s_attribType[(int)layout.attributes[i].type], GL_FALSE, layout.stride, (void*)(uintptr_t)layout.attributes[i].offset));
    }

    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    if (geom.getTextureHandle().id == 0)
        glBindTexture(GL_TEXTURE_2D, textures[0]);
    else
        glBindTexture(GL_TEXTURE_2D, textures[geom.getTextureHandle().id]);

    GL_CHECK(glDrawElements(GL_TRIANGLES, geom.getIndexCount(), GL_UNSIGNED_SHORT, 0));
}