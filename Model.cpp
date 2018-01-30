#include "Model.h"

Model::Model(glm::vec3 position, glm::quat rot, glm::vec3 scale, char const * ModelPath, char const * pathTexture, char const * specTexture, bool dynamic = false, bool GenerateCollision = true) : position{ position }, rot{ rot }, ModelPath{ ModelPath }, pathTexture{ pathTexture }, specTexture{ specTexture }, dynamic{ dynamic }, GenerateCollision{ GenerateCollision }
{
	ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::translate(ModelMatrix, position);
	ModelMatrix *= glm::toMat4(rot);
	//ModelMatrix = glm::scale(ModelMatrix, scale);
}


Model::~Model()
{
}

btRigidBody* Model::getBody() {
	return rigidBody;
}

glm::mat4 Model::getModelMatrix()
{
	if (rigidBody == NULL)
	{
		return ModelMatrix;
	}
	rigidBody->getWorldTransform().getOpenGLMatrix(&ModelMatrix[0][0]);
	return ModelMatrix;
}

glm::vec3 Model::GetPosition()
{
	if (rigidBody == NULL)
	{
		return position;
	}
	 return glm::vec3(rigidBody->getWorldTransform().getOrigin().getX(), rigidBody->getWorldTransform().getOrigin().getY(), rigidBody->getWorldTransform().getOrigin().getZ());
}

void Model::Load()
{
	std::string err;

	std::string mtlPath(ModelPath);

	size_type pos = mtlPath.find_last_of("\\");
	if (pos != std::string::npos)
	{
		mtlPath.erase(pos+1);
	}

	tinyobj::LoadObj(shapes, materials, err, ModelPath, mtlPath.c_str(), tinyobj::triangulation | tinyobj::calculate_normals);

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

		/*btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(position.x, position.y, position.z));

		btCompoundShape* shape = new btCompoundShape;

		for (int i = 0; i < shapes.size(); i++) {
			btTriangleIndexVertexArray *btMesh = new btTriangleIndexVertexArray(
				shapes[i].mesh.indices.size() / 3,
				(int *)shapes[i].mesh.indices.data(), 3 * sizeof(GLuint),
				shapes[i].mesh.positions.size(),
				(btScalar *)shapes[i].mesh.positions.data(), 3 * sizeof(float));
			btBvhTriangleMeshShape *childShape = new btBvhTriangleMeshShape(btMesh, true);
		shape->addChildShape(t, childShape);
		}*/

		btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(rot.x, rot.y, rot.z, rot.w), btVector3(position.x, position.y, position.z)));
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

	meshes.reserve(shapes.size());

}

void Model::UploadToBuffers()
{
	for (auto& shape : shapes)
	{
		meshes.push_back(Mesh(shape.mesh.positions, shape.mesh.indices, shape.mesh.normals, shape.mesh.texcoords, pathTexture, specTexture));
	}
	isLoaded = true;
}

void Model::Draw()
{
	for (auto& mesh : meshes) {
		mesh.Draw();
	}
}
