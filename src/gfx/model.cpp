#include "gfx/model.h"

#include "core.h"

#include <assimp/Importer.hpp>   
#include <assimp/scene.h>           
#include <assimp/postprocess.h>     

namespace gfx
{

Model::Model()
{

}
Model::~Model()
{

}
void Model::free()
{
    for (auto &mesh: meshes)
    {
        mesh.free();
    }
}
void Model::loadModelFromPath(std::string filePath, bool defaultTextures)
{
    Model::defaultTextures = defaultTextures;
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filePath,
        aiProcess_Triangulate |
        aiProcess_FlipUVs     |
        aiProcess_GenNormals
    );    

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        ASSERT(false, std::string("Assimp Error\n") + importer.GetErrorString());
    }
    directory = filePath.substr(0, filePath.find_last_of('/'));
    processNode(scene->mRootNode, scene);
    DEBUGMESSAGE((std::to_string(meshes.size()) + " Meshes and " + std::to_string(texturesLoaded.size()) + " Textures").c_str());
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture2D> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Mesh::Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.uv = vec;

            // tangent

            // bitangent
        }
        else
        {
            vertex.uv = glm::vec2(0, 0);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    } 

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture2D> diffMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
    textures.insert(textures.end(), diffMaps.begin(), diffMaps.end());

    std::vector<Texture2D> specMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
    textures.insert(textures.end(), specMaps.begin(), specMaps.end());

    return Mesh(vertices, indices, textures);
}

std::vector<Texture2D> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture2D> textures;
    
    if (mat->GetTextureCount(type) == 0 && defaultTextures)
    {
        std::vector<Texture2D> tex;
        Texture2D emptyTex;
        unsigned char *data = new unsigned char[4];
        for (int i = 0; i < 4; i++)
        {
            data[i] = 255;
        } 
        emptyTex.load(1, 1, data, GL_RGBA, typeName);
        tex.push_back(emptyTex);
        return tex;
    }

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < texturesLoaded.size(); j++)
        {
            if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(texturesLoaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture2D texture;
            texture.load((directory + '/' + str.C_Str()).c_str(), typeName);
            texture.path = str.C_Str();
            textures.push_back(texture);
            texturesLoaded.push_back(texture);
        }
    }
    return textures;
}

void Model::draw(ShaderProgram shader)
{
    for (auto &mesh: meshes)
    {
        mesh.draw(shader);
    }
}

} // namespace gfx
