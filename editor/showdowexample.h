#ifndef SHADOW_EXAMPLE_H
#define SHADOW_EXAMPLE_H

#include "imgui_setup.h"
#include "window.h"

#include "utils/camera.h"
#include "utils/ecs.h"
#include "utils/controller.h"
#include "utils/components.h"

#include "gfx/model.h"
#include "gfx/newframebuffer.h"

#include "event.h"

class Editor
{
public:
    Editor(Window &window) : window(window)
    {
        editorCamera.setPerspectiveProjection(45.0f, 1200.f/800.0f, 0.001f, 100.f);
        cameraTransform.translation = {0, 0, -2};

        gfx::Model cube{};
        cube.loadModelFromPath("../assets/cube.obj");

        cubeCount = cube.getMeshes()[0].indices.size();

        lightVAO.bind();
        lightVBO.bind();
        lightEBO.bind();
        lightVBO.load((float*)(&cube.getMeshes()[0].vertices[0]), sizeof(gfx::Mesh::Vertex) * cube.getMeshes()[0].vertices.size());
        lightEBO.load(cube.getMeshes()[0].indices.data(), sizeof(unsigned int) * cube.getMeshes()[0].indices.size());
        lightVAO.linkVertexBuffer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gfx::Mesh::Vertex), (void*)(offsetof(gfx::Mesh::Vertex, position)));
        lightVAO.linkVertexBuffer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gfx::Mesh::Vertex), (void*)(offsetof(gfx::Mesh::Vertex, normal)));
        lightVAO.linkVertexBuffer(2, 2, GL_FLOAT, GL_FALSE, sizeof(gfx::Mesh::Vertex), (void*)(offsetof(gfx::Mesh::Vertex, uv)));
        lightVAO.unBind();
        lightVBO.unBind();
        lightEBO.unBind();

        cubeVAO.bind();
        cubeVBO.bind();
        cubeEBO.bind();
        cubeVBO.load((float*)(&cube.getMeshes()[0].vertices[0]), sizeof(gfx::Mesh::Vertex) * cube.getMeshes()[0].vertices.size());
        cubeEBO.load(cube.getMeshes()[0].indices.data(), sizeof(unsigned int) * cube.getMeshes()[0].indices.size());
        cubeVAO.linkVertexBuffer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gfx::Mesh::Vertex), (void*)(offsetof(gfx::Mesh::Vertex, position)));
        cubeVAO.linkVertexBuffer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gfx::Mesh::Vertex), (void*)(offsetof(gfx::Mesh::Vertex, normal)));
        cubeVAO.linkVertexBuffer(2, 2, GL_FLOAT, GL_FALSE, sizeof(gfx::Mesh::Vertex), (void*)(offsetof(gfx::Mesh::Vertex, uv)));
        cubeVAO.unBind();
        cubeVBO.unBind();
        cubeEBO.unBind();

        cube.free();

        gfx::Model plane{};
        plane.loadModelFromPath("../assets/quad.obj");

        planeCount = plane.getMeshes()[0].indices.size();

        planeVAO.bind();
        planeVBO.bind();
        planeEBO.bind();
        planeVBO.load((float*)(&plane.getMeshes()[0].vertices[0]), sizeof(gfx::Mesh::Vertex) * plane.getMeshes()[0].vertices.size());
        planeEBO.load(plane.getMeshes()[0].indices.data(), sizeof(unsigned int) * plane.getMeshes()[0].indices.size());
        planeVAO.linkVertexBuffer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gfx::Mesh::Vertex), (void*)(offsetof(gfx::Mesh::Vertex, position)));
        planeVAO.linkVertexBuffer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gfx::Mesh::Vertex), (void*)(offsetof(gfx::Mesh::Vertex, normal)));
        planeVAO.linkVertexBuffer(2, 2, GL_FLOAT, GL_FALSE, sizeof(gfx::Mesh::Vertex), (void*)(offsetof(gfx::Mesh::Vertex, uv)));
        planeVAO.unBind();
        planeVBO.unBind();
        planeEBO.unBind();

        plane.free();

        lightTransform.scale = {.1, .1, .1};

        planeTransform.rotation = {glm::half_pi<float>(), 0, 0};
        planeTransform.scale = {5, 5, 5};
        planeTransform.translation = {0, -1, 0};

        cubeTransform.translation = {0, 0, 0};

        lightShader.attachShader("../shaders/vert/shadowLightShader.vert");
        lightShader.attachShader("../shaders/frag/shadowLightShader.frag");
        lightShader.link();

        objectShader.attachShader("../shaders/vert/shadowObjectShader.vert");
        objectShader.attachShader("../shaders/frag/shadowObjectShader.frag");
        objectShader.link();

        shadowMapShader.attachShader("../shaders/vert/shadowMapShader.vert");
        shadowMapShader.attachShader("../shaders/frag/shadowMapShader.frag");
        shadowMapShader.link();

        shadowBuffer.init(2048, 2048, gfx::FrameBufferType::DEPTH);
           
    }
    ~Editor()
    {
        lightShader.free();
        objectShader.free();
        shadowMapShader.free();

        lightVAO.free();
        lightVBO.free();
        lightEBO.free();

        cubeVAO.free();
        cubeVBO.free();
        cubeEBO.free();

        planeVAO.free();
        planeVBO.free();
        planeEBO.free();
    }

    void update(float deltaTime)
    {
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_L) != GLFW_PRESS)
        {
            controller.moveInPlaneXZ(window.getGLFWwindow(), deltaTime, cameraTransform.translation, cameraTransform.rotation);
            editorCamera.setViewYXZ(cameraTransform.translation, cameraTransform.rotation);
        }
        else
        {
            controller.moveInPlaneXZ(window.getGLFWwindow(), deltaTime, lightTransform.translation, lightTransform.rotation);
        }
    }

    void render()
    {
        glCall(glClearColor(0, 0, 0, 1));
        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        glCall(glEnable(GL_DEPTH_TEST));

        static glm::mat4 proj{1};
        static glm::mat4 view{1};
        static glm::mat4 mod{1};

        shadowBuffer.bind();
        shadowMapShader.bind();
        glCall(glClear(GL_DEPTH_BUFFER_BIT));
        glCall(glEnable(GL_DEPTH_TEST));
        static glm::mat4 lightProj = glm::ortho(-10.f, 10.f, -10.f, 10.f, .001f, 100.f);
        glm::mat4 lightView = glm::lookAt(lightTransform.translation * 20.f, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
        glm::mat4 lightSpace = lightProj * lightView;
        shadowMapShader.Mat4f("lightSpace", glm::value_ptr(lightSpace));
        mod = cubeTransform.mat4();
        shadowMapShader.Mat4f("model", glm::value_ptr(mod));
        cubeVAO.bind();
        cubeEBO.bind();
        glCall(glDrawElements(GL_TRIANGLES, cubeCount, GL_UNSIGNED_INT, 0));
        mod = planeTransform.mat4();
        shadowMapShader.Mat4f("model", glm::value_ptr(mod));
        planeVAO.bind();
        planeEBO.bind();
        glCall(glDrawElements(GL_TRIANGLES, planeCount, GL_UNSIGNED_INT, 0));
        int width, height; window.size(width, height); 
        shadowBuffer.unBind(width, height);

        // Editor Camera 
        proj = editorCamera.getProjection();
        view = editorCamera.getView();
        
        // Drawing Light
        mod = lightTransform.mat4();
        lightShader.bind();
        lightShader.Mat4f("proj", glm::value_ptr(proj));
        lightShader.Mat4f("view", glm::value_ptr(view));
        lightShader.Mat4f("model", glm::value_ptr(mod));
        lightShader.vec3f("lightCol", glm::value_ptr(glm::vec3{1, 1, 1}));
        lightVAO.bind();
        lightEBO.bind();
        glCall(glDrawElements(GL_TRIANGLES, cubeCount, GL_UNSIGNED_INT, 0));

        // Drawing Objects
        objectShader.bind();
        shadowBuffer.bindTextureAttachment(gfx::DEPTH, 0);
        objectShader.veci("shadowMap", 0);
        objectShader.Mat4f("lightSpace", glm::value_ptr(lightSpace));
        objectShader.Mat4f("proj", glm::value_ptr(proj));
        objectShader.Mat4f("view", glm::value_ptr(view));
        objectShader.vec3f("lightCol", glm::value_ptr(glm::vec3{1, 1, 1}));
        objectShader.vec3f("lightPos", glm::value_ptr(lightTransform.translation));
        mod = cubeTransform.mat4();
        objectShader.Mat4f("model", glm::value_ptr(mod));
        cubeVAO.bind();
        cubeEBO.bind();
        glCall(glDrawElements(GL_TRIANGLES, cubeCount, GL_UNSIGNED_INT, 0));
        mod = planeTransform.mat4();
        objectShader.Mat4f("model", glm::value_ptr(mod));
        planeVAO.bind();
        planeEBO.bind();
        glCall(glDrawElements(GL_TRIANGLES, planeCount, GL_UNSIGNED_INT, 0));
        

        myImGuiStartFrame();

        ImGui::Image((ImTextureID)(shadowBuffer.getTextureAttachment(gfx::FrameBufferType::DEPTH)), {500, 500}, {0, 1}, {1, 0});

        myImGuiEndFrame();
    
    }


private:
    Camera editorCamera;
    Transform cameraTransform;

    KeyboardMovementController controller;

    gfx::VertexArray lightVAO, cubeVAO, planeVAO;
    gfx::VertexBuffer lightVBO, cubeVBO, planeVBO;
    gfx::ElementBuffer lightEBO, cubeEBO, planeEBO;
    Transform lightTransform, cubeTransform, planeTransform;

    gfx::FrameBuffer shadowBuffer;

    int cubeCount, planeCount;

    gfx::ShaderProgram lightShader, objectShader, shadowMapShader;

    Window &window;
};

#endif