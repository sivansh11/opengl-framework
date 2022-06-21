#ifndef VAO_H
#define VAO_H

#include "core.h"
#include <glad/glad.h>

namespace gfx
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void linkVertexBuffer(GLuint layout, unsigned int size, GLenum type, GLenum normalize, unsigned int stride, void * offset);

        void free();
        
        void bind();
        void unBind();

        GLuint id{};
    private:

    };
} // namespace gfx

#endif