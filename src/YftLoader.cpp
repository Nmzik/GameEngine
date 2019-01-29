#include "YftLoader.h"
#include "YdrLoader.h"
#include "YbnLoader.h"

void YftLoader::Init(memstream& file)
{
	Loaded = true;

	FragType* fragType = (FragType*)file.read(sizeof(FragType));

	SYSTEM_BASE_PTR(fragType->DrawablePointer);
	file.seekg(fragType->DrawablePointer);

	ydr = std::make_unique<YdrLoader>();
	ydr->isYft = true;
	ydr->Init(file);
	gpuMemory += ydr->gpuMemory;

	SYSTEM_BASE_PTR(fragType->PhysicsLODGroupPointer);
	file.seekg(fragType->PhysicsLODGroupPointer);

	FragPhysicsLODGroup* fragPhysicsLODGroup = (FragPhysicsLODGroup*)file.read(sizeof(FragPhysicsLODGroup));

	SYSTEM_BASE_PTR(fragPhysicsLODGroup->PhysicsLOD1Pointer);
	file.seekg(fragPhysicsLODGroup->PhysicsLOD1Pointer);

	FragPhysicsLOD* fragPhysicsLOD = (FragPhysicsLOD*)file.read(sizeof(FragPhysicsLOD));
	SYSTEM_BASE_PTR(fragPhysicsLOD->ChildrenPointer);

	file.seekg(fragPhysicsLOD->ChildrenPointer);

	/*fragPhysicsLODs = new std::vector<YdrLoader>();
	fragPhysicsLODs->reserve(fragPhysicsLOD->ChildrenCount);

	for (int i = 0; i < fragPhysicsLOD->ChildrenCount; i++)
	{
	 uint64_t* data_pointer = (uint64_t*)file.read(sizeof(uint64_t));

	 uint64_t ChildrenPointer = file.tellg();

	 SYSTEM_BASE_PTR(data_pointer[0]);
	 file.seekg(data_pointer[0]);

	 FragPhysTypeChild* fragPhysTypeChild = (FragPhysTypeChild*)file.read(sizeof(FragPhysTypeChild));

	 SYSTEM_BASE_PTR(fragPhysTypeChild->Drawable1Pointer);
	 file.seekg(fragPhysTypeChild->Drawable1Pointer);

	 YdrLoader* ydr = YdrPool.getPool().Load();
	 ydr->isYft = true;
	 ydr->Init(file, systemSize);
	 fragPhysicsLODs->push_back(ydr);

	 file.seekg(ChildrenPointer);
	}*/

	SYSTEM_BASE_PTR(fragPhysicsLOD->BoundPointer);
	file.seekg(fragPhysicsLOD->BoundPointer);

	ybn = std::make_unique<YbnLoader>();
	ybn->Init(file);
}

YftLoader::~YftLoader()
{
	/*for (auto& ydr : *fragPhysicsLODs)
	{
	 YdrPool.getPool().Remove(ydr);
	}

	delete fragPhysicsLODs;*/
}
