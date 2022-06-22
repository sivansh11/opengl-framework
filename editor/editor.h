#ifndef EDITOR_H
#define EDITOR_H

#include "basic_renderer.h"

#include "imgui_setup.h"
#include "window.h"

#include "utils/camera.h"
#include "utils/ecs.h"
#include "utils/controller.h"
#include "utils/components.h"

#include "gfx/model.h"

#include "event.h"

class Editor
{
public:
    Editor(Window &window) : window(window), renderer(scene)
    {
        editorCamera = scene.newEntity();
        int width, height; window.size(width, height);
        scene.assign<Camera>(editorCamera);
        scene.assign<Transform>(editorCamera).translation = {0, 0, -2};

        scene.get<Camera>(editorCamera).setPerspectiveProjection(45, 1200.f/800.f, 0.001, 100);

        light = scene.newEntity();
        scene.assign<gfx::Model>(light).loadModelFromPath("../assets/cube.obj", false);
        scene.assign<Transform>(light).scale = {.05, .05, .05};
        scene.get<Transform>(light).translation = {0, 0, 0};
        scene.assign<Light>(light);
        
        // model = scene.newEntity();
        // scene.assign<gfx::Model>(model).loadModelFromPath("../assets/floatcity/Scifi Floating City/Scifi Floating City.obj");
        // scene.assign<Transform>(model).scale = {.01, .01, .01};
        // scene.get<Transform>(model).translation = {0, 0, 2};
        // scene.assign<gfx::Material>(model);

        gfx::Model m;
        m.loadModelFromPath("../assets/quad.obj", false);
        model = scene.newEntity();
        // scene.assign<gfx::Mesh>(model) = m.meshes[0];
        std::vector<gfx::Texture2D> tex;
        tex.push_back(gfx::Texture2D("../textures/brickwall_diffuse.jpg", "diffuse"));
        tex.push_back(gfx::Texture2D("../textures/brickwall_normal.jpg", "normal"));
        scene.assign<gfx::Mesh>(model).load(m.meshes[0].vertices, m.meshes[0].indices, tex);
        scene.assign<Transform>(model);
        scene.assign<gfx::Material>(model);

        m.free();
    }
    ~Editor()
    {
        for (auto ent: ecs::SceneView<gfx::Model>(scene))
        {
            scene.get<gfx::Model>(ent).free();
        }
    }

    void update(float deltaTime)
    {
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_L) != GLFW_PRESS)
        {
            controller.moveInPlaneXZ(window.getGLFWwindow(), deltaTime, scene.get<Transform>(editorCamera).translation, scene.get<Transform>(editorCamera).rotation);
            scene.get<Camera>(editorCamera).setViewYXZ(scene.get<Transform>(editorCamera).translation, scene.get<Transform>(editorCamera).rotation);
        }
        else
        {
            controller.moveInPlaneXZ(window.getGLFWwindow(), deltaTime, scene.get<Transform>(light).translation, scene.get<Transform>(light).rotation);
        }
    }

    void render()
    {
        glCall(glClearColor(0, 0, 0, 1));
        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        glCall(glEnable(GL_DEPTH_TEST));

        renderer.render(editorCamera);

        myImGuiStartFrame();

        ImGui::Text("NOTE: PRESS L ALONG WITH AWSD TO MOVE LIGHT");

        ImGui::Begin("light");
        ImGui::DragFloat3("ambient", (float*)&scene.get<Light>(light).ambient, .01);
        ImGui::DragFloat3("diffuse", (float*)&scene.get<Light>(light).diffuse, .01);
        ImGui::DragFloat3("specular", (float*)&scene.get<Light>(light).specular, .01);
        ImGui::End();

        renderer.imguiRender();

        myImGuiEndFrame();
        
    }

private:
    Renderer renderer;

    ecs::Scene scene;
    ecs::EntityID editorCamera;
    ecs::EntityID light;
    ecs::EntityID model;
    
    Dispatcher dispatcher{};

    KeyboardMovementController controller;

    Window &window;
};

#endif