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
	btKinematicCharacterController* getPhysCharacter();
	void Draw();
};

