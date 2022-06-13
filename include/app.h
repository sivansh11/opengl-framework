#ifndef APP_H
#define APP_H

#include "window.h"
#include "event.h"

class App
{
public:
    App();
    ~App();

    void run();

private:
    std::string title = "ogl";
    int width = 800;
    int height = 800;
    Window window;
};

#endif
