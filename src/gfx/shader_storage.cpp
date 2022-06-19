#include "gfx/shader_storage.h"


namespace gfx
{

ShaderStorage::ShaderStorage(void* data, size_t dataSize)
{
    glCall(glGenBuffers(1, &id));
    init(data, dataSize);
}

void ShaderStorage::init(void* data, size_t dataSize)
{
    glCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, id));
    glCall(glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, data, GL_DYNAMIC_DRAW));
    unBind();
}

void ShaderStorage::update_gpu(void* data, size_t dataSize)
{
    glCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, id));
    void* p = glCall(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY));
    memcpy(p, data, dataSize);
    glCall(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
    unBind();
}

void ShaderStorage::update_cpu(void* data, size_t dataSize)
{
    glCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, id));
    void* p = glCall(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY));
    memcpy(data, p, dataSize);
    glCall(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
    unBind();
}

void ShaderStorage::bind(int binding)
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, id);
}
void ShaderStorage::unBind()
{
    glCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}


} // namespace gfx
