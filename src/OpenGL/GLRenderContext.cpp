#include "GLRenderContext.h"

#include "SDL.h"
#include "GLFunctions.h"
#include "Windows/NativeWindow.h"

GLRenderContext::GLRenderContext(RenderContextDescriptor desc)
	: RenderContext(desc.videoMode)
{
	std::unique_ptr<NativeWindow> window = std::make_unique<NativeWindow>();

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GLContext GLContext = SDL_GL_CreateContext(window->window);

	SDL_GL_SetSwapInterval(0);  //	ZERO - no vsync

	OpenGL_Init();

	if (SDL_GL_ExtensionSupported("GL_EXT_texture_filter_anisotropic"))
	{
		float aniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
		printf("ANISO SUPPORTED %f\n", aniso);
	}
	else
	{
		printf("NOT SUPPORTED\n");
	}
	//	if (!GLEW_ARB_texture_compression_bptc) printf("NOT INITALIZED BPTC\n");
	//	if (!GLEW_EXT_texture_compression_s3tc) printf("NOT INITALIZED s3tc\n");
	//	if (!GLEW_EXT_texture_compression_rgtc) printf("NOT INITALIZED rgtc\n");

	//	glEnable(GL_DEBUG_OUTPUT);
	//	glDebugMessageCallback(myDebugCallback, nullptr);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_MULTISAMPLE);
	glDisable(GL_DITHER);

	glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);  //	always zero to one
}


GLRenderContext::~GLRenderContext()
{
}
