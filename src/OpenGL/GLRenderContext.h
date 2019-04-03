#pragma once
#include "RenderContext.h"

class GLRenderContext : public RenderContext
{
public:
	GLRenderContext(RenderContextDescriptor desc);
	~GLRenderContext();
};

