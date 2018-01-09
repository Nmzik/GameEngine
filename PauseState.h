#pragma once
#include "State.h"

class PauseState :
	public State
{
public:
	PauseState(Game* game);
	~PauseState();

	virtual void enter() override;
	virtual void exit() override;

	virtual void tick(float delta_time) override;

	virtual void handleEvent(const SDL_Event& event) override;
};

