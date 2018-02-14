#include "Player.h"

Player::Player(glm::vec3 position, btDiscreteDynamicsWorld* world)
{
	btPairCachingGhostObject* physObject = new btPairCachingGhostObject();
	physShape = new btCapsuleShapeZ(0.45f, 1.2f);
	physObject->setUserPointer(this);
	physObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(position.x, position.y, position.z)));
	physObject->setCollisionShape(physShape);
	physObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	physCharacter = new btKinematicCharacterController(physObject, physShape, 0.30f);
	physCharacter->setFallSpeed(100.f);
	physCharacter->setUseGhostSweepTest(false);
	//physCharacter->setVelocityForTimeInterval(btVector3(1.f, 1.f, 1.f), 1.f);
	physCharacter->setGravity(world->getGravity());
	world->addCollisionObject(physObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	world->addAction(physCharacter);

	std::vector <float> vertices = {
			-0.4f, -0.4f, -0.4f,  0.0f,  0.0f, -0.4f, 0.0f, 0.0f, // bottom-left
             0.4f,  0.4f, -0.4f,  0.0f,  0.0f, -0.4f, 0.4f, 0.4f, // top-right
             0.4f, -0.4f, -0.4f,  0.0f,  0.0f, -0.4f, 0.4f, 0.0f, // bottom-right         
             0.4f,  0.4f, -0.4f,  0.0f,  0.0f, -0.4f, 0.4f, 0.4f, // top-right
            -0.4f, -0.4f, -0.4f,  0.0f,  0.0f, -0.4f, 0.0f, 0.0f, // bottom-left
            -0.4f,  0.4f, -0.4f,  0.0f,  0.0f, -0.4f, 0.0f, 0.4f, // top-left
            // front face
            -0.4f, -0.4f,  0.4f,  0.0f,  0.0f,  0.4f, 0.0f, 0.0f, // bottom-left
             0.4f, -0.4f,  0.4f,  0.0f,  0.0f,  0.4f, 0.4f, 0.0f, // bottom-right
             0.4f,  0.4f,  0.4f,  0.0f,  0.0f,  0.4f, 0.4f, 0.4f, // top-right
             0.4f,  0.4f,  0.4f,  0.0f,  0.0f,  0.4f, 0.4f, 0.4f, // top-right
            -0.4f,  0.4f,  0.4f,  0.0f,  0.0f,  0.4f, 0.0f, 0.4f, // top-left
            -0.4f, -0.4f,  0.4f,  0.0f,  0.0f,  0.4f, 0.0f, 0.0f, // bottom-left
            // left face
            -0.4f,  0.4f,  0.4f, -0.4f,  0.0f,  0.0f, 0.4f, 0.0f, // top-right
            -0.4f,  0.4f, -0.4f, -0.4f,  0.0f,  0.0f, 0.4f, 0.4f, // top-left
            -0.4f, -0.4f, -0.4f, -0.4f,  0.0f,  0.0f, 0.0f, 0.4f, // bottom-left
            -0.4f, -0.4f, -0.4f, -0.4f,  0.0f,  0.0f, 0.0f, 0.4f, // bottom-left
            -0.4f, -0.4f,  0.4f, -0.4f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -0.4f,  0.4f,  0.4f, -0.4f,  0.0f,  0.0f, 0.4f, 0.0f, // top-right
            // right face
             0.4f,  0.4f,  0.4f,  0.4f,  0.0f,  0.0f, 0.4f, 0.0f, // top-left
             0.4f, -0.4f, -0.4f,  0.4f,  0.0f,  0.0f, 0.0f, 0.4f, // bottom-right
             0.4f,  0.4f, -0.4f,  0.4f,  0.0f,  0.0f, 0.4f, 0.4f, // top-right         
             0.4f, -0.4f, -0.4f,  0.4f,  0.0f,  0.0f, 0.0f, 0.4f, // bottom-right
             0.4f,  0.4f,  0.4f,  0.4f,  0.0f,  0.0f, 0.4f, 0.0f, // top-left
             0.4f, -0.4f,  0.4f,  0.4f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -0.4f, -0.4f, -0.4f,  0.0f, -0.4f,  0.0f, 0.0f, 0.4f, // top-right
             0.4f, -0.4f, -0.4f,  0.0f, -0.4f,  0.0f, 0.4f, 0.4f, // top-left
             0.4f, -0.4f,  0.4f,  0.0f, -0.4f,  0.0f, 0.4f, 0.0f, // bottom-left
             0.4f, -0.4f,  0.4f,  0.0f, -0.4f,  0.0f, 0.4f, 0.0f, // bottom-left
            -0.4f, -0.4f,  0.4f,  0.0f, -0.4f,  0.0f, 0.0f, 0.0f, // bottom-right
            -0.4f, -0.4f, -0.4f,  0.0f, -0.4f,  0.0f, 0.0f, 0.4f, // top-right
            // top face
            -0.4f,  0.4f, -0.4f,  0.0f,  0.4f,  0.0f, 0.0f, 0.4f, // top-left
             0.4f,  0.4f , 0.4f,  0.0f,  0.4f,  0.0f, 0.4f, 0.0f, // bottom-right
             0.4f,  0.4f, -0.4f,  0.0f,  0.4f,  0.0f, 0.4f, 0.4f, // top-right     
             0.4f,  0.4f,  0.4f,  0.0f,  0.4f,  0.0f, 0.4f, 0.0f, // bottom-right
            -0.4f,  0.4f, -0.4f,  0.0f,  0.4f,  0.0f, 0.0f, 0.4f, // top-left
            -0.4f,  0.4f,  0.4f,  0.0f,  0.4f,  0.0f, 0.0f, 0.0f  // bottom-left  
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

	playerDirection = glm::vec3(0.0f, 0.0f, 0.0f);
}

Player::~Player()
{
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	//printf("DESTRUCTOR \n");
}

btKinematicCharacterController* Player::getPhysCharacter()
{
	return physCharacter;
}

void Player::TakeDamage(float dmg)
{
	health -= dmg;
}

glm::mat4 Player::getPosition()
{
	if (physCharacter->getGhostObject()->getWorldTransform().getOrigin().getY() <= -300) {
		physCharacter->getGhostObject()->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-50, 100, 0)));
	}

	glm::mat4 model;
	physCharacter->getGhostObject()->getWorldTransform().getOpenGLMatrix(&model[0][0]);

	return model;
}

void Player::PhysicsTick()
{
	physCharacter->setWalkDirection(btVector3(playerDirection.x, playerDirection.y, playerDirection.z));
}

void Player::ExitVehicle()
{
	vehicle->SetThrottle(1.0f);
	vehicle->SetSteeringValue(0.0f);
	vehicle = NULL;
}

void Player::EnterVehicle(Vehicle* nearestVehicle)
{
	if (nearestVehicle != nullptr) {
		printf("WE ARE IN VEHICLE");
		vehicle = nearestVehicle;
	}
}

Vehicle* Player::GetCurrentVehicle()
{
	return vehicle;
}

void Player::Jump()
{
	if (physCharacter->onGround()) physCharacter->jump(btVector3(0.0f, 0.0f, 30.0f));
}

void Player::Draw()
{
	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}