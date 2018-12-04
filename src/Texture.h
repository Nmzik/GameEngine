#pragma once
#include "opengl.h"

struct grcTexture;
class memstream;

class Texture
{
	GLuint textureID;
public:
	Texture(grcTexture* texture, memstream& file);
	~Texture();

	Texture(const Texture&) = delete;
	Texture(Texture &&) = delete;
	Texture& operator=(const Texture &) = delete;
	Texture& operator=(Texture &&) = delete;
};
