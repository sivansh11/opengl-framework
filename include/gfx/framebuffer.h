#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "core.h"
#include <glad/glad.h>

namespace gfx
{
    static GLfloat framebufferVertices[] = 
    {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
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

        void bind();
        void unBind();

        void clearColor(float r, float g, float b, float a);

        GLuint getColorTexture();
        
    private:
        FramebufferSpecification m_spec;
        std::vector<GLuint> attachments;
        GLuint id;
    };
}

#endif