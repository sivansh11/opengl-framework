#include "gfx/shader_storage.h"


namespace gfx
{

ShaderStorage::ShaderStorage()
{
    glCall(glGenBuffers(1, &id));
}

void ShaderStorage::init(void* data, size_t dataSize)
{
    glCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, id));
    glCall(glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, data, GL_DYNAMIC_COPY));
    unBind();
}

void ShaderStorage::update(void* data, size_t dataSize)
{
    glCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, id));
    void* p = glCall(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY));
    memcpy(p, data, dataSize);
    glCall(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
    unBind();
}

void ShaderStorage::bind(gfx::ShaderProgram &program, int binding, const char* name)
{
    glCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, id));
    GLuint blockIndex = glCall(glGetProgramResourceIndex(program.getHandle(), GL_SHADER_STORAGE_BLOCK, name));
    glCall(glShaderStorageBlockBinding(program.getHandle(), blockIndex, binding));
}
void ShaderStorage::unBind()
{
    glCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}


} // namespace gfx
