#include "spirv_glsl.hpp"
#include "spirv_msl.hpp"

#include <vector>
#include <fstream>
#include <memory>

static std::vector<uint32_t> read_spirv_file(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (!file)
    {
        fprintf(stderr, "Failed to open SPIR-V file: %s\n", path);
        return {};
    }

    fseek(file, 0, SEEK_END);
    long len = ftell(file) / sizeof(uint32_t);
    rewind(file);

    std::vector<uint32_t> spirv(len);
    if (fread(spirv.data(), sizeof(uint32_t), len, file) != size_t(len))
        spirv.clear();

    fclose(file);
    return spirv;
}

static void write_file(std::string& contents, const char* path)
{
    std::ofstream out(path);
    if (!out.is_open())
    {
        fprintf(stderr, "Failed to write shader file: %s\n", path);
        return;
    }
    out << contents;
    out.close();
}

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s TYPE (MSL OR GLSL) SOURCE OUTPUT ", argv[0]);
        return -1;
    }

    const char* shaderType = argv[1];
    const char* srcPath = argv[2];
    const char* dstPath = argv[3];

    std::vector<uint32_t> spirv_binary = read_spirv_file(srcPath);
    std::string source;

    if (strcmp(shaderType, "GLSL") == 0)
    {
        spirv_cross::CompilerGLSL glsl(std::move(spirv_binary));

        spirv_cross::CompilerGLSL::Options options;
        //options.version = 430;
        options.vulkan_semantics = true;
        options.es = false;
        glsl.set_common_options(options);
        source = glsl.compile();
    }
    else if (strcmp(shaderType, "MSL") == 0)
    {
        spirv_cross::CompilerMSL msl(std::move(spirv_binary));

        spirv_cross::CompilerMSL::Options options;
        options.set_msl_version(2, 1);
        options.platform = spirv_cross::CompilerMSL::Options::Platform::iOS;
        msl.set_msl_options(options);

        source = msl.compile();
    }
    else
    {
        fprintf(stderr, "Unrecognized shader type");
        return -1;
    }
    write_file(source, dstPath);

    return 0;
}