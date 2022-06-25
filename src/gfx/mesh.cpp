#include "gfx/mesh.h"


namespace gfx
{

Mesh::Mesh()
{

}
Mesh::~Mesh()
{

}

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, Material &material)
{
    load(vertices, indices, material);
}

void Mesh::load(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, Material &material)
{
    Mesh::material = material;
    Mesh::vertices = vertices;
    Mesh::indices = indices;
    setupMesh();
}

void Mesh::free()
{
    glCall(glDeleteBuffers(1, &VBO));
    glCall(glDeleteBuffers(1, &EBO));
    glCall(glDeleteVertexArrays(1, &VAO));
}

void Mesh::setupMesh()
{
    glCall(glGenVertexArrays(1, &VAO));
    glCall(glGenBuffers(1, &VBO));
    glCall(glGenBuffers(1, &EBO));

    glCall(glBindVertexArray(VAO));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));

    glCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));  

    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

    // vertex positions
    glCall(glEnableVertexAttribArray(0));	
    glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)));
    // vertex normals
    glCall(glEnableVertexAttribArray(1));	
    glCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));
    // vertex texture coords
    glCall(glEnableVertexAttribArray(2));	
    glCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv)));

    glCall(glBindVertexArray(0));
}

void Mesh::draw(ShaderProgram &shader, Material &parentMaterial, Transform &parentTransform)
{
    if (hasMaterial)
    {
        int i = 0;
        if (material.hasDiffuseMap())
        {
            material.diffuseMap->bind(i);
            shader.veci("material.diffuseMap", i++);
            shader.veci("material.hasDiffuseMap", true);
        }
        else
        {
            shader.veci("material.hasDiffuseMap", false);
        }
        if (material.hasSpecularMap())
        {
            material.specularMap->bind(i);
            shader.veci("material.specularMap", i++);
            shader.veci("material.hasSpecularMap", true);
        }
        else
        {
            shader.veci("material.hasSpecularMap", false);
        }
        if (material.hasNormalMap())
        {
            material.normalMap->bind(i);
            shader.veci("material.normalMap", i++);
            shader.veci("material.hasNormalMap", true);
        }
        else
        {
            shader.veci("material.hasNormalMap", false);
        }
    
        shader.vec3f("material.ambient", glm::value_ptr(material.ambient * parentMaterial.ambient));
        shader.vec3f("material.diffuse", glm::value_ptr(material.diffuse * parentMaterial.diffuse));
        shader.vec3f("material.specular", glm::value_ptr(material.specular * parentMaterial.specular));
        shader.vecf("material.shininess", (material.shininess + parentMaterial.shininess) / 2);
    }
    shader.Mat4f("model", glm::value_ptr(parentTransform.mat4() * transform.mat4()));
    glCall(glBindVertexArray(VAO));
    glCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
    glCall(glBindVertexArray(0));
}

} // namespace gfx
