#include "YddLoader.h"
#include "YdrLoader.h"

void YddLoader::init(memstream& file)
{
    drawableDictionary = (DrawableDictionary*)file.read(sizeof(DrawableDictionary));
    drawableDictionary->Resolve(file);

    ydrFiles.reserve(drawableDictionary->drawables.size());
}

void YddLoader::finalize(BaseRenderer* _renderer, memstream& file)
{
    for (int i = 0; i < drawableDictionary->drawables.size(); i++)
    {
        YdrLoader* ydr = GlobalPool::GetInstance()->ydrPool.create();
        rmcDrawable* drawable = (rmcDrawable*)drawableDictionary->drawables[i];
        ydr->loadDrawable(drawable, false, _renderer, file);

        uint32_t hash = drawableDictionary->hashes.Get(i);

        ydrFiles.insert({hash, ydr});
    }
}

YddLoader::~YddLoader()
{
    for (auto& ydr : ydrFiles)
    {
        GlobalPool::GetInstance()->ydrPool.remove(ydr.second);
    }
}
