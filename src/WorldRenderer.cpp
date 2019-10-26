#include "CBuilding.h"
#include "Camera.h"
#include "WorldRenderer.h"
#include "Model.h"

#ifdef WIN32
#include "OpenGL/OpenGLRenderer.h"
#include "windows/Win32Window.h"
#else
#include "metal/MetalRenderer.h"
#endif

WorldRenderer::WorldRenderer(NativeWindow* window)
{
#ifdef WIN32
    renderer = std::make_unique<OpenGLRenderer>(window);
#else
    renderer = std::make_unique<MetalRenderer>();
#endif
}

WorldRenderer::~WorldRenderer()
{
}

void WorldRenderer::renderDrawable(YdrLoader* drawable)
{
    int curTexture = 0;
    for (auto& model : drawable->models)
    {
        if ((model.Unk_2Ch & 1) == 0)
        {
            continue;  //	PROXIES
        }
        for (auto& geometry : model.geometries)
        {
			renderer->renderGeom(geometry);
        }
    }
}

void WorldRenderer::renderBuilding(CBuilding* building)
{
    renderDrawable(building->ydr);
}

void WorldRenderer::renderPed(CPed* ped)
{
    for (auto& ydr : ped->playerModel)
    {
        //renderDrawable(ydr);
    }
}

void WorldRenderer::renderVehicle(CVehicle* vehicle)
{
    renderDrawable(vehicle->getDrawable()->ydr);
}

void WorldRenderer::renderWorld(GameWorld* world, Camera* curCamera)
{
    renderer->beginFrame();

    glm::mat4 view = curCamera->getViewMatrix();
    glm::mat4 projection = curCamera->getProjection();
    glm::mat4 projectionView = projection * view;

    curCamera->updateFrustum(projectionView);

	renderer->updateGlobalSceneBuffer(projection, view);

    for (auto& object : world->renderList)
    {
        glm::mat4 test = object->getMatrix();
        renderer->updatePerModelData(test);

        switch (object->getType())
        {
            case ObjectType::Building:
            {
                CBuilding* building = static_cast<CBuilding*>(object);
                renderBuilding(building);
                break;
            }
            case ObjectType::Vehicle:
            {
                CVehicle* vehicle = static_cast<CVehicle*>(object);
                renderVehicle(vehicle);
                break;
            }
            case ObjectType::Ped:
            {
                CPed* ped = static_cast<CPed*>(object);
                renderPed(ped);
                break;
            }
            default:
                break;
        }
    }

    //GL_CHECK(glDisable(GL_CULL_FACE));

    /*if (RenderDebugWorld)
    {
        world->getPhysicsSystem()->getPhysicsWorld()->debugDrawWorld();
        glm::mat4 DefaultMatrix(1.0f);
        glBindBuffer(GL_UNIFORM_BUFFER, uboModel);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &DefaultMatrix[0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, defaultTexture);
        world->getPhysicsSystem()->debug.render();
    }*/

    //endFrame();

    renderer->presentFrame();
}
