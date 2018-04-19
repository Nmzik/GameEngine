#pragma once
#include "YdrLoader.h"

class YddLoader
{
public:
	std::unordered_map<uint32_t, YdrLoader*> YdrFiles;
	YtdFile * externalYtd;
	uint32_t time;
	YddLoader(memstream& file, btDiscreteDynamicsWorld* world);
	~YddLoader();

	void Draw(Shader* shader);
};

