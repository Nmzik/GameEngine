#pragma once

#include <vector>

enum Type {
	ymap,
	ydr,
	ydd,
	yft
};

class Resource
{
public:
	Type type;
	uint32_t Hash;
	std::vector<uint8_t> Buffer;

	Resource(Type t, uint32_t hash) : type(t), Hash(hash) {

	}

	~Resource() {

	}

private:

};
