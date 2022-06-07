#ifndef SHADER_H
#define SHADER_H

#include "core.h"

#include <glad/glad.h>

namespace gfx
{
    class ShaderProgram
    {
        class Shader
        {
        public:
            Shader(const char *shaderPath);
            void compileShader();
            void printShaderInfoLog();
            void free();
            GLuint id;
        private:
            const char *shaderPath;
        };

    public:
        ShaderProgram();
        ~ShaderProgram();
        void attachShader(const char *shaderPath);
        void link();
        void free();
        void reload();

        int uniformLoc(const char* uniformName);
        void bind();
        void dispatchCompute(uint x, uint y, uint z);
        void Mat4f(const char* uniformName, const float *valuePointer, GLboolean transpose=false); 
        void Mat3f(const char* uniformName, const float *valuePointer, GLboolean transpose=false); 
        void Mat2f(const char* uniformName, const float *valuePointer, GLboolean transpose=false); 
        void vec4f(const char* uniformName, const float *valuePointer);
        void vec3f(const char* uniformName, const float *valuePointer);
        void vec2f(const char* uniformName, const float *valuePointer);
        void vecf(const char* uniformName, float value);
        void veci(const char* uniformName, int value);

    private:
        void printProgramInfoLog();
        std::vector<Shader> shaders;
        GLuint id;
    };
} // namespace gfx

    

#endif