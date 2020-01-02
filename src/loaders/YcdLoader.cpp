#include "YdrLoader.h"
#include "YcdLoader.h"

YcdLoader::YcdLoader(memstream& file)
{
    ClipDictionary* dictionary = (ClipDictionary*)&file.data[0];
    dictionary->Resolve(file);

    printf("DONE");

}

YcdLoader::~YcdLoader()
{
}
