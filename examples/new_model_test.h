#ifndef NEW_MODEL_TEST_H
#define NEW_MODEL_TEST_H

#include "gfx/shader.h"
#include "gfx/model.h"
#include "gfx/shader_storage.h"

#include "utils/ecs.h"
#include "utils/components.h"
#include "utils/camera.h"

namespace example
{

class NewModelTest
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
    NewModelTest(ecs::Scene &scene) : scene(scene)
    {
        model.loadModelFromPath("../assets/Cat_v1_L3.123cb1b1943a-2f48-4e44-8f71-6bbe19a3ab64/12221_Cat_v1_l3.obj");

        light.loadModelFromPath("../assets/cube.obj");

        lightShader.attachShader("../shaders/vert/simple_light.vert");
        lightShader.attachShader("../shaders/frag/simple_light.frag");
        lightShader.link();

        shader.attachShader("../shaders/vert/new_mesh_test.vert");
        shader.attachShader("../shaders/frag/new_mesh_test.frag");
        shader.link();

        modelTransform.scale = {.1, .1, .1};

        lightTransform.scale = {.01, .01, .01};

        lightBuffer.lightPos = lightTransform.translation;
        lightSSBO.init(&lightBuffer, sizeof(lightBuffer));

        cameraSSBO.init(&cameraBuffer, sizeof(cameraBuffer));
    }
    ~NewModelTest()
    {
        model.free();
        shader.free();
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
        light.draw(lightShader);

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
        model.draw(shader);
    }

    void imguiRender()   
    {

    }

private:
    ecs::Scene &scene;

    gfx::ShaderProgram shader;
    gfx::ShaderProgram lightShader;
    gfx::Model model{};
    gfx::Model light{};
    Transform modelTransform{};
    Transform lightTransform{};
    gfx::ShaderStorage lightSSBO{};
    gfx::ShaderStorage cameraSSBO{};
    LightBuffer lightBuffer{};
    CameraBuffer cameraBuffer{};
};

} // namespace example


#endif