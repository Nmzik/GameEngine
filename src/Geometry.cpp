#include "Geometry.h"

#include "YdrLoader.h"

Geometry::Geometry(VertexBufferHandle vert_handle, IndexBufferHandle index_handle, uint32_t countIndices, TextureHandle tex_handle)
    : vertexBufferHandle(vert_handle)
    , indexBufferHandle(index_handle)
    , indexCount(countIndices)
    , textureHandle(tex_handle)
{
}

Geometry::~Geometry()
{
    /*glDeleteBuffers(2, VBO);
    glDeleteVertexArrays(1, &VAO);*/
}
