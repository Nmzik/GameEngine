#pragma once
#include <vector>

enum Type {
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
	uint32_t TextureDictionaryHash;
	int32_t SystemSize;
	FileType* file;
	bool loaded = false;
	std::vector<uint8_t> Buffer;

	Resource(Type t, uint32_t hash, uint32_t TextureHash, FileType* _file) : 
		type(t), 
		Hash(hash), 
		TextureDictionaryHash(TextureHash),
		file(_file)
	{

	}

	~Resource() {

	}

private:

};