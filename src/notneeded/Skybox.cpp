#include "Skybox.h"

Skybox::Skybox()
{
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    //	glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.positions.size() * sizeof(float), &shapes[0].mesh.positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &normal_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    //	glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.normals.size() * sizeof(float), &shapes[0].mesh.normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &tex_coord_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer);
    //	glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.texcoords.size() * sizeof(float), &shapes[0].mesh.texcoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    //	num_indices = shapes[0].mesh.indices.size();

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[0].mesh.indices.size() * sizeof(unsigned int), &shapes[0].mesh.indices[0], GL_STATIC_DRAW);

    //	cubemapTexture = loadCubemap(faces);*/
}

Skybox::~Skybox()
{
}

void Skybox::Draw()
{
    //	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    /*SkyboxShader->use();
	//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
	glm::mat4 view = glm::mat4(glm::mat3(glm::lookAt(glm::vec3(0, 0.0, -10), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0))));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 100.0f);
	SkyboxShader->setMat4("view", view);
	SkyboxShader->setMat4("projection", projection);
	SkyboxShader->setMat4("model", glm::mat4(1.0));
	// skybox cube
	glBindVertexArray(VAO);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//glDepthFunc(GL_LESS); // set depth function back to default*/
}