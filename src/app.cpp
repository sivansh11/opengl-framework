#include "app.h"

#include "imgui_setup.h"
#include "gfx/texture.h"
#include "gfx/shader.h"
#include "gfx/model.h"

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
    static ImVec3 color_for_body = ImVec3(float(0) / 255,float(0) / 255,float(0) / 255);
    static ImVec3 color_for_pops = ImVec3(float(45) / 255,float(45) / 255,float(45) / 255);
    static ImVec3 color_for_tabs = ImVec3(float(0) / 255,float(0) / 255,float(0) / 255);
    imgui_easy_theming(color_for_text, color_for_head, color_for_area, color_for_body, color_for_pops, color_for_tabs);

}

App::~App()
{
    myImGuiBye();
}

enum BoxType
{
    Green,
    Red,
    Blue,
};

struct PayLoad
{
    BoxType boxHit;
};


struct Box
{
    Box(vec3 min, vec3 max, BoxType type) : box{min, max}, type(type) {}
    bvh::BoundingBox box;
    BoxType type;

    static void intersect(Box &box, bvh::Ray<PayLoad> &ray)
    {
        float intersect = box.box.intersect(ray);
        if (intersect < ray.t)
        {
            ray.t = intersect;
            ray.data.boxHit = box.type;
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
        1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }; 

    gfx::ShaderProgram hello_shader;
    hello_shader.attachShader("../shaders/vert/texture_example.vert");
    hello_shader.attachShader("../shaders/frag/texture_example.frag");
    hello_shader.link();

    gfx::ElementBuffer ebo;
    gfx::VertexBuffer vbo;
    gfx::VertexArray vao;
    ebo.load(indices, sizeof(indices));
    vbo.load(vertices, sizeof(vertices));
    vao.linkVertexBuffer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    vao.linkVertexBuffer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.linkVertexBuffer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    uint32_t *data = new uint32_t[width * height];
    for (int i=0; i<width; i++) for (int j=0; j<height; j++)
    {
        data[j * width + i] = 0;
    }

    Camera camera;
    camera.set(vec3{0, 0, 0}, vec3{0, 0, 5}, vec3{0, 1, 0}, float(width) / height, 90.0);
    glm::vec3 transform{}, rotate{};
    KeyboardMovementController controller;    

    // std::vector<Box> boxes{
    //     Box{vec3{-1, -1, 7}, vec3{1, 1, 9}, BoxType::Red},
    //     Box{vec3{-1, -1, 5}, vec3{1, 1, 7}, BoxType::Green},
    //     Box{vec3{-3, 1, 5}, vec3{-1, -1, 7}, BoxType::Blue},
    // };
    std::vector<Box> boxes;

    Random random_;
    random_.init(5);
    for (int i=0; i<100; i++)
    {
        int x = random_.randFloat(-10, 10);
        int y = random_.randFloat(-10, 10);
        int z = random_.randFloat(7, 15);
        switch (int(random_.randFloat(1, 4)))
        {
        case 1:
            boxes.push_back(Box{{x * 2, y * 2, z * 2}, {x * 2 + 2, y * 2 + 2, z * 2 + 2}, BoxType::Green});
            break;
        case 2:
            boxes.push_back(Box{{x * 2, y * 2, z * 2}, {x * 2 + 2, y * 2 + 2, z * 2 + 2}, BoxType::Red});
            break;
        case 3:
            boxes.push_back(Box{{x * 2, y * 2, z * 2}, {x * 2 + 2, y * 2 + 2, z * 2 + 2}, BoxType::Blue});
            break;
        
        default:
            break;
        }
    }

    bvh::BVH<Box> bvh{};

    float numThreads = 12;


    while (!window.shouldClose())
    {
        window.startFrame();

        // controller.moveInPlaneXZ(window.getGLFWwindow(), (1000.f / ImGui::GetIO().Framerate) * 0.1, transform, rotate);
        // camera.set(transform, glm::vec3{glm::cos(rotate.x) * glm::cos(rotate.y), glm::sin(rotate.x) * glm::sin(rotate.y), glm::sin(rotate.y)}, glm::vec3{0, 1, 0}, 1, 90.0f);

        bvh.BVH_BIN_builder(boxes);

        glCall(glClearColor(0.1, 0.1, 0.1, 1));
        glCall(glClear(GL_COLOR_BUFFER_BIT));

        std::vector<std::thread> threads;

        for (int n=0; n<numThreads; n++)
        {
            int width = App::width;
            int height = App::height;
            auto task = [&data, &numThreads, &camera, &bvh, width, height, n]()
            {
                int columnsPerThread = width / numThreads;
                for (int i = n * columnsPerThread; i <= (n + 1) * columnsPerThread && i<width; i++) for (int j=0; j<height; j++)
                {
                    float U = float(i) / (width - 1);
                    float V = float(j) / (height - 1);

                    bvh::Ray<PayLoad> r;
                    r = camera.getRay<PayLoad>(U, V);
                    bvh.intersect(r);

                    if (r.t != FLT_MAX)
                    {
                        switch (r.data.boxHit)
                        {
                        case BoxType::Green:
                            data[j * width + i] = 0xff00ff00;
                            break;
                        case BoxType::Red:
                            data[j * width + i] = 0xff0000ff;
                            break;
                        case BoxType::Blue:
                            data[j * width + i] = 0xffff0000;
                            break;
                        default:
                            data[j * width + i] = 0x00000000;
                            break;
                        }
                    }
                }
            };
            threads.emplace_back(task);
        }
        for (int i=0; i<threads.size(); i++)
        {
            threads[i].join();
        }
        
        gfx::Texture2D tex(width, height, data, GL_RGBA);

        hello_shader.bind();
        tex.bind(0);
        ebo.bind();
        glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        myImGuiStartFrame();

        ImGui::Begin("info");
        ImGui::Text("%.2f fps", ImGui::GetIO().Framerate);
        ImGui::DragFloat("num threads", &numThreads);
        ImGui::End();

        myImGuiEndFrame();

        window.endFrame();
    }
    
}