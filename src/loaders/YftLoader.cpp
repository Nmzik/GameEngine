#include "YftLoader.h"
#include "YbnLoader.h"
#include "YdrLoader.h"

void YftLoader::init(memstream& file)
{
    fragType = (FragType*)file.read(sizeof(FragType));
    fragType->Resolve(file);

    rmcDrawable* drawable = *(fragType->drawable);
    for (int i = 0; i < drawable->DrawableModels[0]->size(); i++)
    {
        for (int j = 0; j < drawable->DrawableModels[0]->Get(i)->m_geometries.size(); j++)
        {
            switch (drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->InfoPointer->Types)
            {
                case 216172782140628998:  //    YFT - 0x030000000199A006
                    switch (drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->InfoPointer->Flags)
                    {
                        case 16473:
                            drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->InfoPointer->Flags = VertexType::PCCH2H4;
                            break;  //  PCCH2H4
                    }
                    break;
                case 216172782140612614:  //    YFT - 0x0300000001996006  PNCH2H4
                    switch (drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->InfoPointer->Flags)
                    {
                        case 89:
                            drawable->DrawableModels[0]->Get(i)->m_geometries.Get(j)->VertexBufferPointer->InfoPointer->Flags = VertexType::PNCH2;
                            break;  //  PNCH2
                    }
                    break;
            }
        }
    }

    /*SYSTEM_BASE_PTR(fragType->DrawablePointer);
    file.seekg(fragType->DrawablePointer);

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

    /*SYSTEM_BASE_PTR(fragPhysicsLOD->BoundPointer);
    file.seekg(fragPhysicsLOD->BoundPointer);

    ybn = GlobalPool::GetInstance()->ybnPool.create();
    ybn->init(file);*/
}

void YftLoader::finalize(BaseRenderer* _renderer, memstream& file)
{
    loaded = true;
    rmcDrawable* drawable = (rmcDrawable*)fragType->drawable.operator*();
    ydr = GlobalPool::GetInstance()->ydrPool.create();
    ydr->loadDrawable(drawable, _renderer, file);
}

YftLoader::~YftLoader()
{
    if (ybn)
        GlobalPool::GetInstance()->ybnPool.remove(ybn);
    GlobalPool::GetInstance()->ydrPool.remove(ydr);
}
