#include "TextureManager.h"

bool TextureManager::testvar = false;
std::unordered_map<uint32_t, GLuint> TextureManager::TexturesMap;

void TextureManager::ChangeBool()
{
	testvar = !testvar;
}

GLuint TextureManager::GetTexture(uint32_t textureHash)
{
	std::unordered_map<uint32_t, GLuint>::iterator it;
	it = TexturesMap.find(textureHash);
	if (it != TexturesMap.end())
	{
		std::cout << "FOUND Texture " << it->first << std::endl;
		auto& element = it->second;

		return element;
	}
	else {
		it = TexturesMap.find(0);
		return it->second;
	}
}

void TextureManager::LoadTexture(uint32_t Hash, GLuint TextureID)
{
	TexturesMap[Hash] = TextureID;
}

GLuint TextureManager::loadTexture(std::string path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		//delete[] data;
	}
	else
	{
		printf("Texture failed to load at path : %s", path);
		stbi_image_free(data);
	}

	return textureID;
}