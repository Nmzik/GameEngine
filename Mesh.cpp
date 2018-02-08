#include "Mesh.h"

Mesh::Mesh(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals)
{
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	num_vertices = vertices.size();

	material = new Material(0, 0);
}

Mesh::Mesh(std::vector<float>& vertices, std::vector<uint16_t>& indices, std::vector<float>& normals, std::vector<float>& texcoords, char const * pathTexture, char const * specTexture)
{
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &tex_coord_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer);
	glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(float), &texcoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	num_indices = indices.size();

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t), &indices[0], GL_STATIC_DRAW); //16 BIT INDICES max 65536

	material = new Material(pathTexture, specTexture);
}

Mesh::Mesh(std::vector<uint8_t>& vertexData, std::vector<uint16_t>& indices, uint16_t VertexStride)
{
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(uint8_t), &vertexData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexStride, nullptr);
	glEnableVertexAttribArray(1); //NORMALS
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexStride, (GLvoid*)12);
	glEnableVertexAttribArray(2); //TEXTCOORD
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VertexStride, (GLvoid*)24);

	num_indices = indices.size();

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t), &indices[0], GL_STATIC_DRAW); //16 BIT INDICES max 65536

	material = new Material(0, 0, true, 0);

}


Mesh::~Mesh()
{
	//delete material;
	//glDeleteVertexArrays(1, &VAO);
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