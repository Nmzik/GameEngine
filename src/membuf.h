#pragma once
#include <iostream>

class memstream {
	uint64_t offset;
public:
	const uint8_t* data;

	memstream(const uint8_t *buffer, size_t sizeData) :
		data(buffer),
		offset(0)
	{

	}

	const uint8_t* read(uint64_t Count) {
		uint64_t origOffset = offset;

		offset += Count;

		return &data[origOffset];
	}

	void read(void* dst, uint64_t size) {
		memcpy(dst, &data[offset], size);
		offset += size;
	}

	std::string getString() {
		return std::string((char*)&data[offset]);
	}

	void seekg(uint64_t OffsetFile) {
		offset = OffsetFile;
	}

	uint64_t tellg() {
		return offset;
	}

	void seekCur(int64_t OffsetFile) {
		offset += OffsetFile;
	}

};