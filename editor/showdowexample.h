#ifndef SHADOW_EXAMPLE_H
#define SHADOW_EXAMPLE_H

#include "window.h"

#include "imgui_setup.h"

#include "utils/shader_loader.h"
#include "utils/camera.h"
#include "utils/ecs.h"
#include "utils/controller.h"
#include "utils/components.h"

#include "gfx/shader.h"
#include "gfx/model.h"
#include "gfx/framebuffer.h"

class Editor
{
public:
    Editor(Window &window) : window(window)
    {
        myImGuiSetup(window);
        // static ImVec3 color_for_text = ImVec3(float(255) / 255,float(255) / 255,float(255) / 255);
        // static ImVec3 color_for_head = ImVec3(float(80) / 255,float(80) / 255,float(80) / 255);
        // static ImVec3 color_for_area = ImVec3(float(50) / 255,float(50) / 255,float(50) / 255);
        // static ImVec3 color_for_body = ImVec3(float(100) / 255,float(100) / 255,float(100) / 255);
        // static ImVec3 color_for_pops = ImVec3(float(45) / 255,float(45) / 255,float(45) / 255);
        // static ImVec3 color_for_tabs = ImVec3(float(0) / 255,float(0) / 255,float(0) / 255);
        // imgui_easy_theming(color_for_text, color_for_head, color_for_area, color_for_body, color_for_pops, color_for_tabs);

        editorCamera = scene.newEntity();
        int width, height; window.size(width, height);
        scene.assign<Camera>(editorCamera);
        scene.assign<Transform>(editorCamera).translation = {0, .5, -2};
        scene.get<Camera>(editorCamera).setPerspectiveProjection(45, 1200.f/800.f, 0.001, 100);

        
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
    }

    void render()
    {
        glCall(glClearColor(0, 0, 0, 1));
        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        glCall(glEnable(GL_DEPTH_TEST));
        glCall(glEnable(GL_CULL_FACE));
        glCall(glCullFace(GL_FRONT));

        static glm::mat4 proj{1};
        static glm::mat4 view{1};

        proj = scene.get<Camera>(editorCamera).getProjection();
        view = scene.get<Camera>(editorCamera).getView();


        
    }

private:
    ecs::Scene scene;
    ecs::EntityID editorCamera;
    ecs::EntityID light;
    ecs::EntityID model;
    
    Dispatcher dispatcher{};

    KeyboardMovementController controller;
    
    gfx::ShaderProgram shadowMapShader;

    Window &window;
};

#endif