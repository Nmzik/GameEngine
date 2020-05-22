#include "WorldRenderer.h"

#include "CBuilding.h"
#include "Camera.h"
#include "Model.h"

#include "loaders/YddLoader.h"
#include "loaders/YtdLoader.h"

#ifdef WIN32
#ifdef VULKAN_API
	#include "vulkan/VulkanRenderer.h"
#else
	#include "OpenGL\OpenGLRenderer.h"
#include "windows/Win32Window.h"
#endif
#elif __ANDROID__
	#define VULKAN_API
	#include "vulkan/VulkanRenderer.h"
#else
	#include "metal/MetalRenderer.h"
#endif

WorldRenderer::WorldRenderer(NativeWindow* window)
{
#if defined(WIN32) || defined(__ANDROID__)
#ifdef VULKAN_API
	renderer = std::make_unique<VulkanRenderer>(window);
#ifdef WIN32
	VulkanRenderer* vulkanRenderer = (VulkanRenderer*)renderer.get();
	vulkanRenderer->initialize();
#endif
#else
	renderer = std::make_unique<OpenGLRenderer>(window);
#endif
#else
	renderer = std::make_unique<MetalRenderer>();
#endif
}

WorldRenderer::~WorldRenderer()
{
}

void WorldRenderer::postLoad()
{
    renderer->postLoad();
}

void WorldRenderer::renderSky(GameWorld* world)
{
    /*if (world->skydome && world->skydomeYTD)
    {
        YdrLoader* skydome = world->skydome->ydrFiles.begin()->second;
        Geometry& geom = world->skydome->ydrFiles.begin()->second->models[0].geometries[0];
        TextureHandle handle = world->skydomeYTD->textures[1064311147].getHandle();

        geom.setTextureHandle(handle);

        glm::mat4 pos = world->getCurrentPlayer()->getMatrix();

        renderer->updatePerModelData(pos);

        renderer->renderSky(geom);
    }*/
}

void WorldRenderer::renderDrawable(YdrLoader* drawable)
{
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
        renderDrawable(ydr);
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

    renderSky(world);

    renderer->updateGlobalSceneBuffer(projection, view);

    for (auto& object : world->renderList)
    {
        glm::mat4 matrix = object->getMatrix();
        renderer->updatePerModelData(matrix);

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
