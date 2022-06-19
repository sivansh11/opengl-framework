#ifndef SIMPLE_RENDERER_H
#define SIMPLE_RENDERER_H

#include "utils/camera.h"
#include "utils/components.h"
#include "utils/ecs.h"

#include "gfx/shader.h"

class SimpleRenderer
{
public:
    SimpleRenderer()
    {
        shaderProgram.attachShader("../shaders/vert/simple_shader_with_camera.vert");
        shaderProgram.attachShader("../shaders/frag/simple_shader_with_camera.frag");
        shaderProgram.link();
    }

    void render(ecs::Scene &scene, ecs::EntityID &camera_)
    {
        Camera &camera = scene.get<Camera>(camera_);
        static glm::mat4 proj{};
        proj = camera.getProjection(); 
        static glm::mat4 view{};
        view = camera.getView(); 
        static glm::mat4 model{1};
        shaderProgram.bind();

        for (auto obj: ecs::SceneView<gfx::Mesh, Transform>(scene))
        {
            gfx::Mesh& mesh = scene.get<gfx::Mesh>(obj);
            Transform &transform = scene.get<Transform>(obj);
            model = transform.mat4();
            shaderProgram.Mat4f("proj", glm::value_ptr(proj));
            shaderProgram.Mat4f("view", glm::value_ptr(view));
            shaderProgram.Mat4f("model", glm::value_ptr(model));
            mesh.bind();
            mesh.draw();
        }
    }

private:
    gfx::ShaderProgram shaderProgram{};
};



#endif