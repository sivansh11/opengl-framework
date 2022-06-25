#include "gfx/vao.h"

namespace gfx
{
    VertexArray::VertexArray()
    {
        glCall(glGenVertexArrays(1, &id));
    }
    VertexArray::~VertexArray()
    {
        free();
    }
    void VertexArray::free()
    {
        glCall(glDeleteVertexArrays(1, &id));
    }
    void VertexArray::bind()
    {
        glCall(glBindVertexArray(id));
    }
    void VertexArray::unBind()
    {
        glCall(glBindVertexArray(0));
    }
    void VertexArray::linkVertexBuffer(GLuint layout, unsigned int size, GLenum type, GLenum normalize, unsigned int stride, void * offset)
    {
        bind();
        glCall(glVertexAttribPointer(layout, size, type, normalize, stride, offset));
        glCall(glEnableVertexAttribArray(layout));
    }
} // namespace gfx

