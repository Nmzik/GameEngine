#include "YftLoader.h"
#include "YdrLoader.h"

void YftLoader::Init(memstream2 & file, int32_t systemSize)
{
	Loaded = true;

	FragType* fragType = (FragType*)file.read(sizeof(FragType));

	SYSTEM_BASE_PTR(fragType->DrawablePointer);

	file.seekg(fragType->DrawablePointer);

	YdrFile = YdrPool::getPool().Load();
	YdrFile->isYft = true;
	YdrFile->Init(file, systemSize);
	gpuMemory += YdrFile->gpuMemory;

	/*SYSTEM_BASE_PTR(FragType.PhysicsLODGroupPointer);

	file.seekg(FragType.PhysicsLODGroupPointer);

	file.read((char*)&FragPhysicsLODGroup, sizeof(FragPhysicsLODGroup));

	SYSTEM_BASE_PTR(FragPhysicsLODGroup.PhysicsLOD1Pointer);

	file.seekg(FragPhysicsLODGroup.PhysicsLOD1Pointer);

	file.read((char*)&FragPhysicsLOD, sizeof(FragPhysicsLOD));

	SYSTEM_BASE_PTR(FragPhysicsLOD.ChildrenPointer);

	file.seekg(FragPhysicsLOD.ChildrenPointer);
	/*if (need) {

	for (int i = 0; i < FragPhysicsLOD.ChildrenCount; i++)
	{
	uint64_t DataPointer;
	file.read((char*)&DataPointer, sizeof(uint64_t));

	uint64_t ChildrenPointer = file.tellg();

	SYSTEM_BASE_PTR(DataPointer);

	file.seekg(DataPointer);

	file.read((char*)&FragPhysTypeChild, sizeof(FragPhysTypeChild));

	SYSTEM_BASE_PTR(FragPhysTypeChild.Drawable1Pointer);

	file.seekg(FragPhysTypeChild.Drawable1Pointer);

	//YdrLoader* ydr = myNew YdrLoader(file, systemSize, world, true);

	/*if (ydr->.size() != 0) {
	wheels.push_back(ydr);
	}
	else {
	delete ydr;
	}

	file.seekg(ChildrenPointer);
	}
	}*/

	//SYSTEM_BASE_PTR(FragPhysicsLOD.BoundPointer);

	//YbnLoader* loader = myNew YbnLoader(world, file, hash);*/
}


void YftLoader::Remove()
{
	gpuMemory = 0;
	Loaded = false;
	if (YdrFile) {
		YdrPool::getPool().Remove(YdrFile);
		YdrFile = nullptr;
	}
}

YftPool::YftPool()
{
	firstAvailable_ = &yfts[0];

	for (int i = 0; i < 999; i++)
	{
		yfts[i].next = &yfts[i + 1];
	}

	yfts[999].next = NULL;
}

YftPool::~YftPool()
{
}

YftLoader * YftPool::Load()
{
	// Make sure the pool isn't full.
	assert(firstAvailable_ != NULL);

	// Remove it from the available list.
	YftLoader* newYft = firstAvailable_;
	firstAvailable_ = newYft->next;

	return newYft;
}

void YftPool::Remove(YftLoader * yft)
{
	yft->Remove();
	yft->next = firstAvailable_;
	firstAvailable_ = yft;
}
