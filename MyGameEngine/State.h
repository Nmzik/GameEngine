#pragma once

#include "SDL.h"

class Game;

class State {
public:
	Game* game;

	State(Game* game) : game(game) {

	}

	virtual void enter() = 0;
	virtual void exit() = 0;

	virtual void tick(float delta_time) = 0;

	virtual void handleEvent(const SDL_Event& e) = 0;

	bool hasExited() const {
		return hasexited_;
	}

private:
	bool hasexited_ = false;

protected:
	void done() {
		hasexited_ = true;
	}

};