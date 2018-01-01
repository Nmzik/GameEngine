#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Model::Model(glm::vec3 position, glm::quat rot, char const * ModelPath, char const * pathTexture, bool dynamic = false, bool GenerateCollision = true)
{
	this->position = position;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	tinyobj::LoadObj(shapes, materials, err, ModelPath, "C:\\Users\\nmzik\\Desktop\\rungholt\\", tinyobj::triangulation | tinyobj::calculate_normals);
	//printf("KOLVO VERTICES %d", (shapes[0].mesh.num_vertices));
	//printf("KOLVO INDICES %d", shapes[0].mesh.indices.size());

	if (GenerateCollision) {

		btConvexHullShape *shape = new btConvexHullShape();

		for (int i = 0; i < shapes.size(); i++) {
			for (size_t j = 0; j < shapes[i].mesh.indices.size() / 3; ++j) {
				// index 1
				auto idx1 = shapes[i].mesh.indices[j * 3];
				btVector3 v0(
					shapes[i].mesh.positions[(idx1 * 3)],
					shapes[i].mesh.positions[(idx1 * 3) + 1],
					shapes[i].mesh.positions[(idx1 * 3) + 2]
				);
				// index 2
				auto idx2 = shapes[i].mesh.indices[(j * 3) + 1];
				btVector3 v1(
					shapes[i].mesh.positions[(idx2 * 3)],
					shapes[i].mesh.positions[(idx2 * 3) + 1],
					shapes[i].mesh.positions[(idx2 * 3) + 2]
				);
				// index 3
				auto idx3 = shapes[i].mesh.indices[(j * 3) + 2];
				btVector3 v2(
					shapes[i].mesh.positions[(idx3 * 3)],
					shapes[i].mesh.positions[(idx3 * 3) + 1],
					shapes[i].mesh.positions[(idx3 * 3) + 2]
				);

				shape->addPoint(v0);
				shape->addPoint(v1);
				shape->addPoint(v2);
			}
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

	for (int i = 0; i < shapes.size(); i++)
	{
		meshes.push_back(Mesh(shapes[i].mesh.positions, shapes[i].mesh.indices, shapes[i].mesh.normals, shapes[i].mesh.texcoords, pathTexture));
	}
}


Model::~Model()
{
}

btRigidBody* Model::getBody() {
	return rigidBody;
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
