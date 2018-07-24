#pragma once
#include "FileType.h"
#include <unordered_map>

struct DrawableDictionary : ResourceFileBase {
	uint32_t Unknown_10h; // 0x00000000
	uint32_t Unknown_14h; // 0x00000000
	uint32_t Unknown_18h; // 0x00000001
	uint32_t Unknown_1Ch; // 0x00000000
	uint64_t HashesPointer;
	uint16_t HashesCount1;
	uint16_t HashesCount2;
	uint32_t Unknown_2Ch; // 0x00000000
	uint64_t DrawablesPointer;
	uint16_t DrawablesCount1;
	uint16_t DrawablesCount2;
	uint32_t Unknown_3Ch; // 0x00000000
};

class YdrLoader;

class YddLoader : public FileType
{
public:
	YddLoader * next;
	std::unordered_map<uint32_t, YdrLoader*> YdrFiles;

	void Init(memstream2 & file, int32_t systemSize) override;
	void Remove();
};

class YddPool
{
public:
	static YddPool& getPool() {
		static YddPool pool;
		return pool;
	}

	YddPool();
	~YddPool();

	YddLoader* Load();
	void Remove(YddLoader* ymap);

	YddLoader ydds[1000];

private:
	YddLoader * firstAvailable_;
};
