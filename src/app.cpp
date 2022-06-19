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

struct PayLoad
{
    /* data */
};


struct Box
{
    Box(vec3 min, vec3 max) : box{min, max} {}
    bvh::BoundingBox box;

    static void intersect(Box &box, bvh::Ray<PayLoad> &ray)
    {
        float intersect = box.box.intersect(ray);
        if (intersect < ray.t)
        {
            ray.t = intersect;
        }
    }

    static void boundingBox(Box &box, vec3 &min, vec3 &max)
    {
        min = box.box.min;
        max = box.box.max;
    }

    static vec3 centroid(Box &box)
    {
        return (box.box.min + box.box.max) / 2.0f;
    }
};

void App::run()
{
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,   0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f,  -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f,  -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,   0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }; 

    Camera camera;
    camera.setPerspectiveProjection(90.0f, 1.0f, .01f, 100.0f);
    KeyboardMovementController controller;

    glm::vec3 transform{0};
    glm::vec3 rotate{0};

    gfx::ShaderProgram program{};
    program.attachShader("../shaders/vert/simple_shader_with_camera.vert");
    program.attachShader("../shaders/frag/simple_shader_with_camera.frag");
    program.link();

    gfx::Mesh mesh{};
    mesh.loadModelFromPath("../assets/colored_cube.obj");

    glEnable(GL_DEPTH_TEST);  

    while (!window.shouldClose())
    {
        window.startFrame();

        glClearColor(.1, .1, .1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        controller.moveInPlaneXZ(window.getGLFWwindow(), 0.01, transform, rotate);
        camera.setViewYXZ(transform, rotate);

        glm::mat4 proj = camera.getProjection();
        glm::mat4 view = camera.getView();

        program.bind();
        program.Mat4f("proj", glm::value_ptr(proj));
        program.Mat4f("view", glm::value_ptr(view));
        mesh.bind();
        mesh.draw();

        myImGuiStartFrame();

        ImGui::Begin("info");
        ImGui::Text("%.2f fps", ImGui::GetIO().Framerate);
        ImGui::Text("%.2f %.2f %.2f", transform.x, transform.y, transform.z);
        ImGui::Text("%.2f %.2f %.2f", rotate.x, rotate.y, rotate.z);
        ImGui::End();

        myImGuiEndFrame();

        window.endFrame();
    }
    
}