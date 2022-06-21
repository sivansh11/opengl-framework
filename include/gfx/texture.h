#ifndef TEXTURE_H
#define TEXTUER_H

#include "core.h"

#include <glad/glad.h>

namespace gfx
{
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

    class Texture2D
    {
    public:
        Texture2D() = default;
        Texture2D(const char *imgPath, std::string type);
        Texture2D(int width, int height, unsigned char* data, GLuint format, std::string type);
        ~Texture2D();
        void load(const char *imgPath, std::string type);
        void load(int width, int height, unsigned char* data, GLuint format, std::string type);
        void remove();
        void bind(int unit);
        void unBind(int unit);

        int width, height;
        std::string type;
        std::string path;
    private:
        GLuint id{};
    };
};

#endif