#ifndef EDITOR_H
#define EDITOR_H

#include "lit_model.h"

#include "imgui_setup.h"

#include "window.h"

#include "gfx/framebuffer.h"

#include "utils/camera.h"
#include "utils/ecs.h"
#include "event.h"
#include "utils/controller.h"

class Editor
{
public:
    Editor(Window &window) : window(window), renderer{scene}
    {
        editorCamera = scene.newEntity();
        int width, height; window.size(width, height);
        scene.assign<Camera>(editorCamera);
        scene.assign<Transform>(editorCamera);

        gfx::FramebufferSpecification fbSpec;
        fbSpec.width = 300;
        fbSpec.height = 200;
        fbSpec.attachments.push_back(gfx::Attachments::Color);
        fbSpec.attachments.push_back(gfx::Attachments::DepthStencil);
        editorView.create(fbSpec);

        scene.get<Camera>(editorCamera).setPerspectiveProjection(45, 300.f/200.f, 0.001, 100);
    }
    ~Editor()
    {

    }

    void update(float deltaTime)
    {
        controller.moveInPlaneXZ(window.getGLFWwindow(), deltaTime, scene.get<Transform>(editorCamera).translation, scene.get<Transform>(editorCamera).rotation);
        scene.get<Camera>(editorCamera).setViewYXZ(scene.get<Transform>(editorCamera).translation, scene.get<Transform>(editorCamera).rotation);
    }

    void render()
    {
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        renderer.render(editorCamera);

        myImGuiStartFrame();
        renderer.imguiRender();
        myImGuiEndFrame();
        
    }

private:
    example::LitModel renderer;

private:
    ecs::Scene scene;
    ecs::EntityID editorCamera;
    
    Dispatcher dispatcher{};

    KeyboardMovementController controller;
    gfx::Framebuffer editorView{};

    Window &window;
};

#endif