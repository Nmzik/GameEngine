#pragma once
#include <cstdint>

struct VideoModeDescriptor
{
    /**
    \brief Screen resolution (in pixels).
    \remarks If the resolution contains a member with a value of 0, the video mode is invalid.
    \see RenderTarget::GetResolution
    */
    uint32_t width = 0;  //!< Extent X axis, i.e. width.
    uint32_t height = 0;

    /**
    \brief Number of bits for each pixel in the color buffer. Should be 24 or 32. By default 32.
    \remarks This is only a hint to the renderer and there is no guarantee which hardware format is finally used for the color buffer.
    To determine the actual color format of a render context, use the RenderContext::QueryColorFormat function.
    \see RenderContext::QueryColorFormat
    */
    int colorBits = 32;

    /**
    \brief Number of bits for each pixel in the depth buffer. Should be 24, 32, or zero to disable depth buffer. By default 24.
    \remarks This is only a hint to the renderer and there is no guarantee which hardware format is finally used for the depth buffer.
    To determine the actual depth-stencil format of a render context, use the RenderContext::QueryDepthStencilFormat function.
    \see RenderContext::QueryDepthStencilFormat
    */
    int depthBits = 24;

    /**
    \brief Number of bits for each pixel in the stencil buffer. Should be 8, or zero to disable stencil buffer. By default 8.
    \remarks This is only a hint to the renderer and there is no guarantee which hardware format is finally used for the stencil buffer.
    To determine the actual depth-stencil format of a render context, use the RenderContext::QueryDepthStencilFormat function.
    \see RenderContext::QueryDepthStencilFormat
    */
    int stencilBits = 8;

    //! Specifies whether to enable fullscreen mode or windowed mode. By default windowed mode.
    bool fullscreen = false;

    /**
    \brief Number of swap-chain buffers. By default 2 (for double-buffering).
    \remarks This is only a hint to the renderer and there is no guarantee how many buffers are finally used for the swap chain.
    Especially OpenGL does not support custom swap chain sizes.
    If this value is 0, the video mode is invalid.
    */
    uint32_t swapChainSize = 2;
};

struct RenderContextDescriptor
{
    VideoModeDescriptor videoMode;
};

class RenderContext
{
    VideoModeDescriptor videoDesc;

public:
    RenderContext(VideoModeDescriptor desc);
	virtual ~RenderContext();
};

