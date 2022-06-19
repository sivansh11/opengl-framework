#ifndef SIMPLE_RENDERER_H
#define SIMPLE_RENDERER_H

#include "utils/camera.h"
#include "utils/components.h"
#include "utils/ecs.h"
#include "utils/filewatcher.h"

#include "gfx/shader.h"
#include "gfx/framebuffer.h"

class SimpleRenderer
{
public:
    SimpleRenderer()
    {
        shaderProgram.attachShader("../shaders/vert/simple_renderer.vert");
        shaderProgram.attachShader("../shaders/frag/simple_renderer.frag");
        shaderProgram.link();
        fileWatchers.push_back(FileWatcher{"../shaders/vert/simple_renderer.vert"});
        fileWatchers.push_back(FileWatcher{"../shaders/frag/simple_renderer.frag"});
    }

    void render(ecs::Scene &scene, ecs::EntityID &camera_)
    {
        bool reloadShader{false};
        for (auto& fileWatcher: fileWatchers)
        {
            if (fileWatcher.hasChanged())
            {
                reloadShader = true;
            }
        }
        if (reloadShader)
        {
            std::cout << "reloaded shader!\n";
            shaderProgram.reload();
        }
        Camera &camera = scene.get<Camera>(camera_);
        static glm::mat4 proj{};
        proj = camera.getProjection(); 
        static glm::mat4 view{};
        view = camera.getView(); 
        static glm::mat4 model{1};
        shaderProgram.bind();

        for (auto obj: ecs::SceneView<gfx::Model, Transform>(scene))
        {
            gfx::Model& model_ = scene.get<gfx::Model>(obj);
            Transform &transform = scene.get<Transform>(obj);
            model = transform.mat4();
            shaderProgram.Mat4f("proj", glm::value_ptr(proj));
            shaderProgram.Mat4f("view", glm::value_ptr(view));
            shaderProgram.Mat4f("model", glm::value_ptr(model));
            model_.bind();
            model_.draw();
        }
    }

private:
    gfx::ShaderProgram shaderProgram{};
    std::vector<FileWatcher> fileWatchers;
};

class FrameBufferUsageRenderer
{
public:
    FrameBufferUsageRenderer(int width, int height)
    {
        gfx::FramebufferSpecification fbSpec;
        fbSpec.width = width;
        fbSpec.height = height;
        fbSpec.attachments.push_back(gfx::Attachments::Color);
        frameBuffer.create(fbSpec);

        shaderProgram.attachShader("../shaders/vert/simple_renderer.vert");
        shaderProgram.attachShader("../shaders/frag/simple_renderer.frag");
        shaderProgram.link();

        frameBufferProgram.attachShader("../shaders/vert/frame_buffer.vert");
        frameBufferProgram.attachShader("../shaders/frag/frame_buffer.frag");
        frameBufferProgram.link();
    }
    void render(ecs::Scene &scene, ecs::EntityID &camera_)
    {
        Camera &camera = scene.get<Camera>(camera_);
        static glm::mat4 proj{};
        proj = camera.getProjection(); 
        static glm::mat4 view{};
        view = camera.getView(); 
        static glm::mat4 model{1};
        
        frameBuffer.bind();

        glClearColor(.1, .1, .1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);  

        shaderProgram.bind();

        for (auto obj: ecs::SceneView<gfx::Model, Transform>(scene))
        {
            gfx::Model& model_ = scene.get<gfx::Model>(obj);
            Transform &transform = scene.get<Transform>(obj);
            model = transform.mat4();
            shaderProgram.Mat4f("proj", glm::value_ptr(proj));
            shaderProgram.Mat4f("view", glm::value_ptr(view));
            shaderProgram.Mat4f("model", glm::value_ptr(model));
            model_.bind();
            model_.draw();
        }
        frameBufferProgram.bind();
        frameBufferProgram.veci("screenTexture", 0);

        frameBuffer.unBind();

        frameBufferQuad.bind();
        glClearColor(.1, .1, .1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        frameBuffer.bindTexture();
        frameBufferQuad.draw();
    }

private:
    gfx::Framebuffer frameBuffer;
    gfx::ShaderProgram shaderProgram;
    gfx::ShaderProgram frameBufferProgram;
    gfx::FrameBufferQuad frameBufferQuad;

};



#endif