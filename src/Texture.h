#pragma once
#include "opengl.h"
#include "stdint.h"

struct grcTexture;
class memstream;

class Texture
{
	GLuint textureID;
	uint32_t TextureHash;
public:
	Texture(grcTexture* texture, memstream& file, uint32_t TextureHash);
	~Texture();

	Texture(const Texture&) = delete;
	Texture(Texture &&) = delete;
	Texture& operator=(const Texture &) = delete;
	Texture& operator=(Texture &&) = delete;

	GLuint getTextureID() const{
		return textureID;
	}
};
