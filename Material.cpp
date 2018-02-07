#include "Material.h"

Material::Material(const char* diffuseTexture, const char* specTexture, bool UseSametextureID, GLuint TextureID)
{
	//if (UseSametextureID)
	//{
		//diffuseTextureID = TextureID;
	//}
	//else {
		if (diffuseTexture != 0)
			diffuseTextureID = TextureManager::SearchTexture(diffuseTexture);
		else
			diffuseTextureID = TextureManager::SearchTexture("blank.jpg");
		if (specTexture != 0)
			specularTextureID = TextureManager::SearchTexture(specTexture);
	//}

}

Material::Material(const char* diffuseTexture, const char* specTexture)
{
	if (diffuseTexture != 0)
		diffuseTextureID = TextureManager::SearchTexture(diffuseTexture);
	else
		diffuseTextureID = TextureManager::SearchTexture("blank.jpg");
	if (specTexture != 0)
		specularTextureID = TextureManager::SearchTexture(specTexture);
}

Material::~Material()
{
}

void Material::bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTextureID);
	//set UseNormalMap/use specularmap int values in shaders
	if (specularTextureID != 0) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTextureID);
	}
}
