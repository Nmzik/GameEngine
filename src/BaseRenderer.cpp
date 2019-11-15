#include "BaseRenderer.h"

#include "CBuilding.h"
#include "Camera.h"
#include "Model.h"

BaseRenderer::BaseRenderer(/*NativeWindow* window*/)
    : /*nativeWindow(window)
    ,*/
    renderDebugWorld(false)
{
    //nativeWindow->InitializeContext();
}

BaseRenderer::~BaseRenderer()
{
}

/*void GameRenderer::beginFrame()
{
    numDrawCalls = 0;
}

void GameRenderer::endFrame()
{
}

void GameRenderer::presentFrame()
{
    //nativeWindow->SwapBuffers();
}

/*endFrame();

    presentFrame();*/
//}
