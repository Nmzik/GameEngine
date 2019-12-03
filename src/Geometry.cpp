#include "Geometry.h"

Geometry::Geometry(VertexBufferHandle vert_handle, IndexBufferHandle index_handle, uint32_t layoutHandle, uint32_t countIndices, TextureHandle tex_handle)
    : vertexBufferHandle(vert_handle)
    , indexBufferHandle(index_handle)
    , textureHandle(tex_handle)
    , indexCount(countIndices)
    , vertexLayoutHandle(layoutHandle)
{
}

Geometry::~Geometry()
{
}
