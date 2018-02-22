#include "Mesh.h"

Mesh::Mesh(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals)
{
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	num_vertices = vertices.size();

	material = new Material(0);
}

Mesh::Mesh(const uint8_t* meshData, uint64_t VertexPointer, uint32_t VertexSize, uint64_t IndicesPointer, uint32_t IndicesSize, uint16_t VertexStride, uint32_t textureHash)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, &meshData[VertexPointer], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexStride, nullptr);
	glEnableVertexAttribArray(1); //NORMALS
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexStride, (GLvoid*)12);
	//printf("VERTEX %d\n", VertexStride);
	glEnableVertexAttribArray(2); //TEXTCOORD
	//if (VertexStride == 36) 
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VertexStride, (GLvoid*)28);
	if (VertexStride == 52) 
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VertexStride, (GLvoid*)36);
	if (VertexStride == 56)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VertexStride, (GLvoid*)32);
	/*if (VertexStride == 68)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VertexStride, (GLvoid*)28);
	if (VertexStride == 64)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VertexStride, (GLvoid*)32);
	if (VertexStride == 72)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VertexStride, (GLvoid*)40);*/

	num_indices = IndicesSize;

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesSize, &meshData[IndicesPointer], GL_STATIC_DRAW); //16 BIT INDICES max 65536

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	material = new Material(textureHash);

}


Mesh::~Mesh()
{
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &normal_buffer);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	delete material;
}

void Mesh::Draw()
{
	glBindVertexArray(VAO);
	material->bind();
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, 0);
}

void Mesh::DrawCollision()
{
	glBindVertexArray(VAO);
	material->bind();
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
}