#ifndef MODEL_H
#define MODEL_H

#include "gfx/vao.h"
#include "gfx/vbo.h"
#include "gfx/ebo.h"

#include "gfx/mesh.h"

namespace gfx
{

class Model
{
public:
    Model();
    ~Model();
    
    void loadModelFromPath(const char *filePath);
    void bind();
    void unBind();
    void draw();

private:
    unsigned int vertexCount;
    std::vector<Mesh::Vertex> vertices;
    unsigned int indexCount;
    std::vector<unsigned int> indices;

    Mesh mesh;
};

} // namespace gfx


#endif

