#pragma once
#include <fstream>
#include <sstream>
#include "iostream"
#include "opengl.h"
#include <glm/glm.hpp>

class Shader
{
public:
    GLuint ID;

    Shader(std::string ShaderPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream ShaderFile;
        std::string line;
        std::stringstream ss[2];
        // open files
        ShaderFile.open(ShaderPath + ".vert");

        if (!ShaderFile.is_open())
        {
            printf("FILE NOT FOUND\n");
        }

        while (std::getline(ShaderFile, line))
        {
            ss[0] << line << '\n';
        }

        ShaderFile.close();
        ShaderFile.clear();
        ShaderFile.open(ShaderPath + ".frag");

        while (std::getline(ShaderFile, line))
        {
            ss[1] << line << '\n';
        }

        ShaderFile.close();

        // convert stream into string
        vertexCode = ss[0].str();
        fragmentCode = ss[1].str();
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

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
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
