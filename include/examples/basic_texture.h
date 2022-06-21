#ifndef BASIC_TEXTURE_H
#define BASIC_TEXTURE_H

#include "gfx/vao.h"
#include "gfx/vbo.h"
#include "gfx/ebo.h"

#include "gfx/shader.h"

#include "gfx/texture.h"

class BasicTexture
{
public:
    BasicTexture()
    {
        float vertices[] = {
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
        };
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };
        vbo.load(vertices, 8 * 4 * sizeof(float));
        ebo.load(indices, 6 * sizeof(unsigned int));
        vao.linkVertexBuffer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(0));
        vao.linkVertexBuffer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
        vao.linkVertexBuffer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

        shader.attachShader("../shaders/vert/basic_texture.vert");
        shader.attachShader("../shaders/frag/basic_texture.frag");
        shader.link();

        texture.load("../textures/tex1.png");
    }
    void render()
    {
        shader.bind();
        texture.bind(0);
        shader.veci("texture1", 0);
        vao.bind();
        ebo.bind();
        vbo.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
private:
    // buffers
    gfx::VertexArray vao{};
    gfx::VertexBuffer vbo{};
    gfx::ElementBuffer ebo{};

    gfx::ShaderProgram shader{};

    gfx::Texture2D texture{};
};

#endif