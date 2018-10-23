#pragma once
#include "State.h"
class MenuState :
	public State
{
public:
	MenuState(Game* game);
	~MenuState();

	virtual void enter() override;
	virtual void exit() override;
};

