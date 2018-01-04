#pragma once
#include "State.h"
class InGameState :
	public State
{
public:
	InGameState(Game* game);
	~InGameState();

	virtual void enter() override;
	virtual void exit() override;

	virtual void tick(float delta_time) override;

	virtual void handleEvent(const SDL_Event& event) override;
};

