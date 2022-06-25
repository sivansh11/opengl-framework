#include "gfx/vbo.h"

namespace gfx
{
    VertexBuffer::VertexBuffer(float* verticies, size_t size)
    {
        glCall(glGenBuffers(1, &id));
        glCall(glBindBuffer(GL_ARRAY_BUFFER, id));
        glCall(glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW));
    }
    VertexBuffer::~VertexBuffer()
    {
        free();
    }
    void VertexBuffer::free()
    {
        glCall(glDeleteBuffers(1, &id));
    }
    /*
        size is in bytes
    */
    void VertexBuffer::load(float* verticies, size_t size)
    {
        ASSERT(id == 0, "buffer already in use");
        glCall(glGenBuffers(1, &id));
        glCall(glBindBuffer(GL_ARRAY_BUFFER, id));
        glCall(glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW));
    }
    void VertexBuffer::bind()
    {
        glCall(glBindBuffer(GL_ARRAY_BUFFER, id));
    }
    void VertexBuffer::unBind()
    {
        glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }


} // namespace gfx

