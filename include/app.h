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
    int width = 600;
    int height = 400;
    Window window;
};

#endif
