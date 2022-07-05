#ifndef MESH_H
#define MESH_H

#include "core.h"

#include "shader.h"
#include "texture.h"

#include "utils/utils.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/components.h"

namespace gfx
{

struct Material 
{
    glm::vec3 ambient{1};
    glm::vec3 diffuse{1};
    glm::vec3 specular{1};
    float shininess = 32;
    std::shared_ptr<Texture2D> diffuseMap{};
    std::shared_ptr<Texture2D> specularMap{};
    std::shared_ptr<Texture2D> normalMap{};
    bool hasDiffuseMap()
    {
        return diffuseMap != nullptr;
    }
    bool hasSpecularMap()
    {
        return specularMap != nullptr;
    }
    bool hasNormalMap()
    {
        return normalMap != nullptr;
    }
};

class Mesh
{
public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal{};
        glm::vec2 uv{};

        bool operator==(const Vertex &other) const 
        {
            return position == other.position &&
                   normal == other.normal &&
                   uv == other.uv;
        }
    };

    Mesh();
    Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, Material &material);
    ~Mesh();

    void load(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, Material &material);
    void draw(ShaderProgram &shader, Material *parentMaterial, Transform &parentTransform);
    void free();

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Material material{};
    Transform transform{};

private:
    void setupMesh();

private:
    GLuint VAO, VBO, EBO;
};

} // namespace gfx


#endif
