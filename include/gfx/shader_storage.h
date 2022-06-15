#ifndef SHADER_STORAGE_H
#define SHADER_STORAGE_H

#include "core.h"

#include "gfx/shader.h"

#include <glad/glad.h>

namespace gfx
{

class ShaderStorage
{
public:
    ShaderStorage();

    void init(void* data, size_t dataSize);
    void update(void* data, size_t dataSize);

    void bind(gfx::ShaderProgram &program, int binding, const char* name);
    void unBind();

private:
    GLuint id;
};

} // namespace gfx


#endif