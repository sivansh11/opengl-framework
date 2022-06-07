#include "gfx/shader.h"

#include "core.h"
#include "utils/utils.h"

namespace gfx
{
    ShaderProgram::Shader::Shader(const char *shaderPath) : shaderPath(shaderPath)
    {
        compileShader();
    }
    void ShaderProgram::Shader::compileShader()
    {
        std::string shaderFile = read_file(shaderPath);
        
        std::string shaderPathString = shaderPath;

        GLenum type;

        if (shaderPathString.find(".vert") != std::string::npos) type = GL_VERTEX_SHADER;
        else if (shaderPathString.find(".frag") != std::string::npos) type = GL_FRAGMENT_SHADER;
        else if (shaderPathString.find(".geom") != std::string::npos) type = GL_GEOMETRY_SHADER;
        else if (shaderPathString.find(".comp") != std::string::npos) type = GL_COMPUTE_SHADER;

        id = glCall(glCreateShader(type));

        char const* shaderCode = shaderFile.c_str();

        glCall(glShaderSource(id, 1, &shaderCode, NULL));

        glCall(glCompileShader(id));

        int success;
        glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &success));

        if (!success) 
        {
            std::cout << "File name:" << shaderPath << '\n';
            printShaderInfoLog();
            std::__throw_runtime_error("Shader not compiled!");
        }
    }
    void ShaderProgram::Shader::free()
    {
        glCall(glDeleteShader(id));
    }
    void ShaderProgram::Shader::printShaderInfoLog()
    {
        GLint logLen;
        glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen));
        std::vector<char> log(logLen);
        glCall(glGetShaderInfoLog(id, logLen, &logLen, log.data()));
        for (auto ch: log) std::cout << ch;
        std::cout << '\n';
    }

    ShaderProgram::ShaderProgram()
    {
        id = glCall(glCreateProgram());
    }
    ShaderProgram::~ShaderProgram()
    {
        free();
    }
    void ShaderProgram::free()
    {
        glCall(glDeleteProgram(id));
    }
    void ShaderProgram::bind()
    {
        glCall(glUseProgram(id));
    }
    void ShaderProgram::dispatchCompute(uint x, uint y, uint z)
    {
        glCall(glDispatchCompute(x, y, z));
        glCall(glMemoryBarrier(GL_ALL_BARRIER_BITS));
    }
    void ShaderProgram::attachShader(const char *shaderPath)
    {
        Shader shader(shaderPath);
        shaders.push_back(shader);
        glCall(glAttachShader(id, shader.id));
    }
    void ShaderProgram::link()
    {
        glCall(glLinkProgram(id));
        int success;
        glCall(glGetProgramiv(id, GL_LINK_STATUS, &success));
        if (!success)
        {
            printProgramInfoLog();
            std::__throw_runtime_error("Shader Program didnt link");
        }
        for (auto& shader: shaders)
        {
            shader.free();
        }
    }
    void ShaderProgram::reload()
    {
        free();
        id = glCall(glCreateProgram());

        for (auto& shader: shaders)
        {
            shader.compileShader();
            glCall(glAttachShader(id, shader.id));
        }
        link();
    }
    void ShaderProgram::printProgramInfoLog()
    {
        GLint logLen;
        glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen));
        std::vector<char> log(logLen);
        glCall(glGetShaderInfoLog(id, logLen, &logLen, log.data()));
        for (auto ch: log) std::cout << ch;
        std::cout << '\n';
    }
    int ShaderProgram::uniformLoc(const char* uniformName)
    {
        int val = glCall(glGetUniformLocation(id, uniformName));
        ASSERT(val != -1, (std::string("uniform not there in shader") + "uniform name is " + uniformName));
        return val;
    }
    void ShaderProgram::Mat4f(const char* uniformName, const float *valuePointer, GLboolean transpose)
    {
        glCall(glUniformMatrix4fv(uniformLoc(uniformName), 1, transpose, valuePointer));
    }
    void ShaderProgram::Mat3f(const char* uniformName, const float *valuePointer, GLboolean transpose)
    {
        glCall(glUniformMatrix3fv(uniformLoc(uniformName), 1, transpose, valuePointer));
    }
    void ShaderProgram::Mat2f(const char* uniformName, const float *valuePointer, GLboolean transpose)
    {
        glCall(glUniformMatrix2fv(uniformLoc(uniformName), 1, transpose, valuePointer));
    }
    void ShaderProgram::vec4f(const char* uniformName, const float *valuePointer)
    {
        glCall(glUniform4fv(uniformLoc(uniformName), 1, valuePointer));
    }
    void ShaderProgram::vec3f(const char* uniformName, const float *valuePointer)
    {
        glCall(glUniform3fv(uniformLoc(uniformName), 1, valuePointer));
    }
    void ShaderProgram::vec2f(const char* uniformName, const float *valuePointer)
    {
        glCall(glUniform2fv(uniformLoc(uniformName), 1, valuePointer));
    }
    void ShaderProgram::veci(const char* uniformName, int value)
    {
        glCall(glUniform1i(uniformLoc(uniformName), value));
    }
    void ShaderProgram::vecf(const char* uniformName, float value)
    {
        glCall(glUniform1f(uniformLoc(uniformName), value));
    }
} // namespace gfx
