#ifndef FRAME_BUFFER_H 
#define FRAME_BUFFER_H

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

enum FrameBufferType
{
    COLOR,
    DEPTH,
    COLORDEPTH
};

class FrameBuffer
{
    struct Attachment
    {
        GLuint id;
        FrameBufferType type;
    };
public:
    FrameBuffer();
    ~FrameBuffer();

    void init(int width, int height, FrameBufferType type = FrameBufferType::COLORDEPTH);
    void free();
    void bind();
    void unBind(int srcWidth, int srcHeight);

    void bindTextureAttachment(FrameBufferType type, int unit);
    GLuint getTextureAttachment(FrameBufferType type);

private:
    GLuint id{};
    int width, height;
    FrameBufferType type;
    std::vector<Attachment> attachments;
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

} // namespace gfx


#endif

