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

namespace gfx
{

class Model
{
public:
    Model();
    ~Model();
    
    void loadModelFromPath(std::string filePath);
    void free();
    void draw(ShaderProgram shader);

private:
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture2D> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

private:
    std::string directory;
    std::vector<Mesh> meshes;
    std::vector<Texture2D> texturesLoaded;
};

} // namespace gfx


#endif

