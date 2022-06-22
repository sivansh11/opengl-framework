#ifndef BASIC_RENDERER_H
#define BASIC_REDNERER_H

#include "imgui_setup.h"

#include "gfx/shader.h"
#include "gfx/model.h"
#include "gfx/shader_storage.h"

#include "utils/ecs.h"
#include "utils/components.h"
#include "utils/camera.h"

struct Light 
{
    glm::vec3 ambient{1};
    glm::vec3 diffuse{3};
    glm::vec3 specular{1};
};

class Renderer
{
public:
    Renderer(ecs::Scene &scene) : scene(scene)
    {
        lightShader.attachShader("../shaders/vert/light.vert");
        lightShader.attachShader("../shaders/frag/light.frag");
        lightShader.link();

        shader.attachShader("../shaders/vert/object.vert");
        shader.attachShader("../shaders/frag/object.frag");
        shader.link();

    }

    ~Renderer()
    {
        lightShader.free();
        shader.free();
    }

    void render(ecs::EntityID camera)
    {
        static glm::mat4 proj{1};
        static glm::mat4 view{1};
        static glm::mat4 mod{1};

        ecs::EntityID light;

        proj = scene.get<Camera>(camera).getProjection();
        view = scene.get<Camera>(camera).getView();

        lightShader.bind();
        for (auto ent: ecs::SceneView<Light>(scene))
        {
            auto& model = scene.get<gfx::Model>(ent);
            auto& transform = scene.get<Transform>(ent);
            mod = transform.mat4();
            lightShader.Mat4f("proj", glm::value_ptr(proj));    
            lightShader.Mat4f("view", glm::value_ptr(view));    
            lightShader.Mat4f("model", glm::value_ptr(mod));
            lightShader.vec3f("lightCol", glm::value_ptr(scene.get<Light>(ent).ambient));
            model.draw(lightShader);
            light = ent;
        }   

        shader.bind();
        for (auto ent: ecs::SceneView<gfx::Material>(scene))
        {
            auto& model = scene.get<gfx::Model>(ent);
            auto& transform = scene.get<Transform>(ent);
            auto& material = scene.get<gfx::Material>(ent);
            mod = transform.mat4();
            shader.Mat4f("proj", glm::value_ptr(proj));    
            shader.Mat4f("view", glm::value_ptr(view));    
            shader.Mat4f("model", glm::value_ptr(mod));
            shader.vec3f("light.lightPos", glm::value_ptr(scene.get<Transform>(light).translation));
            shader.vec3f("light.ambient", glm::value_ptr(scene.get<Light>(light).ambient));
            shader.vec3f("light.diffuse", glm::value_ptr(scene.get<Light>(light).diffuse));
            shader.vec3f("light.specular", glm::value_ptr(scene.get<Light>(light).specular));
            shader.vec3f("cameraPos", glm::value_ptr(scene.get<Transform>(camera).translation));
            model.draw(shader);
        }

    }

    void imguiRender()
    {
        ImGui::Begin("RenderInfo");
        ImGui::Text("%.2f ms", ImGui::GetIO().Framerate);
        ImGui::End();
    }

private:
    ecs::Scene &scene;

    gfx::ShaderProgram lightShader{};
    gfx::ShaderProgram shader{};

};

#endif
