#ifndef EBO_H
#define EBO_H

#include "core.h"

#include <glad/glad.h>

namespace gfx
{
    class ElementBuffer
    {
    public:
        ElementBuffer() = default;
        ElementBuffer(GLuint *indices, size_t size);
        ~ElementBuffer();

        void load(GLuint *indices, size_t size);
        void bind();
        void unBind();

        GLuint id{};
    private:

    };
} // namespace gfx


#endif