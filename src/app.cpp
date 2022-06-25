#include "app.h"

#include "newshadermodule.h"

App::App() : window(width, height, title.c_str())
{

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