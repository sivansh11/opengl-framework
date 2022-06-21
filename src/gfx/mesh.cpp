#include "gfx/mesh.h"


namespace gfx
{

Mesh::Mesh()
{

}
Mesh::~Mesh()
{
    
}

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Texture2D> &textures)
{
    load(vertices, indices, textures);
}

void Mesh::load(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Texture2D> &textures)
{
    Mesh::textures = textures;
    Mesh::vertices = vertices;
    Mesh::indices = indices;
}

void Mesh::free()
{
    for (auto &texture: textures)
    {
        texture.free();
    }
    vao.free();
    vbo.free();
    ebo.free();
}

void Mesh::setupMesh()
{
    vao.bind();
    vbo.bind();
    vbo.load((float*)(vertices.data()), sizeof(vertices[0]) * vertices.size());

    ebo.bind();
    ebo.load((unsigned int*)(indices.data()), sizeof(indices[0]) * indices.size());

    vao.linkVertexBuffer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
    vao.linkVertexBuffer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
    vao.linkVertexBuffer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));

    vao.unBind();
}

void Mesh::draw(ShaderProgram &shader)
{
    unsigned int diffN = 1;
    unsigned int specN = 1;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        std::string number;
        std::string name = textures[i].type;
        if (name == "diffuse")
        {
            number = std::to_string(diffN++);
        }
        else if (name == "specular")
        {
            number = std::to_string(specN++);
        }
        textures[i].bind(i);
        shader.veci((name + number).c_str(), i);
    }

    vao.bind();
    glCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
    vao.unBind();
}

} // namespace gfx
