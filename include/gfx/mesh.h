#ifndef MESH_H
#define MESH_H

#include "gfx/vao.h"
#include "gfx/vbo.h"
#include "gfx/ebo.h"

#include "gfx/model.h"

namespace gfx
{

class Mesh
{
public:
    Mesh();
    ~Mesh();
    
    void loadModelFromPath(const char *filePath);
    void bind();
    void unBind();
    void draw();

private:
    unsigned int vertexCount;
    std::vector<Model::Vertex> vertices;
    unsigned int indexCount;
    std::vector<unsigned int> indices;

    Model model;
};

} // namespace gfx


#endif

