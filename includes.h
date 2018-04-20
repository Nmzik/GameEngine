#pragma once

#include <vector>

enum Type {
	ymap,
	ydr,
	ydd,
	yft,
	ytd
};

class Resource
{
public:
	Type type;
	uint32_t Hash;
	uint32_t TextureDictionaryHash;
	std::vector<uint8_t> Buffer;

	Resource(Type t, uint32_t hash, uint32_t TextureHash = 0) : type(t), Hash(hash), TextureDictionaryHash(TextureHash) {

	}

	~Resource() {

	}

private:

};
