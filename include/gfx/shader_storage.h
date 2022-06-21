#ifndef SHADER_STORAGE_H
#define SHADER_STORAGE_H

#include "core.h"

#include <glad/glad.h>

namespace gfx
{

class ShaderStorage
{
public:
    ShaderStorage() = default;
    ShaderStorage(void* data, size_t dataSize);

    void init(void* data, size_t dataSize);
    void update_gpu(void* data, size_t dataSize);
    void update_cpu(void* data, size_t dataSize);

    void bind(int binding);
    void unBind();

private:
    GLuint id;
};

} // namespace gfx


#endif