#include "GLTexture.h"

#include "YtdLoader.h"
#include "membuf.h"

/*GLTexture::GLTexture(grcTexture* GLTexture, memstream& file, uint32_t Hash)
    : TextureHash(Hash)
{
    unsigned int InternalFormat;
    unsigned int format;
    unsigned int type;
    bool compressed = false;

    switch (GLTexture->Format)
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
            InternalFormat = GL_RGBA8;  // Working
            format = GL_BGRA;
            type = GL_UNSIGNED_INT_8_8_8_8_REV;
            break;
        case D3DFMT_A1R5G5B5:  // NOTHING???!!!
            InternalFormat = GL_RGBA;
            format = GL_BGRA;
            type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
            break;
        case D3DFMT_A8:  // COLOR BLACK ALWAYS?
            return;
            InternalFormat = GL_RED;
            format = GL_RED;
            type = GL_UNSIGNED_BYTE;
            break;
        case D3DFMT_A8B8G8R8:  // CANT Check!
            return;
            InternalFormat = GL_RGBA;
            format = GL_RGBA;
            type = GL_UNSIGNED_INT_8_8_8_8_REV;
            break;
        case D3DFMT_L8:  // FONTS?
            return;
            InternalFormat = GL_RED;
            format = GL_RED;
            type = GL_UNSIGNED_BYTE;
            break;
        default:
            printf("UNSUPPORTED FORMAT\n");
            break;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexStorage2D(GL_TEXTURE_2D, GLTexture->Levels, InternalFormat, GLTexture->Width, GLTexture->Height);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLTexture->Levels - 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLTexture->Levels <= 1 ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //	QUALITY
    uint8_t TextureQuality = 0;  //	Which is the base mipmap GLTexture to load
    /*bool Ultra = true;
	if (Ultra)
	 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);*/

    //	0 = High Res
    //	1 = Half Res
    //	2 = Quarter Res
    //	UGLY
    //	TextureQuality = GLTexture->Levels < TextureQuality ? 0: TextureQuality;

    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, TextureQuality);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLTexture->Levels);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 0);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 12);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    /*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);*/

    /*unsigned int offset = 0;

    if (compressed)
    {
        unsigned int blockSize = (InternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || InternalFormat == GL_COMPRESSED_RED_RGTC1) ? 8 : 16;

        for (unsigned int level = 0; level < GLTexture->Levels; ++level)
        {
            unsigned int size = ((GLTexture->Width + 3) / 4) * ((GLTexture->Height + 3) / 4) * blockSize;

            if (level >= TextureQuality)
                glCompressedTexSubImage2D(
                    GL_TEXTURE_2D, level, 0, 0, GLTexture->Width, GLTexture->Height, InternalFormat, size, &file.data[GLTexture->DataPointer] + offset);

            offset += size;
            GLTexture->Width /= 2;
            GLTexture->Height /= 2;
        }
    }
    else
    {
        for (unsigned int level = 0; level < GLTexture->Levels; ++level)
        {
            unsigned int size = ((GLTexture->Width + 1) >> 1) * ((GLTexture->Height + 1) >> 1) * 4;

            if (level >= TextureQuality)
                glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, GLTexture->Width, GLTexture->Height, format, type, &file.data[GLTexture->DataPointer] + offset);

            offset += size;
            GLTexture->Width /= 2;
            GLTexture->Height /= 2;
        }
    }
    /*std::unordered_map<uint32_t, TextureManager::GLTexture>::iterator it = TextureManager::GetTextureManager().TexturesMap.find(Hash);
    if (it == TextureManager::GetTextureManager().TexturesMap.end())
        TextureManager::GetTextureManager().LoadTexture(Hash, textureID);
    else
        it->second.referenceCount++;
}

GLTexture::~GLTexture()
{
    TextureManager::GetTextureManager().RemoveTexture(TextureHash);

    glDeleteTextures(1, &textureID);
}*/