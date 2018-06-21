#pragma once
#include "State.h"
#include "PauseState.h"
class InGameState :
	public State
{
public:
	InGameState(Game* game);
	~InGameState();

	virtual void enter() override;
	virtual void exit() override;
};

