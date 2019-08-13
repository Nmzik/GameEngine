#include "Shader.h"

Shader::Shader(std::string ShaderPath)
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
    // vertex GLShader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment GLShader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // GLShader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
}

void Shader::checkCompileErrors(unsigned int GLShader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(GLShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(GLShader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(GLShader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(GLShader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}