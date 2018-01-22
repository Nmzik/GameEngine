#pragma once
#include "GameWorld.h"
#include "RpfFile.h"

class GameData
{
	std::vector<RpfFile> RpfFiles;
public:
	GameData();
	~GameData();
};

