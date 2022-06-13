#include "app.h"

#include "imgui_setup.h"
#include "gfx/shader.h"
#include "gfx/model.h"

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


void App::run()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    }; 

    gfx::ShaderProgram hello_shader;
    hello_shader.attachShader("../shaders/vert/hello_triangle.vert");
    hello_shader.attachShader("../shaders/frag/hello_triangle.frag");
    hello_shader.link();

    gfx::VertexBuffer vbo;
    gfx::VertexArray vao;
    vbo.load(vertices, sizeof(vertices));
    vao.linkVertexBuffer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    while (!window.shouldClose())
    {
        window.startFrame();

        glCall(glClearColor(0.1, 0.1, 0.1, 1));
        glCall(glClear(GL_COLOR_BUFFER_BIT));

        hello_shader.bind();
        vao.bind();
        glCall(glDrawArrays(GL_TRIANGLES, 0, 3));

        myImGuiStartFrame();

        ImGui::Begin("test");
        ImGui::Text("testing");
        ImGui::End();

        myImGuiEndFrame();

        window.endFrame();
    }
    
}