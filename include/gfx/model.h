#ifndef MODEL_H
#define MODEL_H

#include "core.h"

#include "ebo.h"
#include "vao.h"
#include "vbo.h"

#include "utils/utils.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

namespace gfx
{
    
class Model
{
public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normal{};
        glm::vec2 uv{};

        bool operator==(const Vertex &other) const 
        {
            return position == other.position &&
                   color == other.color &&
                   normal == other.normal &&
                   uv == other.uv;
        }
    };

    Model();
    ~Model();

    void load(std::vector<Vertex> &vertices, std::vector<uint32_t> &indices);
    void bind();
    void unBind();
    void draw();

private:
    VertexArray vao{};

    VertexBuffer vbo{};
    size_t verticesSize = 0;

    ElementBuffer ebo{};
    size_t indicesSize = 0;

    bool hasIndexBuffer = false;

};

} // namespace gfx


#endif
