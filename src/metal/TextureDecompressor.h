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

namespace Decompressor {
    void imageDecodeToBgra8(void* _dst, const void* _src, uint32_t _width, uint32_t _height, uint32_t _dstPitch, TextureFormat format);
}
#endif /* TextureDecompressor_hpp */
