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

    std::vector<Texture2D> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    Mesh mesh_(vertices, indices, textures);
    aiColor4D color;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color))
    {
        mesh_.material.ambient.r = color.r;   
        mesh_.material.ambient.g = color.g;   
        mesh_.material.ambient.b = color.b;   
    }
    else
    {
        mesh_.material.ambient = {1, 1, 1};
    }
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color))
    {
        mesh_.material.diffuse.r = color.r;   
        mesh_.material.diffuse.g = color.g;   
        mesh_.material.diffuse.b = color.b;   
    }
    else
    {
        mesh_.material.diffuse = {1, 1, 1};
    }
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color))
    {
        mesh_.material.specular.r = color.r;   
        mesh_.material.specular.g = color.g;   
        mesh_.material.specular.b = color.b;   
    }
    else
    {
        mesh_.material.specular = {1, 1, 1};  
    }
    float shininess;
    if (AI_SUCCESS == aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess))
    {
         mesh_.material.shininess = shininess;
    }
    else
    {
         mesh_.material.shininess = 32;
    }

    return mesh_;
}

std::vector<Texture2D> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture2D> textures;
    
    if (mat->GetTextureCount(type) == 0 && defaultTextures && typeName != "normal")
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
        mesh.draw(shader, material, transform);
    }
}

} // namespace gfx
