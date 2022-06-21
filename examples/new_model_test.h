#ifndef NEW_MODEL_TEST_H
#define NEW_MODEL_TEST_H

#include "gfx/vao.h"
#include "gfx/vbo.h"
#include "gfx/ebo.h"
#include "gfx/shader.h"
#include "gfx/model.h"

#include "utils/ecs.h"
#include "utils/components.h"
#include "utils/camera.h"

namespace example
{

class NewModelTest
{
public:
    NewModelTest(ecs::Scene &scene) : scene(scene)
    {
        model.loadModelFromPath("../assets/Cat_v1_L3.123cb1b1943a-2f48-4e44-8f71-6bbe19a3ab64/12221_Cat_v1_l3.obj");

        shader.attachShader("../shaders/vert/new_mesh_test.vert");
        shader.attachShader("../shaders/frag/new_mesh_test.frag");
        shader.link();

    }

    void render(ecs::EntityID &camera)
    {
        static glm::mat4 proj{1};
        static glm::mat4 view{1};
        static glm::mat4 mod{1};

        proj = scene.get<Camera>(camera).getProjection();
        view = scene.get<Camera>(camera).getView();

        shader.bind();
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

    gfx::ShaderProgram shader{};
    gfx::Model model{};
    Transform modelTransform{};
};

} // namespace example


#endif