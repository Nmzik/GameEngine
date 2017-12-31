#include "Model.h"

Model::Model(glm::vec3 position, glm::quat rot, std::vector<float> vertices, std::vector<unsigned int> indices, std::vector<float> normals, std::vector<float> texcoords, char const * pathTexture, bool dynamic = false, bool GenerateCollision = true)
{
	this->position = position;

	//printf("KOLVO VERTICES %d", (shapes[0].mesh.num_vertices));
	//printf("KOLVO INDICES %d", shapes[0].mesh.indices.size());

	if (GenerateCollision) {

		btConvexHullShape *shape = new btConvexHullShape();

		for (size_t i = 0; i < indices.size() / 3; ++i) {
			// index 1
			auto idx1 = indices[i * 3];
			btVector3 v0(
				vertices[(idx1 * 3)],
				vertices[(idx1 * 3) + 1],
				vertices[(idx1 * 3) + 2]
			);
			// index 2
			auto idx2 = indices[(i * 3) + 1];
			btVector3 v1(
				vertices[(idx2 * 3)],
				vertices[(idx2 * 3) + 1],
				vertices[(idx2 * 3) + 2]
			);
			// index 3
			auto idx3 = indices[(i * 3) + 2];
			btVector3 v2(
				vertices[(idx3 * 3)],
				vertices[(idx3 * 3) + 1],
				vertices[(idx3 * 3) + 2]
			);

			shape->addPoint(v0);
			shape->addPoint(v1);
			shape->addPoint(v2);
		}

			/*for (int i = 0; i < vertices.size() / 8; i++)
			{
				btVector3 newVector(vertices[i * 8], vertices[i * 8 + 1], vertices[i * 8 + 2]);
				shape->addPoint(newVector);
			}*/

			btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(rot.w, rot.x, rot.y, rot.z), btVector3(position.x, position.y, position.z)));
		if (dynamic) {
			btScalar mass = 1;
			btVector3 fallInertia(0, 0, 0);
			shape->calculateLocalInertia(mass, fallInertia);
			btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, MotionState, shape, fallInertia);
			rigidBody = new btRigidBody(fallRigidBodyCI);
		}
		else {
			btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, MotionState, shape, btVector3(0, 0, 0));
			rigidBody = new btRigidBody(groundRigidBodyCI);
		};
	}

	meshes.push_back(Mesh(vertices, indices, normals, texcoords, pathTexture));
}


Model::~Model()
{
}

btRigidBody* Model::getBody() {
	return rigidBody;
}

unsigned int Model::loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		printf("Texture failed to load at path : %s", path);
		stbi_image_free(data);
	}

	return textureID;
}

glm::mat4 Model::GetMat4()
{
	glm::mat4 model(1.0);
	if (rigidBody == NULL)
	{
		return model;
	}
	rigidBody->getWorldTransform().getOpenGLMatrix(&model[0][0]);
	return model;
}

glm::vec3 Model::GetPosition()
{
	return position;
}

void Model::Draw()
{
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw();
	}
}
