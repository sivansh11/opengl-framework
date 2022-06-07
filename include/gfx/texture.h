#ifndef TEXTURE_H
#define TEXTUER_H

#include "core.h"

#include <glad/glad.h>

namespace gfx{
    class TextureStorage2D
    {
    public:
        TextureStorage2D(int width, int height);
        ~TextureStorage2D();
        void init(int width, int height);
        void remove();
        void bind(int uint_);
        void unBind(int uint_);
        // GLuint getHandle();

        int width, height;

    private:
        GLuint id{};
    };
}

#endif