#pragma once

class Game;

class State {
public:
	Game* game;

	State(Game* game) : game(game) {

	}

	virtual void enter() = 0;
	virtual void exit() = 0;

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