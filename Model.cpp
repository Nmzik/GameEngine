#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Model::Model(glm::vec3 position, std::vector<float> vertices, bool dynamic = false)
{
	this->position = position;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	//tinyobj::LoadObj(shapes, materials, err, "C:\\Users\\nmzik\\Desktop\\rungholt\\rungholt.obj", "C:\\Users\\nmzik\\Desktop\\rungholt\\", tinyobj::triangulation | tinyobj::calculate_normals);

	//printf("KOLVO VERTICES %d", (shapes[0].mesh.num_vertices));
	//printf("KOLVO INDICES %d", shapes[0].mesh.indices.size());

	btConvexHullShape *shape = new btConvexHullShape();
	for (int i = 0; i < vertices.size()/8; i++)
	{
		btVector3 newVector(vertices[i * 8], vertices[i * 8 + 1], vertices[i * 8 + 2]);
		shape->addPoint(newVector);
	}
	//btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	//btCollisionShape* boxShape = new btBoxShape(btVector3(1.f, 1.f, 1.f));

	btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(position.x, position.y, position.z)));
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

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(shapes[0].mesh.indices.size()), &shapes[0].mesh.in[0], GL_STATIC_DRAW);
}


Model::~Model()
{
}

btRigidBody* Model::getBody() {
	return rigidBody;
}

glm::mat4 Model::GetMat4()
{
	glm::mat4 model;
	rigidBody->getWorldTransform().getOpenGLMatrix(&model[0][0]);
	return model;
}

glm::vec3 Model::GetPosition()
{
	return position;
}

void Model::Draw()
{
	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	//for (int i = 0; i < meshes.size(); i++)
		//meshes[i].Draw();
}
