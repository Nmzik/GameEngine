#pragma once

enum class ShaderType
{
    Undefined,       //!< Undefined shader type.
    Vertex,          //!< Vertex shader type.
    TessControl,     //!< Tessellation control shader type (also "Hull Shader").
    TessEvaluation,  //!< Tessellation evaluation shader type (also "Domain Shader").
    Geometry,        //!< Geometry shader type.
    Fragment,        //!< Fragment shader type (also "Pixel Shader").
    Compute,         //!< Compute shader type.
};

struct ShaderDescriptor
{
    ShaderType type;
    const char* PathToShader;
};

class Shader
{
public:
    Shader(const ShaderType type);
    virtual ~Shader();

    inline ShaderType GetType() const
    {
        return type_;
    }

private:
    ShaderType type_;
};
