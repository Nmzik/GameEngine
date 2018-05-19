#include "YnvLoader.h"



YnvLoader::YnvLoader()
{
	dtNavMeshCreateParams params;

	memset(&params, 0, sizeof(params));

	int navDataSize;
	unsigned char* navData;
	bool result = dtCreateNavMeshData(&params, &navData, &navDataSize);
	if (result)
	{
		printf("DONE");
	}
}


YnvLoader::~YnvLoader()
{
}
