#include "gfx/vao.h"

namespace gfx
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &id);
    }
    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &id);
    }
    void VertexArray::bind()
    {
        glBindVertexArray(id);
    }
    void VertexArray::unBind()
    {
        glBindVertexArray(0);
    }
    void VertexArray::linkVertexBuffer(GLuint layout, unsigned int size, GLenum type, GLenum normalize, unsigned int stride, void * offset)
    {
        glVertexAttribPointer(layout, size, type, normalize, stride, offset);
        glEnableVertexAttribArray(layout);
    }
} // namespace gfx

