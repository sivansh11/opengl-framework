#ifndef MESH_H
#define MESH_H

#include "core.h"

#include "ebo.h"
#include "vao.h"
#include "vbo.h"

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

namespace gfx
{
    
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
    Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Texture2D> &textures);
    ~Mesh();

    void load(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Texture2D> &textures);
    void draw(ShaderProgram &shader);
    void free();

    std::vector<Texture2D> textures;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

private:
    void setupMesh();

private:
    VertexArray vao{};
    VertexBuffer vbo{};
    ElementBuffer ebo{};
};

} // namespace gfx


#endif
