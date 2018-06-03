#include "TextureManager.h"

std::vector<GLuint> TextureManager::TexturesID;
std::unordered_map<uint32_t, TextureManager::Texture> TextureManager::TexturesMap;

void TextureManager::Initialize()
{
	TexturesID.resize(30000);
	glGenTextures(30000, &TexturesID[0]);
}

GLuint TextureManager::GetTexture(uint32_t textureHash)
{
	std::unordered_map<uint32_t, Texture>::iterator it = TexturesMap.find(textureHash);
	if (it != TexturesMap.end())
	{
		return it->second.TextureID;
	}
	else if (textureHash == 1551155749){
		it = TexturesMap.find(0);
		return it->second.TextureID;
	}
	else {
		it = TexturesMap.find(0);
		return it->second.TextureID;
	}
}

void TextureManager::LoadTexture(uint32_t Hash, GLuint TextureID)
{
	/*std::unordered_map<uint32_t, GLuint>::iterator it = TexturesMap.find(Hash);
	if (it != TexturesMap.end())
	{
		printf("HERE");
	}*/
	TexturesMap[Hash] = Texture{TextureID, 1};
	//TexturesMap.insert(std::pair<uint32_t, Texture>(Hash, Texture{ TextureID, 0 }));

}

void TextureManager::RemoveTexture(uint32_t Hash)
{
	std::unordered_map<uint32_t, Texture>::iterator it = TexturesMap.find(Hash);
	if (it != TexturesMap.end())
	{
		it->second.referenceCount--;
		if (it->second.referenceCount == 0) {
			TexturesID.push_back(it->second.TextureID);
			TexturesMap.erase(it);
		}
	}
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
		printf("Texture failed to load at path : %s", path.c_str());
		stbi_image_free(data);
	}

	return textureID;
}
