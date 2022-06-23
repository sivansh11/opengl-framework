#ifndef BASIC_RENDERER_H
#define BASIC_REDNERER_H

#include "imgui_setup.h"

#include "gfx/shader.h"
#include "gfx/model.h"
#include "gfx/shader_storage.h"

#include "utils/ecs.h"
#include "utils/components.h"
#include "utils/camera.h"

#include "window.h"


class Renderer
{
public:
    Renderer(ecs::Scene &scene, Window &window) : scene(scene), window(window)
    {
        
    }

    ~Renderer()
    {
        
    }

    void render(ecs::EntityID camera)
    {

    }

    void imguiRender()
    {
        
    }

private:
    ecs::Scene &scene;
    Window &window;
};

#endif
