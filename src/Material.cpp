#include "Material.h"

Material::Material(uint32_t DiffuseHash, uint32_t BumpHash, uint32_t SpecularHash, uint32_t DetailHash)
{
	/*diffuseTextureID = TextureManager::GetTextureManager().GetTexture(DiffuseHash);
	if (BumpHash != 0) {
	 bumpTextureID = TextureManager::GetTextureManager().GetTexture(BumpHash);
	 useBump = true;
	}
	if (SpecularHash != 0)
	 specularTextureID = TextureManager::GetTextureManager().GetTexture(SpecularHash);
	if (DetailHash != 0)
	 detailTextureID = TextureManager::GetTextureManager().GetTexture(SpecularHash);*/
}

Material::~Material()
{
}
