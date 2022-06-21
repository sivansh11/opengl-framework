#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "core.h"

#include "gfx/vbo.h"
#include "gfx/ebo.h"
#include "gfx/vao.h"

#include <glad/glad.h>

namespace gfx
{
static GLfloat framebufferVertices[] = 
{
    -1.0f, -1.0f, 0.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
        1.0f,  1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
};

static GLuint framebufferIndices[] = 
{
    0, 2, 1,
    0, 3, 2
};

enum Attachments
{
    Color,
    DepthStencil
};

struct FramebufferSpecification
{
    uint32_t width, height;
    bool swapChainTarget = false;
    std::vector<Attachments> attachments;
};


class Framebuffer
{
public:
    Framebuffer() {}
    Framebuffer(const FramebufferSpecification& spec);
    ~Framebuffer();
    void create(const FramebufferSpecification& spec);

    void invalidate();
    void invalidate(int width, int height);

    void bind();
    void unBind();
    void bindTexture() { glCall(glBindTexture(GL_TEXTURE_2D, getColorTexture())); }

    float width() { return m_spec.width; }
    float height() { return m_spec.height; }

    GLuint getColorTexture();
    
private:
    FramebufferSpecification m_spec;
    std::vector<GLuint> attachments;
    GLuint id;
};

class FrameBufferQuad
{
public:
    FrameBufferQuad();

    void bind();
    void unBind();
    void draw();

private:
    VertexArray vao;
    VertexBuffer vbo;
    ElementBuffer ebo;
};

}

#endif