#include "YftLoader.h"
#include "YdrLoader.h"
#include "YbnLoader.h"

void YftLoader::Init(memstream2 & file, int32_t systemSize)
{
	Loaded = true;

	FragType* fragType = (FragType*)file.read(sizeof(FragType));

	SYSTEM_BASE_PTR(fragType->DrawablePointer);

	file.seekg(fragType->DrawablePointer);

	YdrFile = YdrPool.getPool().Load();
	YdrFile->isYft = true;
	YdrFile->Init(file, systemSize);
	gpuMemory += YdrFile->gpuMemory;

	SYSTEM_BASE_PTR(fragType->PhysicsLODGroupPointer);
	file.seekg(fragType->PhysicsLODGroupPointer);

	FragPhysicsLODGroup* fragPhysicsLODGroup = (FragPhysicsLODGroup*)file.read(sizeof(FragPhysicsLODGroup));

	SYSTEM_BASE_PTR(fragPhysicsLODGroup->PhysicsLOD1Pointer);
	file.seekg(fragPhysicsLODGroup->PhysicsLOD1Pointer);

	FragPhysicsLOD* fragPhysicsLOD = (FragPhysicsLOD*)file.read(sizeof(FragPhysicsLOD));
	SYSTEM_BASE_PTR(fragPhysicsLOD->ChildrenPointer);

	//file.seekg(FragPhysicsLOD.ChildrenPointer);
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

	SYSTEM_BASE_PTR(fragPhysicsLOD->BoundPointer);
	file.seekg(fragPhysicsLOD->BoundPointer);

	ybnFile = YbnPool.getPool().Load();
	ybnFile->Init(file);
}


void YftLoader::Remove()
{
	gpuMemory = 0;

	if (YdrFile) {
		YdrPool.getPool().Remove(YdrFile);
		YdrFile = nullptr;
	}
	if (ybnFile) {
		YbnPool.getPool().Remove(ybnFile);
		ybnFile = nullptr;
	}
}
