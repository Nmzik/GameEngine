#include "YmapLoader.h"



YmapLoader::YmapLoader()
{
	std::ifstream file("C:\\Users\\nmzik\\Desktop\\vb_34_day.ymap", std::ios::binary);

	if (!file.is_open()) {
		printf("NOT FOUND!");
	}
	//ADD NEED
	uint32_t rsc7;
	file.read((char*)&rsc7, sizeof(uint32_t));

	if (rsc7 == 0x37435352) {
		printf("NOT IMPLEMENTED!");
	}
	else {
		printf("HERE");
	}

	file.seekg(0);
	uint8_t *ttest1 = new uint8_t[1000];
	file.read((char*)&ttest1[0], 1000);
	uint8_t *ttest2 = new uint8_t[1000];
	int ret;
	z_stream strm;

	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);

	if (ret != Z_OK) {
		std::cerr << "Zlib error: inflateInit() failed" << std::endl;
	}

	strm.avail_in = 1000;
	strm.next_in = (Bytef*)ttest1;
	strm.avail_out = 100000;
	strm.next_out = (Bytef*)ttest2;

	ret = inflate(&strm, Z_NO_FLUSH);

	switch (ret) {
	case Z_NEED_DICT:
		ret = Z_DATA_ERROR;
	case Z_DATA_ERROR:
	case Z_MEM_ERROR:
		std::cerr << "Zlib error: inflate()" << std::endl;
		//return ret;
	}
	inflateEnd(&strm);



	file.close();
}


YmapLoader::~YmapLoader()
{
}
