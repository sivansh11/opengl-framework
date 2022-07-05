#ifndef MODEL_H
#define MODEL_H

#include "core.h"

#include "gfx/vao.h"
#include "gfx/vbo.h"
#include "gfx/ebo.h"

#include "gfx/mesh.h"

#include "gfx/shader.h"

#include <assimp/Importer.hpp>   
#include <assimp/scene.h>           
#include <assimp/postprocess.h>  

#include "utils/components.h"

namespace gfx
{

class Model
{
public:
    Model();
    ~Model();
    
    void loadModelFromPath(std::string filePath);
    void free();
    void draw(ShaderProgram shader, Material *material);

    std::vector<Mesh>& getMeshes() { return meshes; }

    Material material{};
    Transform transform{};

    bool defaultTextures = true;

private:
    // void processNode(aiNode *node, const aiScene *scene);
    void processNode(aiNode *node, const aiScene *scene, aiMatrix4x4 &transform);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene, aiMatrix4x4 &transform);
    std::shared_ptr<Texture2D> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> texturesLoaded;
};

} // namespace gfx


#endif

