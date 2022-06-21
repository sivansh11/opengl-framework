#ifndef LIT_MODEL_H
#define LIT_MODEL_H

#include "gfx/shader.h"
#include "gfx/model.h"
#include "gfx/shader_storage.h"

#include "utils/camera.h"
#include "utils/components.h"
#include "utils/controller.h"
#include "utils/ecs.h"

#include "imgui_setup.h"

namespace example
{

class LitModel
{
    struct LightBuffer
    {
        alignas(16) glm::vec3 lightPos;
        alignas(16) glm::vec3 lightCol{1, 1, 1};
    };
    struct CameraBuffer
    {
        alignas(16) glm::vec3 cameraPos{0};
    };
public:
    LitModel(ecs::Scene &scene) : scene(scene)
    {
        shader.attachShader("../shaders/vert/lit_model.vert");   
        shader.attachShader("../shaders/frag/lit_model.frag");   
        shader.link();

        lightShader.attachShader("../shaders/vert/lit_model_light.vert");
        lightShader.attachShader("../shaders/frag/lit_model_light.frag");
        lightShader.link();

        model.loadModelFromPath("../assets/flat_vase.obj");
        modelTransform.rotation = {glm::pi<float>(), 0, 0};
        light.loadModelFromPath("../assets/cube.obj");

        lightTransform.scale = {.01f, .01f, .01f};

        lightBuffer.lightPos = lightTransform.translation;
        lightSSBO.init(&lightBuffer, sizeof(lightBuffer));

        cameraSSBO.init(&cameraBuffer, sizeof(cameraBuffer));
    }

    void render(ecs::EntityID &camera)
    {
        static glm::mat4 proj{1};
        static glm::mat4 view{1};
        static glm::mat4 mod{1};

        proj = scene.get<Camera>(camera).getProjection();
        view = scene.get<Camera>(camera).getView();

        lightShader.bind();
        mod = lightTransform.mat4();
        lightShader.Mat4f("proj", glm::value_ptr(proj));    
        lightShader.Mat4f("view", glm::value_ptr(view));    
        lightShader.Mat4f("model", glm::value_ptr(mod));
        light.bind();
        light.draw();

        shader.bind();
        lightSSBO.bind(0);
        lightBuffer.lightPos = lightTransform.translation;
        lightSSBO.update_gpu(&lightBuffer, sizeof(lightBuffer));
        cameraSSBO.bind(1);
        cameraBuffer.cameraPos = scene.get<Transform>(camera).translation;
        cameraSSBO.update_gpu(&cameraBuffer, sizeof(cameraBuffer));
        mod = modelTransform.mat4();
        shader.Mat4f("proj", glm::value_ptr(proj));    
        shader.Mat4f("view", glm::value_ptr(view));    
        shader.Mat4f("model", glm::value_ptr(mod));    
        model.bind();
        model.draw();
    }

    void imguiRender()
    {
        ImGui::Begin("info");
        ImGui::Text("%.2f fps", ImGui::GetIO().Framerate);
        ImGui::DragFloat3("light pos", (float*)&lightTransform.translation);
        ImGui::Text("%.2f %.2f %.2f", cameraBuffer.cameraPos.x, cameraBuffer.cameraPos.y, cameraBuffer.cameraPos.z);
        ImGui::End();
    }

private:
    gfx::ShaderProgram shader{};
    gfx::ShaderProgram lightShader{};
    gfx::Model model{};
    gfx::Model light{};
    gfx::ShaderStorage lightSSBO{};
    gfx::ShaderStorage cameraSSBO{};
    LightBuffer lightBuffer{};
    CameraBuffer cameraBuffer{};
    Transform modelTransform{};
    Transform lightTransform{};
    KeyboardMovementController controller;
    ecs::Scene &scene;
};

} // namespace example

#endif