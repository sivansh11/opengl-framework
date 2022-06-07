#ifndef VBO_H
#define VBO_H

#include "core.h"

#include <glad/glad.h>

namespace gfx
{
    class VertexBuffer
    {
    public:
        VertexBuffer() = default;
        VertexBuffer(float* verticies, size_t size);
        ~VertexBuffer();

        void load(float* verticies, size_t size);
        void bind();
        void unBind();

        GLuint id{};
    private:

    };
} // namespace gfx


#endif