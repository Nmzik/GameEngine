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

	virtual void tick(float delta_time) override;

	virtual void handleEvent(const SDL_Event& event) override;
};

