#include "app.h"

#include "imgui_setup.h"
#include "gfx/texture.h"
#include "gfx/shader.h"
#include "gfx/model.h"
#include "gfx/mesh.h"
#include "gfx/shader_storage.h"

#include "utils/bvh.h"
#include "utils/camera.h"
#include "utils/ecs.h"
#include "utils/controller.h"
#include "utils/utils.h"

#include "renderer/simple_renderer.h"

App::App() : window(width, height, title.c_str())
{
    myImGuiSetup(window);
    static ImVec3 color_for_text = ImVec3(float(255) / 255,float(255) / 255,float(255) / 255);
    static ImVec3 color_for_head = ImVec3(float(80) / 255,float(80) / 255,float(80) / 255);
    static ImVec3 color_for_area = ImVec3(float(50) / 255,float(50) / 255,float(50) / 255);
    static ImVec3 color_for_body = ImVec3(float(100) / 255,float(100) / 255,float(100) / 255);
    static ImVec3 color_for_pops = ImVec3(float(45) / 255,float(45) / 255,float(45) / 255);
    static ImVec3 color_for_tabs = ImVec3(float(0) / 255,float(0) / 255,float(0) / 255);
    imgui_easy_theming(color_for_text, color_for_head, color_for_area, color_for_body, color_for_pops, color_for_tabs);

}

App::~App()
{
    myImGuiBye();
}

void App::run()
{
    ecs::Scene scene;

    auto camera = scene.newEntity();
    scene.assign<Camera>(camera).setPerspectiveProjection(90.0f, 1.0f, .01f, 100.0f);
    KeyboardMovementController controller;
    scene.assign<Transform>(camera);

    SimpleRenderer renderer;

    gfx::Mesh mesh{};
    mesh.loadModelFromPath("../assets/flat_vase.obj");

    auto obj = scene.newEntity();    
    scene.assign<gfx::Mesh>(obj).loadModelFromPath("../assets/flat_vase.obj");
    scene.assign<Transform>(obj).translation = {0, 0, 3};

    while (!window.shouldClose())
    {
        window.startFrame();

        glClearColor(.1, .1, .1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);  

        auto& transform = scene.get<Transform>(camera);

        controller.moveInPlaneXZ(window.getGLFWwindow(), 0.01, transform.translation, transform.rotation);
        auto& cam = scene.get<Camera>(camera); 
        cam.setViewYXZ(transform.translation, transform.rotation);

        renderer.render(scene, camera);

        myImGuiStartFrame();

        ImGui::Begin("info");
        ImGui::Text("%.2f fps", ImGui::GetIO().Framerate);
        ImGui::End();

        myImGuiEndFrame();

        window.endFrame();
    }
    
}