#pragma once
#include <fstream>
#include <sstream>
#include "iostream"
#include "opengl.h"
#include <glm/glm.hpp>

class Shader
{
    GLuint ID;

public:
    Shader(std::string ShaderPath);
    ~Shader();

    void use()
    {
        glUseProgram(ID);
    }

    void setBool(const uint32_t& UniformValue, bool value) const
    {
        glUniform1i(UniformValue, (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const uint32_t& UniformValue, int value) const
    {
        glUniform1i(UniformValue, value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const uint32_t& UniformValue, float value) const
    {
        glUniform1f(UniformValue, value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const uint32_t& UniformValue, const glm::vec2& value) const
    {
        glUniform2fv(UniformValue, 1, &value[0]);
    }
    // ------------------------------------------------------------------------
    void setVec3(const uint32_t& UniformValue, const glm::vec3& value) const
    {
        glUniform3fv(UniformValue, 1, &value[0]);
    }
    // ------------------------------------------------------------------------
    void setVec4(const uint32_t& UniformValue, const glm::vec4& value) const
    {
        glUniform4fv(UniformValue, 1, &value[0]);
    }
    // ------------------------------------------------------------------------
    void setMat2(const uint32_t& UniformValue, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(UniformValue, 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const uint32_t& UniformValue, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(UniformValue, 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    inline void setMat4(const uint32_t& UniformValue, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(UniformValue, 1, GL_FALSE, &mat[0][0]);
    }

private:
    void checkCompileErrors(unsigned int GLShader, std::string type);
};