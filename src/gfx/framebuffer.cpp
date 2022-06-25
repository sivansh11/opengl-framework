#include "gfx/framebuffer.h"


namespace gfx
{

FrameBuffer::FrameBuffer()
{
    
}    
FrameBuffer::~FrameBuffer()
{

}

void FrameBuffer::init(int width, int height, FrameBufferType type)
{
    ASSERT(id == 0, "framebuffer already initialized");
    FrameBuffer::width = width;
    FrameBuffer::height = height;
    FrameBuffer::type = type;

    glCall(glGenFramebuffers(1, &id));
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, id));

    switch (type)
    {
    case FrameBufferType::COLORDEPTH:
        // color
        GLuint colorTexture;
        glCall(glGenTextures(1, &colorTexture));
        glCall(glBindTexture(GL_TEXTURE_2D, colorTexture));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));  
        glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
        glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0));
        attachments.push_back(Attachment{colorTexture, FrameBufferType::COLOR});
        // depthStencil
        GLuint depthStencil;
        glCall(glGenTextures(1, &depthStencil));
        glCall(glBindTexture(GL_TEXTURE_2D, depthStencil));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));  
        glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
        glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencil, 0));
        attachments.push_back(Attachment{depthStencil, FrameBufferType::DEPTH});
        
        break;
    case FrameBufferType::DEPTH:
        GLuint depthTexture;
        glCall(glGenTextures(1, &depthTexture));
        glCall(glBindTexture(GL_TEXTURE_2D, depthTexture));
        glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0));
        glCall(glDrawBuffer(GL_NONE));
        glCall(glReadBuffer(GL_NONE));
        attachments.push_back(Attachment{depthTexture, FrameBufferType::DEPTH});

    default:
        break;
    }

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
        ASSERT(false, "");
    }
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::bindTextureAttachment(FrameBufferType type, int unit)
{
    for (auto attachment: attachments)
    {
        if (attachment.type != type) continue;
        glCall(glActiveTexture(GL_TEXTURE0 + unit));
        glCall(glBindTexture(GL_TEXTURE_2D, attachment.id));
        return;
    }
    ASSERT(false, "attachment not in framebuffer");
}
GLuint FrameBuffer::getTextureAttachment(FrameBufferType type)
{
    for (auto attachment: attachments)
    {
        if (attachment.type != type) continue;
        return attachment.id;
    }
    ASSERT(false, "attachment not in framebuffer");
}

void FrameBuffer::free()
{
    glCall(glDeleteFramebuffers(1, &id));
}

void FrameBuffer::bind()
{
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, id));
    glCall(glViewport(0, 0, width, height));
}
void FrameBuffer::unBind(int srcWidth, int srcHeight)
{
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    glCall(glViewport(0, 0, srcWidth, srcHeight));
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

} // namespace gfx
