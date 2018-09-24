#include "TextureManager.h"

TextureManager TextureManager::texManager;

GLuint TextureManager::GetTexture(uint32_t textureHash)
{
	std::unordered_map<uint32_t, Texture>::iterator it = TexturesMap.find(textureHash);
	if (it != TexturesMap.end())
	{
		return it->second.TextureID;
	}
	else {
		it = TexturesMap.find(475118591);
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
			glDeleteTextures(1, &it->second.TextureID);
			TexturesMap.erase(it);
		}
	}
}