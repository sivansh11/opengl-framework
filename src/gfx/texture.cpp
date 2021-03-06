#include "gfx/texture.h"

#include "utils/utils.h"

#include "stb_image.h"

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
    
    Texture2D::Texture2D(int width, int height, unsigned char* data, GLuint format, std::string type)
    {
        load(width, height, data, format, type);
    }
    Texture2D::Texture2D(const char *imgPath, std::string type)
    {
        load(imgPath, type);
    }
    Texture2D::~Texture2D()
    {
        free();
    }
    void Texture2D::free()
    {
        glCall(glDeleteTextures(1, &id));
    }
    void Texture2D::load(int width, int height, unsigned char* data, GLuint format, std::string type)
    {
        Texture2D::width = width;
        Texture2D::height = height;
        Texture2D::type = type;

        glCall(glGenTextures(1, &id));
        glCall(glBindTexture(GL_TEXTURE_2D, id));

        glCall(glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        glCall(glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        glCall(glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT));
        glCall(glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT));

        glCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
        glCall(glGenerateMipmap(GL_TEXTURE_2D));
    }
    void Texture2D::load(const char *imgPath, std::string type)
    {
        path = imgPath;
        int width, height, nChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(imgPath, &width, &height, &nChannels, 0);
        ASSERT(data, std::string("file: ") + imgPath + " Error:" + stbi_failure_reason());
        GLint format;
        switch (nChannels)
        {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            RUNTIME_ASSERT(false, "Automatic format detection failed");
            break;
        }   
        load(width, height, data, format, type);
        stbi_image_free(data);
    }
    void Texture2D::bind(int unit)
    {
        glCall(glActiveTexture(GL_TEXTURE0 + unit));
        glCall(glBindTexture(GL_TEXTURE_2D, id));
    }
    void Texture2D::unBind(int unit)
    {        
        glCall(glBindTexture(GL_TEXTURE_2D, 0));
    }  

} // namespace gfx


