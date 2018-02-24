#include "Model.h"

Model::Model(btDiscreteDynamicsWorld* DynamicsWorld, glm::vec3 position, glm::quat rot, glm::vec3 scale, char const * ModelPath, char const * pathTexture, char const * specTexture, bool dynamic = false, bool GenerateCollision = true) : position{ position }, rot{ rot }, ModelPath{ ModelPath }, pathTexture{ pathTexture }, specTexture{ specTexture }, dynamic{ dynamic }, GenerateCollision{ GenerateCollision }, world{ DynamicsWorld }
{
	ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::translate(ModelMatrix, position);
	ModelMatrix *= glm::toMat4(rot);
	ModelMatrix = glm::scale(ModelMatrix, scale);
}


Model::~Model()
{
}

btRigidBody* Model::getBody() {
	return rigidBody;
}

glm::mat4 Model::getModelMatrix()
{
	//if (rigidBody == NULL)
	//{
		//return ModelMatrix;
	//}

	glm::mat4 testmat = glm::mat4(1.0);
	testmat *= glm::toMat4(glm::quat(0.f, 0.f, -0.707f, 0.707f));
	//glm::mat4 rotate = glm::toMat4(glm::quat(0.f, 0.f, 0.0, 1.0f));
	//model = rotate * model; //ROTATION ORDER
	//rigidBody->getWorldTransform().getOpenGLMatrix(&ModelMatrix[0][0]);
	return testmat;
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

	size_t pos = mtlPath.find_last_of("\\");
	if (pos != std::string::npos)
	{
		mtlPath.erase(pos+1);
	}

	//tinyobj::LoadObj(shapes, materials, err, ModelPath, mtlPath.c_str(), tinyobj::triangulation | tinyobj::calculate_normals);

	if (GenerateCollision) {

		btConvexHullShape *shape = new btConvexHullShape();

		
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

		btDefaultMotionState* MotionState = new btDefaultMotionState(btTransform(btQuaternion(rot.x, rot.z, rot.y, rot.w), btVector3(position.x, position.y, position.z)));
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
		world->addRigidBody(rigidBody);
	}

	//meshes.reserve(shapes.size());

}

void Model::UploadToBuffers()
{
	
	isLoaded = true;
}

void Model::Draw()
{
	for (auto& mesh : meshes) {
		mesh.Draw();
	}
}
