#pragma once
#include <vector>

enum Type
{
	ymap,
	ydr,
	ydd,
	yft,
	ytd,
	ybn
};

class FileType;

class Resource
{
public:
	Type type;
	uint32_t Hash;
	int32_t SystemSize;
	FileType* file;
	bool loaded = false;
	std::vector<uint8_t> Buffer;

	Resource(Type t, uint32_t hash, FileType* _file)
		: type(t)
		, Hash(hash)
		, file(_file)
	{
	}

	~Resource()
	{
	}

private:
};
