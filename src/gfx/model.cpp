#include "gfx/model.h"


namespace gfx
{

Model::Model()
{

}
Model::~Model()
{

}

void Model::load(std::vector<Vertex> &vertices, std::vector<uint32_t> &indices)
{
    vao.bind();
    vbo.load((float*)(vertices.data()), sizeof(vertices[0]) * vertices.size());
    verticesSize = vertices.size();

    if (indices.size() != 0)
    {   
        ebo.load((uint32_t*)(indices.data()), sizeof(indices[0]) * indices.size());
        indicesSize = indices.size();
    }

    vao.linkVertexBuffer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
    vao.linkVertexBuffer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
    vao.linkVertexBuffer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
    vao.linkVertexBuffer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));
}
void Model::draw()
{
    bind();
    if (indicesSize == 0)
    {
        glCall(glDrawArrays(GL_TRIANGLES, 0, verticesSize));
    }
    else
    {
        glCall(glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0));
    }
}

void Model::unBind()
{
    vbo.unBind();
    ebo.unBind();
    vao.unBind();
}

void Model::bind()
{
    vao.bind();
    ebo.bind();
}

} // namespace gfx
