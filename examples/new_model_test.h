#ifndef NEW_MODEL_TEST_H
#define NEW_MODEL_TEST_H

#include "gfx/vao.h"
#include "gfx/vbo.h"
#include "gfx/ebo.h"

#include "gfx/shader.h"

#include "gfx/model.h"

#include "utils/ecs.h"

namespace example
{

class NewModelTest
{
public:
    NewModelTest(ecs::Scene &scene) : scene(scene)
    {
        gfx::Model model;
        model.loadModelFromPath("../assets/backpack/backpack.obj");
        
    }

    void render(ecs::EntityID &camera)
    {

    }

    void imguiRender()   
    {

    }

private:
    ecs::Scene &scene;
};

} // namespace example


#endif