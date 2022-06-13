#include "gfx/texture.h"

#include "utils/utils.h"


namespace gfx
{
    TextureStorage2D::TextureStorage2D(int width, int height)
    {
        init(width, height);        
    }
    TextureStorage2D::~TextureStorage2D()
    {
        glCall(glDeleteTextures(1, &id));
    }

    void TextureStorage2D::init(int width, int height)
    {
        TextureStorage2D::width = width;
        TextureStorage2D::height = height;

        glCall(glCreateTextures(GL_TEXTURE_2D, 1, &id));

        glCall(glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        glCall(glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCall(glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        glCall(glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        
        glCall(glTextureStorage2D(id, 1, GL_RGBA32F, width, height));
        
        glCall(glBindImageTexture(0, id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F));
    }

    void TextureStorage2D::remove()
    {
        glCall(glDeleteTextures(1, &id));

    }   
    void TextureStorage2D::bind(int uint_)
    {
        glCall(glBindTextureUnit(uint_, id));
    }
    void TextureStorage2D::unBind(int uint_)
    {
        glCall(glBindTextureUnit(uint_, 0));
    }

    Texture2D::Texture2D(const char *imgPath)
    {

    }
    Texture2D::~Texture2D()
    {

    }
    void Texture2D::init(int width, int height, void* data)
    {
        
    }

} // namespace gfx


