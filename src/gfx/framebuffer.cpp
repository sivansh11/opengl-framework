#include "gfx/framebuffer.h"


namespace gfx
{
Framebuffer::Framebuffer(const FramebufferSpecification& spec) : m_spec(spec)
{

}
Framebuffer::~Framebuffer()
{
    glCall(glDeleteFramebuffers(1, &id));
    for (auto tex: attachments)
    {
        glDeleteTextures(1, &tex);
    }
    attachments.clear();
}

void Framebuffer::invalidate()
{
    glCall(glGenFramebuffers(1, &id));
    bind();

    for (auto type: m_spec.attachments)
    {
        switch (type)
        {
        case Attachments::Color:
            GLuint textureColorBuffer;
            glCall(glGenTextures(1, &textureColorBuffer));
            glCall(glBindTexture(GL_TEXTURE_2D, textureColorBuffer));
            glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_spec.width, m_spec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
            glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            glCall(glBindTexture(GL_TEXTURE_2D, 0));
            glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0));
            attachments.push_back(textureColorBuffer);
            break;
        case Attachments::DepthStencil:
            GLuint textureDepthStencilBuffer;
            glCall(glGenTextures(1, &textureDepthStencilBuffer));
            glCall(glBindTexture(GL_TEXTURE_2D, textureDepthStencilBuffer));
            glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_spec.width, m_spec.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL));
            glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureDepthStencilBuffer, 0));
            attachments.push_back(textureDepthStencilBuffer);
            break;
        default:
            break;
        }
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    unBind();
}

void Framebuffer::create(const FramebufferSpecification& spec)
{
    m_spec = spec;
    invalidate();
}


void Framebuffer::bind()
{
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, id));
}

void Framebuffer::unBind()
{
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

GLuint Framebuffer::getColorTexture()
{
    int i=0;
    for (auto type: m_spec.attachments)
    {
        if (type == Attachments::Color) break;
        i++;
    }
    return attachments[i];
}

FrameBufferQuad::FrameBufferQuad()
{
    vao.bind();
    vbo.bind();
    ebo.bind();
    vbo.load(framebufferVertices, 16 * sizeof(float));
    vao.linkVertexBuffer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(0));
    vao.linkVertexBuffer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
    ebo.load(gfx::framebufferIndices, 6 * sizeof(unsigned int));
    vao.unBind();
    vbo.unBind();
    ebo.unBind();
}
void FrameBufferQuad::bind()
{
    vao.bind();
    vbo.bind();
    ebo.bind();
}
void FrameBufferQuad::unBind()
{
    vao.unBind();
    vbo.unBind();
    ebo.unBind();
}
void FrameBufferQuad::draw()
{
    glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

}
