#pragma once

#include "Model.h"

class Player
{
	btKinematicCharacterController *physCharacter;
	unsigned int VBO, EBO, VAO;
public:
	Player(btDiscreteDynamicsWorld* world);
	~Player();

	glm::mat4 getPosition();
	void Jump();
	btKinematicCharacterController* getPhysCharacter();
	void Draw();
};

