#include "TextureManager.h"

TextureManager TextureManager::texManager;

GLuint TextureManager::GetTexture(uint32_t textureHash)
{
	std::unordered_map<uint32_t, Texture>::iterator it = TexturesMap.find(textureHash);
	if (it != TexturesMap.end())
	{
		return it->second.TextureID;
	}
	else
	{
		return DefaultTexture;
	}
}

void TextureManager::LoadTexture(uint32_t Hash, GLuint TextureID)
{
	TexturesMap.emplace(Hash, Texture{TextureID, 1});
}

void TextureManager::RemoveTexture(uint32_t Hash)
{
	std::unordered_map<uint32_t, Texture>::iterator it = TexturesMap.find(Hash);
	if (it != TexturesMap.end())
	{
		it->second.referenceCount--;
		if (it->second.referenceCount == 0)
		{
			glDeleteTextures(1, &it->second.TextureID);
			TexturesMap.erase(it);
		}
	}
}