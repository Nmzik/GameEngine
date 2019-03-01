#pragma once
#include "GameData.h"
#include "opengl.h"

class Water
{
    uint32_t VBO, EBO, VAO;
    uint32_t num_indices;
    GLuint diffuseTextureID;

public:
    glm::vec3 BSCenter;
    float BSRadius;

    Water(WaterQuad waterQuad);
    ~Water();
    void Draw();
};
