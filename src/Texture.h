#pragma once
#include <cstdint>

enum class Format
{
    Undefined,  //!< Undefined format.

    /* --- Color formats --- */
    R8UNorm,  //!< Color format: red 8-bit normalized unsigned integer component.
    R8SNorm,  //!< Color format: red 8-bit normalized signed integer component.
    R8UInt,   //!< Color format: red 8-bit unsigned integer component.
    R8SInt,   //!< Color format: red 8-bit signed integer component.

    R16UNorm,  //!< Color format: red 16-bit normalized unsigned interger component.
    R16SNorm,  //!< Color format: red 16-bit normalized signed interger component.
    R16UInt,   //!< Color format: red 16-bit unsigned interger component.
    R16SInt,   //!< Color format: red 16-bit signed interger component.
    R16Float,  //!< Color format: red 16-bit floating point component.

    R32UInt,   //!< Color format: red 32-bit unsigned interger component.
    R32SInt,   //!< Color format: red 32-bit signed interger component.
    R32Float,  //!< Color format: red 32-bit floating point component.

    RG8UNorm,  //!< Color format: red, green 8-bit normalized unsigned integer components.
    RG8SNorm,  //!< Color format: red, green 8-bit normalized signed integer components.
    RG8UInt,   //!< Color format: red, green 8-bit unsigned integer components.
    RG8SInt,   //!< Color format: red, green 8-bit signed integer components.

    RG16UNorm,  //!< Color format: red, green 16-bit normalized unsigned interger components.
    RG16SNorm,  //!< Color format: red, green 16-bit normalized signed interger components.
    RG16UInt,   //!< Color format: red, green 16-bit unsigned interger components.
    RG16SInt,   //!< Color format: red, green 16-bit signed interger components.
    RG16Float,  //!< Color format: red, green 16-bit floating point components.

    RG32UInt,   //!< Color format: red, green 32-bit unsigned interger components.
    RG32SInt,   //!< Color format: red, green 32-bit signed interger components.
    RG32Float,  //!< Color format: red, green 32-bit floating point components.

    RGB8UNorm,  //!< Color format: red, green, blue 8-bit normalized unsigned integer components. \note Only supported with: OpenGL, Vulkan.
    RGB8SNorm,  //!< Color format: red, green, blue 8-bit normalized signed integer components. \note Only supported with: OpenGL, Vulkan.
    RGB8UInt,   //!< Color format: red, green, blue 8-bit unsigned integer components. \note Only supported with: OpenGL, Vulkan.
    RGB8SInt,   //!< Color format: red, green, blue 8-bit signed integer components. \note Only supported with: OpenGL, Vulkan.

    RGB16UNorm,  //!< Color format: red, green, blue 16-bit normalized unsigned interger components. \note Only supported with: OpenGL, Vulkan.
    RGB16SNorm,  //!< Color format: red, green, blue 16-bit normalized signed interger components. \note Only supported with: OpenGL, Vulkan.
    RGB16UInt,   //!< Color format: red, green, blue 16-bit unsigned interger components. \note Only supported with: OpenGL, Vulkan.
    RGB16SInt,   //!< Color format: red, green, blue 16-bit signed interger components. \note Only supported with: OpenGL, Vulkan.
    RGB16Float,  //!< Color format: red, green, blue 16-bit floating point components. \note Only supported with: OpenGL, Vulkan.

    RGB32UInt,   //!< Color format: red, green, blue 32-bit unsigned interger components.
    RGB32SInt,   //!< Color format: red, green, blue 32-bit signed interger components.
    RGB32Float,  //!< Color format: red, green, blue 32-bit floating point components.

    RGBA8UNorm,  //!< Color format: red, green, blue, alpha 8-bit normalized unsigned integer components.
    RGBA8SNorm,  //!< Color format: red, green, blue, alpha 8-bit normalized signed integer components.
    RGBA8UInt,   //!< Color format: red, green, blue, alpha 8-bit unsigned integer components.
    RGBA8SInt,   //!< Color format: red, green, blue, alpha 8-bit signed integer components.

    RGBA16UNorm,  //!< Color format: red, green, blue, alpha 16-bit normalized unsigned interger components.
    RGBA16SNorm,  //!< Color format: red, green, blue, alpha 16-bit normalized signed interger components.
    RGBA16UInt,   //!< Color format: red, green, blue, alpha 16-bit unsigned interger components.
    RGBA16SInt,   //!< Color format: red, green, blue, alpha 16-bit signed interger components.
    RGBA16Float,  //!< Color format: red, green, blue, alpha 16-bit floating point components.

    RGBA32UInt,   //!< Color format: red, green, blue, alpha 32-bit unsigned interger components.
    RGBA32SInt,   //!< Color format: red, green, blue, alpha 32-bit signed interger components.
    RGBA32Float,  //!< Color format: red, green, blue, alpha 32-bit floating point components.

    /* --- Extended color formats --- */
    R64Float,     //!< Color format: red 64-bit floating point component. \note Only supported with: OpenGL, Vulkan.
    RG64Float,    //!< Color format: red, green 64-bit floating point components. \note Only supported with: OpenGL, Vulkan.
    RGB64Float,   //!< Color format: red, green, blue 64-bit floating point components. \note Only supported with: OpenGL, Vulkan.
    RGBA64Float,  //!< Color format: red, green, blue, alpha 64-bit floating point components. \note Only supported with: OpenGL, Vulkan.

    /* --- Reversed color formats --- */
    BGRA8UNorm,  //!< Color format: blue, green, red, alpha 8-bit normalized unsigned integer components. \note Only supported with: Vulkan, Direct3D 11, Direct3D 12, Metal.
    BGRA8SNorm,  //!< Color format: blue, green, red, alpha 8-bit normalized signed integer components. \note Only supported with: Vulkan.
    BGRA8UInt,   //!< Color format: blue, green, red, alpha 8-bit unsigned integer components. \note Only supported with: Vulkan.
    BGRA8SInt,   //!< Color format: blue, green, red, alpha 8-bit signed integer components. \note Only supported with: Vulkan.
    BGRA8sRGB,   //!< Color format: blue, green, red, alpha 8-bit normalized unsigned integer components in sRGB non-linear color space. \note Only supported with: Vulkan, Direct3D 11, Direct3D 12, Metal.

    /* --- Depth-stencil formats --- */
    D16UNorm,           //!< Depth-stencil format: depth 16-bit normalized unsigned integer component.
    D24UNormS8UInt,     //!< Depth-stencil format: depth 24-bit normalized unsigned integer component, and 8-bit unsigned integer stencil component.
    D32Float,           //!< Depth-stencil format: depth 32-bit floating point component.
    D32FloatS8X24UInt,  //!< Depth-stencil format: depth 32-bit floating point component, and 8-bit unsigned integer stencil components (where the remaining 24 bits are unused).

    /* --- Compressed color formats --- */
    BC1RGB,   //!< Compressed color format: RGB S3TC DXT1 with 8 bytes per 4x4 block. \note Only supported with: OpenGL, Vulkan.
    BC1RGBA,  //!< Compressed color format: RGBA S3TC DXT1 with 8 bytes per 4x4 block.
    BC2RGBA,  //!< Compressed color format: RGBA S3TC DXT3 with 16 bytes per 4x4 block.
    BC3RGBA,  //!< Compressed color format: RGBA S3TC DXT5 with 16 bytes per 4x4 block.
};

enum class TextureType
{
    Texture1D,         //!< 1-Dimensional texture.
    Texture2D,         //!< 2-Dimensional texture.
    Texture3D,         //!< 3-Dimensional texture.
    TextureCube,       //!< Cube texture.
    Texture1DArray,    //!< 1-Dimensional array texture.
    Texture2DArray,    //!< 2-Dimensional array texture.
    TextureCubeArray,  //!< Cube array texture.
    Texture2DMS,       //!< 2-Dimensional multi-sample texture.
    Texture2DMSArray,  //!< 2-Dimensional multi-sample array texture.
};

struct TextureDescriptor
{
    //! Hardware texture type. By default TextureType::Texture1D.
    TextureType type = TextureType::Texture1D;

    Format format = Format::RGBA8UNorm;
	
	/**
    \brief Number of MIP-map levels. By default 0.
    \remarks If this is 0, the full MIP-chain will be generated.
    If this is 1, no MIP-mapping is used for this texture and it has only a single MIP-map level.
    This field is ignored for multi-sampled textures (i.e. TextureType::Texture2DMS, TextureType::Texture2DMSArray),
    since these texture types only have a single MIP-map level.
    \see NumMipLevels
    \see RenderSystem::GenerateMips
    */
    uint32_t mipLevels = 0;
};

class Texture
{
public:
	Texture();
	virtual ~Texture();
};

