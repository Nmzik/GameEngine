#pragma once
#include "FileType.h"
#include <unordered_map>
#include "YdrLoader.h"

struct DrawableDictionary : ResourceFileBase
{
	uint32_t Unknown_10h; // 0x00000000
	uint32_t Unknown_14h; // 0x00000000
	uint32_t Unknown_18h; // 0x00000001
	uint32_t Unknown_1Ch; // 0x00000000
	pgArray<uint32_t> HashesPointer;
	pgObjectArray<gtaDrawable> Drawables;
	uint32_t Unknown_3Ch; // 0x00000000

	void Resolve(memstream& file)
	{
		HashesPointer.Resolve(file);
		Drawables.Resolve(file);
		//	Drawables->Resolve(file);
	}
};

class YdrLoader;

class YddLoader : public FileType
{
	public:
	std::unordered_map<uint32_t, std::unique_ptr<YdrLoader>> ydrFiles;

	void Init(memstream& file) override;
	~YddLoader();
};