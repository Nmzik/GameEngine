#include "GameRenderer.h"
#include "CBuilding.h"
#include "Camera.h"
#include "Model.h"
#include "YdrLoader.h"

GameRenderer::GameRenderer(/*NativeWindow* window*/)
    : /*nativeWindow(window)
    ,*/
    renderDebugWorld(false)
{
    //nativeWindow->InitializeContext();
}

GameRenderer::~GameRenderer()
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

void GameRenderer::renderDrawable(YdrLoader* drawable)
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
            /*mainShader->use();

            glBindVertexArray(geometry.VAO);

            glActiveTexture(GL_TEXTURE0);
            curTexture = !curTexture;
            if (curTexture == 0)
                glBindTexture(GL_TEXTURE_2D, defaultTexture1);
            else
                glBindTexture(GL_TEXTURE_2D, defaultTexture2);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, defaultTexture1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, defaultTexture1);

            glDrawElements(GL_TRIANGLES, geometry.num_indices, GL_UNSIGNED_SHORT, 0);*/

/*NumDrawCalls++;
        }
    }
}

void GameRenderer::renderBuilding(CBuilding* building)
{
    renderDrawable(building->ydr);
}

void GameRenderer::renderPed(CPed* ped)
{
    for (auto& ydr : ped->playerModel)
    {
        renderDrawable(ydr);
    }
}

void GameRenderer::renderVehicle(CVehicle* vehicle)
{
    renderDrawable(vehicle->getDrawable()->ydr);
}*/

/*void GameRenderer::renderWorld(GameWorld* world, Camera* curCamera)
{
    //beginFrame();

    glm::mat4 view = curCamera->getViewMatrix();
    glm::mat4 projection = curCamera->getProjection();
    glm::mat4 ProjectionView = projection * view;

    curCamera->updateFrustum(ProjectionView);

    /*glBindBuffer(GL_UNIFORM_BUFFER, uboGlobal);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &ProjectionView[0]);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboGlobal);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboModel);

    for (auto& object : world->renderList)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, uboModel);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &object->getMatrix()[0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

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

    glDisable(GL_CULL_FACE);

    if (RenderDebugWorld)
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

/*endFrame();

    presentFrame();*/
//}
