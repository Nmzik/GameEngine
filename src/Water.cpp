#include "Water.h"

Water::Water(WaterQuad waterQuad)
{
	float sx = waterQuad.maxX - waterQuad.minX;
	float sy = waterQuad.maxY - waterQuad.minY;


	std::vector <float> vertices = {
		waterQuad.minX, waterQuad.minY, waterQuad.z,  0.0f, 0.0f,
		waterQuad.maxX, waterQuad.minY, waterQuad.z,  sx, 0.0f,
		waterQuad.minX, waterQuad.maxY, waterQuad.z,  0.0f, sy,
		waterQuad.maxX, waterQuad.maxY, waterQuad.z,  sx, sy,
	};

	std::vector<uint16_t> indices;

	if (waterQuad.Type == 0) {
		num_indices = 6;

		indices.resize(num_indices);

		indices[0] = 0;
		indices[1] = 2;
		indices[2] = 1;
		indices[3] = 1;
		indices[4] = 2;
		indices[5] = 3;
	}
	else {
		num_indices = 3;

		indices.resize(num_indices);

		switch (waterQuad.Type)
		{
		case 1:
			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 2;
			break;
		case 2:
			indices[0] = 0;
			indices[1] = 3;
			indices[2] = 2;
			break;
		case 3:
			indices[0] = 1;
			indices[1] = 3;
			indices[2] = 2;
			break;
		case 4:
			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 3;
			break;
		default:
			break;
		}
	}

	BSCenter = glm::vec3((waterQuad.minX + waterQuad.maxX) * 0.5f, (waterQuad.minY + waterQuad.maxY) * 0.5f, waterQuad.z);
	BSRadius = glm::length(glm::vec2(waterQuad.maxX - waterQuad.minX, waterQuad.maxY - waterQuad.minY)) * 0.5f;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(2); //TEXCoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t), &indices[0], GL_STATIC_DRAW);

	//diffuseTextureID = TextureManager::GetTextureManager().GetTexture(185264090);
}

Water::~Water()
{

}

void Water::Draw()
{
	glBindVertexArray(VAO);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, diffuseTextureID);

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, 0);
}