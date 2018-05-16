#include "Material.h"

Material::Material(uint32_t DiffuseHash, uint32_t BumpHash, uint32_t SpecularHash, uint32_t DetailHash)
{
	diffuseTextureID = TextureManager::GetTexture(DiffuseHash);
	if (BumpHash != 0)
		bumpTextureID = TextureManager::GetTexture(BumpHash);
	if (SpecularHash != 0)
		specularTextureID = TextureManager::GetTexture(SpecularHash);
	if (DetailHash != 0)
		detailTextureID = TextureManager::GetTexture(SpecularHash);
}

Material::~Material()
{
}
