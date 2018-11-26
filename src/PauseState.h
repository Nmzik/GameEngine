#pragma once
#include "State.h"

class PauseState : public State
{
public:
	PauseState(Game* game);
	~PauseState();

	virtual void enter() override;
	virtual void exit() override;
};
