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
        scene.get<gfx::Model>(light).transform.scale = {.05, .05, .05};
        scene.get<gfx::Model>(light).transform.translation = {0, 0, 0};
        scene.assign<Light>(light);
        
        // model = scene.newEntity();
        // scene.assign<gfx::Model>(model).loadModelFromPath("../assets/glTF/Sponza.gltf");
        // scene.get<gfx::Model>(model).getMeshes()[0].material.ambient = glm::vec3{.1};
        // scene.get<gfx::Model>(model).getMeshes()[0].material.diffuse = glm::vec3{.5};
        // scene.get<gfx::Model>(model).getMeshes()[0].material.specular = glm::vec3{.5};
        // scene.get<gfx::Model>(model).getMeshes()[0].material.shininess = 32;
        // scene.get<gfx::Model>(model).transform.scale = {.01, .01, .01};
        // scene.get<gfx::Model>(model).transform.translation = {0, 0, 2};
        // scene.assign<Object>(model);


        model = scene.newEntity();
        scene.assign<gfx::Model>(model).loadModelFromPath("../assets/low_poly_tree/Lowpoly_tree_sample.obj", false);
        scene.get<gfx::Model>(model).transform.translation = {0, 0, 2};
        scene.get<gfx::Model>(model).transform.scale = {0.1, 0.1, 0.1};
        scene.assign<Object>(model);

        model = scene.newEntity();
        scene.assign<gfx::Model>(model).loadModelFromPath("../assets/quad.obj", false);
        scene.get<gfx::Model>(model).getMeshes()[0].material.ambient = glm::vec3{1};
        scene.get<gfx::Model>(model).getMeshes()[0].material.diffuse = glm::vec3{1};
        scene.get<gfx::Model>(model).getMeshes()[0].material.specular = glm::vec3{1};
        scene.get<gfx::Model>(model).transform.scale = {1, 1, 1};
        scene.get<gfx::Model>(model).transform.translation = {0, 0, 2};
        scene.get<gfx::Model>(model).transform.rotation = {glm::half_pi<float>(), 0, 0};
        scene.assign<Object>(model);

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
            controller.moveInPlaneXZ(window.getGLFWwindow(), deltaTime, scene.get<gfx::Model>(light).transform.translation, scene.get<gfx::Model>(light).transform.rotation);
        }
    }

    void render()
    {
        glCall(glClearColor(0, 0, 0, 1));
        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        glCall(glEnable(GL_DEPTH_TEST));

        renderer.render(editorCamera);

        myImGuiStartFrame();

        ImGui::Begin("NOTE");
        ImGui::Text("PRESS L ALONG WITH AWSD TO MOVE LIGHT");
        ImGui::End();

        ImGui::Begin("light");
        ImGui::DragFloat3("ambient", (float*)&scene.get<Light>(light).ambient, .01);
        ImGui::DragFloat3("diffuse", (float*)&scene.get<Light>(light).diffuse, .01);
        ImGui::DragFloat3("specular", (float*)&scene.get<Light>(light).specular, .01);
        ImGui::End();

        static ecs::EntityID selectedEnt;
        static int selectedIdx;
        static bool init = false;

        ImGui::Begin("Scene Entities");
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Objects"))
        {
            for (auto ent: ecs::SceneView<Object>(scene))
            {
                if (ImGui::TreeNode((void*)(ent), "%lld", ent))
                {
                    init = true;
                    selectedEnt = ent;  
                    if(ImGui::IsItemClicked())
                    {
                        selectedIdx = -1;
                    }
                    std::vector<std::string> items;
                    for (int i = 0; i < scene.get<gfx::Model>(ent).getMeshes().size(); i++)
                    {
                        items.push_back(std::to_string(i));                        
                    }
                    static int item_current_idx = 0; 
                    for (int i = 0; i < items.size(); i++)
                    {
                        const bool is_selected = (item_current_idx == i);
                        if (ImGui::Selectable(items[i].c_str(), is_selected))
                        {
                            item_current_idx = i;
                            selectedIdx = i;
                        }
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
        ImGui::End();

        ImGui::Begin("Entity Data");
        if (init)
        {
            if (selectedIdx >= 0)
            {
                // mesh
                ImGui::Text("Material");
                gfx::Material &mat = scene.get<gfx::Model>(selectedEnt).getMeshes()[selectedIdx].material;
                ImGui::DragFloat3("ambient", (float*)(&mat.ambient), 0.01);
                ImGui::DragFloat3("diffuse", (float*)(&mat.diffuse), 0.01);
                ImGui::DragFloat3("specular", (float*)(&mat.specular), 0.01);
                ImGui::DragFloat("shininess", &mat.shininess, 1, 1, 64);
                ImGui::Text("Transform");
                Transform &transform = scene.get<gfx::Model>(selectedEnt).getMeshes()[selectedIdx].transform; 
                ImGui::DragFloat3("translation", (float*)(&transform.translation), 0.01);
                ImGui::DragFloat3("scale", (float*)(&transform.scale), 0.01);
                ImGui::DragFloat3("rotation", (float*)(&transform.rotation), 0.01);
            }
            else
            {   
                // model
                ImGui::Text("Material");
                gfx::Material &mat = scene.get<gfx::Model>(selectedEnt).material;
                ImGui::DragFloat3("ambient", (float*)(&mat.ambient), 0.01);
                ImGui::DragFloat3("diffuse", (float*)(&mat.diffuse), 0.01);
                ImGui::DragFloat3("specular", (float*)(&mat.specular), 0.01);
                ImGui::DragFloat("shininess", &mat.shininess, 1, 1, 64);
                ImGui::Text("Transform");
                Transform &transform = scene.get<gfx::Model>(selectedEnt).transform; 
                ImGui::DragFloat3("translation", (float*)(&transform.translation), 0.01);
                ImGui::DragFloat3("scale", (float*)(&transform.scale), 0.01);
                ImGui::DragFloat3("rotation", (float*)(&transform.rotation), 0.01);

            }

        }
        ImGui::End();

        // ImGui::ShowDemoWindow();

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