#include "spirv_glsl.hpp"
#include "spirv_msl.hpp"

#include <vector>
#include <fstream>

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
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s SOURCE OUTPUT", argv[0]);
        return -1;
    }

    std::vector<uint32_t> spirv_binary = read_spirv_file(argv[1]);

    spirv_cross::CompilerMSL msl(std::move(spirv_binary));

    spirv_cross::CompilerMSL::Options options;
    options.set_msl_version(2, 1);
    options.platform = spirv_cross::CompilerMSL::Options::Platform::iOS;
    std::string source = msl.compile();

    /*spirv_cross::CompilerGLSL glsl(std::move(spirv_binary));

    // The SPIR-V is now parsed, and we can perform reflection on it.
    spirv_cross::ShaderResources resources = glsl.get_shader_resources();

    // Get all sampled images in the shader.
    for (auto& resource : resources.sampled_images)
    {
        unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
        unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
        printf("Image %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);

        // Modify the decoration to prepare it for GLSL.
        glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);

        // Some arbitrary remapping if we want.
        glsl.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);
    }

    // Set some options.
    spirv_cross::CompilerGLSL::Options options;
    options.version = 430;
    options.es = false;
    glsl.set_common_options(options);

    // Compile to GLSL, ready to give to GL driver.
    std::string source = glsl.compile();*/
    write_file(source, argv[2]);

    return 0;
}