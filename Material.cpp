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

void Material::bind(Shader* shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTextureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bumpTextureID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, specularTextureID);
}
