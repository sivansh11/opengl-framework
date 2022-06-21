#include "app.h"

#include "editor.h"

App::App() : window(width, height, title.c_str())
{
    myImGuiSetup(window);
    // static ImVec3 color_for_text = ImVec3(float(255) / 255,float(255) / 255,float(255) / 255);
    // static ImVec3 color_for_head = ImVec3(float(80) / 255,float(80) / 255,float(80) / 255);
    // static ImVec3 color_for_area = ImVec3(float(50) / 255,float(50) / 255,float(50) / 255);
    // static ImVec3 color_for_body = ImVec3(float(100) / 255,float(100) / 255,float(100) / 255);
    // static ImVec3 color_for_pops = ImVec3(float(45) / 255,float(45) / 255,float(45) / 255);
    // static ImVec3 color_for_tabs = ImVec3(float(0) / 255,float(0) / 255,float(0) / 255);
    // imgui_easy_theming(color_for_text, color_for_head, color_for_area, color_for_body, color_for_pops, color_for_tabs);

}

App::~App()
{
    myImGuiBye();
}

void App::run()
{
    Editor editor{window};

    while (!window.shouldClose())
    {
        editor.update(0.01);  // TODO: actually getting proper timings

        window.startFrame();

        editor.render();

        window.endFrame();
    }
    
}