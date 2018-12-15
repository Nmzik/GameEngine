#pragma once
#include "opengl.h"
#include "stdint.h"

struct grcTexture;
class memstream;

class Texture
{
	uint32_t TextureHash;
	GLuint textureID;
public:
	Texture(uint32_t Hash, grcTexture* texture, memstream& file);
	~Texture();

	Texture(const Texture&) = delete;
	Texture(Texture &&) = delete;
	Texture& operator=(const Texture &) = delete;
	Texture& operator=(Texture &&) = delete;

	GLuint getTextureID() const{
		return textureID;
	}
};
