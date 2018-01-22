#include "GameData.h"

GameData::GameData()
{
	std::vector <std::string> RpfsFiles = {
		{"C:\\Program Files\\Rockstar Games\\Grand Theft Auto V\\x64a.rpf"}
	};

	for (std::string rpfFile : RpfsFiles)
	{
		RpfFile file(rpfFile);
		RpfFiles.push_back(file);
	}
}


GameData::~GameData()
{
}
