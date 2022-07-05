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

struct Light  
{
    enum
    {
        POINTLIGHT,
        DIRECTIONAL,
        SPOTLIGHT
    };
    int type;  // 0 is pointlight, 1 is directional, 2 is spotlight
    glm::vec3 lightDir{1, 1, 0};

    float linear = .0;
    float quadratic = .0;
};
struct Object {};

struct Shadow {};

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

        light = scene.newEntity();
        scene.assign<gfx::Model>(light).defaultTextures = false;
        scene.get<gfx::Model>(light).loadModelFromPath("../assets/cube.obj");
        scene.get<gfx::Model>(light).material.ambient = {.3, .3, .3};
        scene.get<gfx::Model>(light).material.diffuse = {.5, .5, .5};
        scene.get<gfx::Model>(light).material.specular = {.3, .3, .3};
        scene.get<gfx::Model>(light).transform.scale = {.01, .01, .01};
        scene.get<gfx::Model>(light).transform.translation = {0, 0, 0};
        scene.assign<Light>(light);
        scene.get<Light>(light).type = Light::POINTLIGHT;

        model = scene.newEntity();
        scene.assign<gfx::Model>(model).loadModelFromPath("../assets/low_poly_tree/Lowpoly_tree_sample.obj");
        scene.get<gfx::Model>(model).transform.scale = {.1, .1, .1};
        scene.assign<Object>(model);
        scene.assign<Shadow>(model);

        model = scene.newEntity();
        scene.assign<gfx::Model>(model).loadModelFromPath("../assets/quad.obj");
        scene.get<gfx::Model>(model).transform.rotation = {-glm::half_pi<float>(), 0, 0};
        scene.assign<Object>(model);
        scene.assign<Shadow>(model);
        // model = scene.newEntity();
        // scene.assign<gfx::Model>(model).loadModelFromPath("../assets/PKG_A_Curtains/NewSponza_Curtains_glTF.gltf");
        // scene.assign<Object>(model);

        ShaderLoaderVert.init("../shaders/vert/shader.vert");
        ShaderLoaderFrag.init("../shaders/frag/shader.frag");

        std::string shaderCodeVert = ShaderLoaderVert.load();

        lightShader.attachShader(shaderCodeVert, gfx::VERTEX);
        lightShader.attachShader("../shaders/frag/light.frag");
        lightShader.link();

        std::string shaderCodeFrag = ShaderLoaderFrag.load();

        std::istringstream shader{shaderCodeFrag};
        std::string line;
        int i = 0;
        while (std::getline(shader, line))
        {
            // std::cout << i++ << ": " << line << '\n';
        }

        objectShader.attachShader(shaderCodeVert, gfx::VERTEX);
        objectShader.attachShader(shaderCodeFrag, gfx::FRAGMENT);
        objectShader.link();

        shadowBuffer.init(2048, 2048, gfx::FrameBufferType::DEPTH);

        shadowShader.attachShader("../shaders/vert/shadow.vert");
        shadowShader.attachShader("../shaders/frag/shadow.frag");
        shadowShader.link();
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
            // scene.get<gfx::Model>(light).transform.translation = scene.get<Transform>(editorCamera).translation - glm::vec3{0,-1,0};
        }   
        else
        {
            controller.moveInPlaneXZ(window.getGLFWwindow(), deltaTime, scene.get<gfx::Model>(light).transform.translation, scene.get<gfx::Model>(light).transform.rotation);
        }
    }

    void render()
    {
        glCall(glClearColor(0.7, 0.8, 1, 1));
        glCall(glClearColor(0, 0, 0, 1));
        glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        glCall(glEnable(GL_DEPTH_TEST));
        glCall(glEnable(GL_CULL_FACE));
        glCall(glCullFace(GL_FRONT));

        static glm::mat4 proj{1};
        static glm::mat4 view{1};

        proj = scene.get<Camera>(editorCamera).getProjection();
        view = scene.get<Camera>(editorCamera).getView();

        
        shadowBuffer.bind();
        glCall(glClear(GL_DEPTH_BUFFER_BIT));
        // todo: write a shadow shader
        // bind shadow shader
        shadowShader.bind();
        // static glm::mat4 lightProj = glm::ortho(-10.f, 10.f, -10.f, 10.f, 1.f, 7.5f);
        // glm::mat4 lightView = glm::lookAt(scene.get<gfx::Model>(light).transform.translation * 5.f, glm::vec3{0, 0, 0}, glm::vec3{0, 1, 0});
        // glm::mat4 lightSpace = lightProj * lightView;
        static glm::mat4 lightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, 1.f, 7.5f);
        static glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), 
                                  glm::vec3( 0.0f, 0.0f,  0.0f), 
                                  glm::vec3( 0.0f, 1.0f,  0.0f));  
        static glm::mat4 lightSpace = lightProjection * lightView;

        shadowShader.Mat4f("lightSpace", glm::value_ptr(lightSpace));
        for (auto ent: ecs::SceneView<Shadow>(scene))
        {
            scene.get<gfx::Model>(ent).draw(shadowShader, nullptr);
        }

        int width, height; window.size(width, height);
        shadowBuffer.unBind(width, height);


        lightShader.bind();
        lightShader.Mat4f("proj", glm::value_ptr(proj));
        lightShader.Mat4f("view", glm::value_ptr(view));
        lightShader.vec3f("lightCol", glm::value_ptr(scene.get<gfx::Model>(light).material.diffuse));
        scene.get<gfx::Model>(light).draw(lightShader, nullptr);

        objectShader.bind();
        objectShader.Mat4f("proj", glm::value_ptr(proj));
        objectShader.Mat4f("view", glm::value_ptr(view));
        objectShader.vec3f("light.lightPos", glm::value_ptr(scene.get<gfx::Model>(light).transform.translation));
        objectShader.vec3f("light.ambient", glm::value_ptr(scene.get<gfx::Model>(light).material.ambient));
        objectShader.vec3f("light.diffuse", glm::value_ptr(scene.get<gfx::Model>(light).material.diffuse));
        objectShader.vec3f("light.specular", glm::value_ptr(scene.get<gfx::Model>(light).material.specular));
        objectShader.veci("light.type", scene.get<Light>(light).type);
        shadowBuffer.bindTextureAttachment(gfx::DEPTH, 3);
        objectShader.veci("shadowMap", 3);
        if (scene.get<Light>(light).type == Light::POINTLIGHT)
        {
            objectShader.vecf("light.linear", scene.get<Light>(light).linear);
            objectShader.vecf("light.quadratic", scene.get<Light>(light).quadratic);
        }
        if (scene.get<Light>(light).type == Light::DIRECTIONAL)
        {
            objectShader.vec3f("light.lightDir", glm::value_ptr(scene.get<Light>(light).lightDir));
        }
        objectShader.vec3f("cameraPos", glm::value_ptr(scene.get<Transform>(editorCamera).translation));
        for (auto ent: ecs::SceneView<Object>(scene))
        {
            scene.get<gfx::Model>(ent).draw(objectShader, &scene.get<gfx::Model>(ent).material);
        }

        myImGuiStartFrame();
        
        ImGui::Begin("RenderInfo");
        ImGui::Text("%.2f ms", ImGui::GetIO().Framerate);
        ImGui::End();

        static ecs::EntityID selectedEnt;
        static int selectedIdx;
        static bool init = false;

        ImGui::Image((ImTextureID)(shadowBuffer.getTextureAttachment(gfx::FrameBufferType::DEPTH)), {500, 500}, {0, 1}, {1, 0});

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

        ImGui::Begin("light");
        ImGui::SliderInt("type", &scene.get<Light>(light).type, 0, 1);
        ImGui::DragFloat3("ambient", (float*)&scene.get<gfx::Model>(light).material.ambient, .01);
        ImGui::DragFloat3("diffuse", (float*)&scene.get<gfx::Model>(light).material.diffuse, .01);
        ImGui::DragFloat3("specular", (float*)&scene.get<gfx::Model>(light).material.specular, .01);
        if (scene.get<Light>(light).type == Light::POINTLIGHT)
        {
            ImGui::DragFloat("linear", &scene.get<Light>(light).linear, .1, 0, 5);
            ImGui::DragFloat("quadratic", &scene.get<Light>(light).quadratic, .1, 0, 5);
        }
        if (scene.get<Light>(light).type == Light::DIRECTIONAL)
        {
            ImGui::DragFloat3("light direction", (float*)&scene.get<Light>(light).lightDir, .01);
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

        myImGuiEndFrame();
    }

private:
    ecs::Scene scene;
    ecs::EntityID editorCamera;
    ecs::EntityID light;
    ecs::EntityID model;
    
    Dispatcher dispatcher{};

    KeyboardMovementController controller;

    gfx::ShaderProgram lightShader, objectShader;
    utils::ShaderLoader ShaderLoaderFrag;
    utils::ShaderLoader ShaderLoaderVert;

    gfx::ShaderProgram shadowShader;

    gfx::FrameBuffer shadowBuffer;
    
    Window &window;
};

#endif