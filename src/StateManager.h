#pragma once

#include <deque>
#include "State.h"

class StateManager
{
public:
	static StateManager& get()
	{
		static StateManager m;
		return m;
	}

	std::deque<std::unique_ptr<State>> states;

	template <class T, class... Targs>
	void enter(Targs&&... args)
	{
		// Notify the previous state it has been suspended
		if (!states.empty())
		{
			states.back()->exit();
		}
		states.emplace_back(std::move(std::make_unique<T>(args...)));
		states.back()->enter();
	}

	void exit()
	{
		states.back()->exit();
		states.pop_back();
		states.back()->enter();
	}

	void updateStack()
	{
		while (!states.empty() && states.back()->hasExited())
		{
			states.back()->exit();
			states.pop_back();
			if (!states.empty())
			{
				states.back()->enter();
			}
		}
	}

	static State* currentState()
	{
		if (StateManager::get().states.empty())
		{
			return nullptr;
		}
		return StateManager::get().states.back().get();
	}
};
