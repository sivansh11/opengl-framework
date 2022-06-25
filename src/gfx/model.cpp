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
void Model::loadModelFromPath(std::string filePath)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filePath,
        aiProcess_Triangulate |
        // aiProcess_FlipUVs     |
        aiProcess_GenNormals
    );    

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        ASSERT(false, std::string("Assimp Error\n") + importer.GetErrorString());
    }
    directory = filePath.substr(0, filePath.find_last_of('/'));
    aiMatrix4x4 transform{};
    processNode(scene->mRootNode, scene, transform);
    DEBUGMESSAGE((std::to_string(meshes.size()) + " Meshes and " + std::to_string(texturesLoaded.size()) + " Textures").c_str());
}

// void Model::processNode(aiNode *node, const aiScene *scene)
// {
//     for (unsigned int i = 0; i < node->mNumChildren; i++)
//     {
//         processNode(node->mChildren[i], scene, transform);
//     }

//     for (unsigned int i = 0; i < node->mNumMeshes; i++)
//     {
//         aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
//         meshes.push_back(processMesh(mesh, scene, transform));
//     }
// }

void Model::processNode(aiNode *node, const aiScene *scene, aiMatrix4x4 &transform)
{
    aiMatrix4x4 localTransform = transform * node->mTransformation;

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, localTransform);
    }

    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, localTransform));
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene, aiMatrix4x4 &transform)
{
    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;

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
            // if (mesh->HasTangentsAndBitangents())
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

    std::shared_ptr<Texture2D> diffMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");

    std::shared_ptr<Texture2D> specMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");

    std::shared_ptr<Texture2D> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "normal");

    // Mesh mesh_(vertices, indices, textures);
    Material meshMaterial;
    aiColor4D color;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color))
    {
        meshMaterial.ambient.r = color.r;   
        meshMaterial.ambient.g = color.g;   
        meshMaterial.ambient.b = color.b;   
        if (diffMaps != nullptr)
        {
            meshMaterial.diffuseMap = diffMaps;
        }
    }
    else
    {
        meshMaterial.ambient = {1, 1, 1};
    }
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color))
    {
        meshMaterial.diffuse.r = color.r;   
        meshMaterial.diffuse.g = color.g;   
        meshMaterial.diffuse.b = color.b;   
        if (diffMaps != nullptr)
        {
            meshMaterial.diffuseMap = diffMaps;
        }
    }
    else
    {
        meshMaterial.diffuse = {1, 1, 1};
    }
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color))
    {
        meshMaterial.specular.r = color.r;   
        meshMaterial.specular.g = color.g;   
        meshMaterial.specular.b = color.b; 
        if (specMaps != nullptr)
        {
            meshMaterial.diffuseMap = specMaps;
        }  
    }
    else
    {
        meshMaterial.specular = {1, 1, 1};  
    }
    float shininess;
    if (AI_SUCCESS == aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess))
    {
         meshMaterial.shininess = shininess;
    }
    else
    {
         meshMaterial.shininess = 32;
    }
    Mesh mesh_(vertices, indices, meshMaterial);
    transform.Decompose(*(aiVector3D*)(&mesh_.transform.scale), *(aiVector3D*)(&mesh_.transform.rotation), *(aiVector3D*)(&mesh_.transform.translation));
    return mesh_;
}

std::shared_ptr<Texture2D> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    if (mat->GetTextureCount(type) == 0) return nullptr;

    // if (mat->GetTextureCount(type) == 0 && defaultTextures)
    // {
    //     unsigned char *data = new unsigned char[4];
    //     for (int i = 0 ; i < 4; i++)
    //         data[i] = 255;
    //     std::shared_ptr<Texture2D> tex = std::make_shared<Texture2D>();
    //     tex->load(1, 1, data, GL_RGBA, typeName);
    //     delete[] data;
    //     return tex;
    // }
    std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        if (texturesLoaded.find(str.C_Str()) != texturesLoaded.end())
        {
            return texturesLoaded.at(str.C_Str());
        }
        texture->load((directory + '/' + str.C_Str()).c_str(), typeName);
        texture->path = str.C_Str();
        texturesLoaded[str.C_Str()] = texture;
    }

    return texture;
}

void Model::draw(ShaderProgram shader)
{
    for (auto &mesh: meshes)
    {
        mesh.draw(shader, material, transform);
    }
}

} // namespace gfx
