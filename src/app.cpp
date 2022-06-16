#include "app.h"

#include "imgui_setup.h"
#include "gfx/texture.h"
#include "gfx/shader.h"
#include "gfx/model.h"
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
    static ImVec3 color_for_body = ImVec3(float(0) / 255,float(0) / 255,float(0) / 255);
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

    gfx::ShaderProgram hello_shader;
    hello_shader.attachShader("../shaders/vert/screen_shader.vert");
    hello_shader.attachShader("../shaders/frag/screen_shader.frag");
    hello_shader.link();

    gfx::ElementBuffer ebo;
    gfx::VertexBuffer vbo;
    gfx::VertexArray vao;
    ebo.load(indices, sizeof(indices));
    vbo.load(vertices, sizeof(vertices));
    vao.linkVertexBuffer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    vao.linkVertexBuffer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.linkVertexBuffer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));    

    std::vector<Box> boxes{
        Box{vec3{-1, -1, 5}, vec3{1, 1, 7}}
    };

    bvh::BVH<Box> bvh;
    bvh.BVH_BIN_builder(boxes);

    gfx::TextureStorage2D screen(500, 500);
    
    gfx::ShaderProgram raytracer_with_bvh{};
    raytracer_with_bvh.attachShader("../shaders/comp/raytracer_with_bvh.comp");
    raytracer_with_bvh.link();

    gfx::ShaderStorage primitives;
    primitives.init(bvh.getPrimitives().data(), bvh.getPrimitives().size() * sizeof(Box));

    gfx::ShaderStorage primitiveID;
    primitiveID.init(bvh.getPrimitiveIDs().data(), bvh.getPrimitiveIDs().size() * sizeof(uint));
    
    gfx::ShaderStorage bvhNodes;
    bvhNodes.init(bvh.getBvhNodes().data(), bvh.getBvhNodes().size() * sizeof(bvh::BvhNode));
    
    struct MetaData
    {
        uint rootNodeID;
        uint nodesUsed;
    } data{bvh.getRootNodeID(), bvh.getNodesUsed()};

    gfx::ShaderStorage metaData;
    metaData.init(&data, sizeof(MetaData));

    while (!window.shouldClose())
    {
        window.startFrame();

        glClearColor(.1, .1, .1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // test.bind();
        // screen.bind(0);
        // test.veci("screen", 0);
        // shaderStorage.bind(0);
        // test.dispatchCompute(glm::ceil(float(width) / 8), glm::ceil(float(height) / 4), 1);

        raytracer_with_bvh.bind();
        screen.bind(0);
        raytracer_with_bvh.veci("screen", 0);
        primitives.bind(0);
        primitiveID.bind(1);
        bvhNodes.bind(2);
        metaData.bind(3);
        raytracer_with_bvh.dispatchCompute(glm::ceil(float(500) / 8), glm::ceil(float(500) / 4), 1);

        hello_shader.bind();
        vao.bind();
        ebo.bind();
        screen.bind(0);
        hello_shader.veci("screen", 0);
        glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        myImGuiStartFrame();

        ImGui::Begin("info");
        ImGui::Text("%.2f fps", ImGui::GetIO().Framerate);
        ImGui::End();

        myImGuiEndFrame();

        window.endFrame();
    }
    
}