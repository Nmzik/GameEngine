#include "../CBuilding.h"
#include "../Camera.h"
#include "../Model.h"
#include "Shader.h"
//#include "../YdrLoader.h"
#include "OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer(NativeWindow* window)
    : nativeWindow(window)
{
    nativeWindow->initializeContext();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_MULTISAMPLE);
    glDisable(GL_DITHER);

    glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);

    mainShader = new Shader("assets/shaders/gbuffer");

    glGenBuffers(1, &uboGlobal);
    glBindBuffer(GL_UNIFORM_BUFFER, uboGlobal);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &uboModel);
    glBindBuffer(GL_UNIFORM_BUFFER, uboModel);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//Default texture
    glGenTextures(1, &textures[0]);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    float pixels1[] = {0.85f, 0.79f, 0.79f, 0.85f, 0.79f, 0.79f, 0.85f, 0.79f, 0.79f, 0.85f, 0.79f, 0.79f};
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels1);
}

OpenGLRenderer::~OpenGLRenderer()
{
}

void OpenGLRenderer::beginFrame()
{
    
}

void OpenGLRenderer::endFrame()
{
}

void OpenGLRenderer::presentFrame()
{
    nativeWindow->swapBuffers();
}

VertexBufferHandle OpenGLRenderer::createVertexBuffer(uint32_t size, const uint8_t* pointer)
{
    /*glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, VBO);
    //////////////
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 drawGeom->VertexBufferPointer->VertexCount * drawGeom->VertexBufferPointer->VertexStride,
                 &meshData[drawGeom->VertexBufferPointer->DataPointer1],
                 GL_STATIC_DRAW);

    //	const Position = 0; //	Positions
    //	const Normals = 1; //	NORMALS
    //	const Color = 2; //	COLOR
    //	const TexCoord = 3; //	TEXTCOORD

    for (auto& Attribute : *Attributes)
    {
        glEnableVertexAttribArray(Attribute.index);
        glVertexAttribPointer(Attribute.index, Attribute.size, Attribute.type, GL_FALSE, Attribute.stride, Attribute.pointer);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 drawGeom->IndexBufferPointer->IndicesCount * sizeof(uint16_t),
                 &meshData[drawGeom->IndexBufferPointer->IndicesPointer],
                 GL_STATIC_DRAW);  //	16 BIT INDICES max 65536*/
    return VertexBufferHandle();
}

IndexBufferHandle OpenGLRenderer::createIndexBuffer(uint32_t size, const uint8_t* pointer)
{
    return IndexBufferHandle();
}

TextureHandle OpenGLRenderer::createTexture(const uint8_t* pointer, int width, int height, int levels, TextureFormat format)
{
    return TextureHandle();
}

void OpenGLRenderer::removeVertexBuffer(VertexBufferHandle handle)
{
    //glDeleteBuffers(2, VBO);
    //glDeleteVertexArrays(1, &VAO);
}

void OpenGLRenderer::removeIndexbuffer(IndexBufferHandle handle)
{
}

void OpenGLRenderer::removeTexture(TextureHandle handle)
{
}

void OpenGLRenderer::renderDrawable(YdrLoader* drawable)
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
            mainShader->use();

            /*glBindVertexArray(geometry.VAO);

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

            glDrawElements(GL_TRIANGLES, geometry.num_indices, GL_UNSIGNED_SHORT, 0);;*/
        }
    }
}

void OpenGLRenderer::renderBuilding(CBuilding* building)
{
    renderDrawable(building->ydr);
}

void OpenGLRenderer::renderPed(CPed* ped)
{
    for (auto& ydr : ped->playerModel)
    {
        renderDrawable(ydr);
    }
}

void OpenGLRenderer::renderVehicle(CVehicle* vehicle)
{
    renderDrawable(vehicle->getDrawable()->ydr);
}

void OpenGLRenderer::renderWorld(GameWorld* world, Camera* curCamera)
{
    beginFrame();

    glm::mat4 view = curCamera->getViewMatrix();
    glm::mat4 projection = curCamera->getProjection();
    glm::mat4 ProjectionView = projection * view;

    curCamera->updateFrustum(ProjectionView);

    glBindBuffer(GL_UNIFORM_BUFFER, uboGlobal);
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

    endFrame();

    presentFrame();
}