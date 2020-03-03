//
//  TextureDecompressor.hpp
//  Bullet3Common
//
//  Created by nmzik on 19.08.2019.
//

#ifndef TextureDecompressor_hpp
#define TextureDecompressor_hpp

#include <stdio.h>

#include "../Texture.h"

struct Quality
{
    enum Enum
    {
        Default,
        Highest,
        Fastest,

        NormalMapDefault,  // Treat the source as a normal map
        NormalMapHighest,
        NormalMapFastest,

        Count
    };
};

void imageEncodeFromRgba8(void* _dst, const void* _src, uint32_t _width, uint32_t _height, uint32_t _depth, Quality::Enum _quality);
void imageDecodeToRgba8(void* _dst, const void* _src, uint32_t _width, uint32_t _height, uint32_t _dstPitch, TextureFormat _srcFormat);
void imageDecodeToBgra8(void* _dst, const void* _src, uint32_t _width, uint32_t _height, uint32_t _dstPitch, TextureFormat format);

#endif /* TextureDecompressor_hpp */
