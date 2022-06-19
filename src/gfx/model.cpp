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
void Model::loadModelFromPath(const char *filePath)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        filePath, 
        aiProcess_Triangulate
    );
    
    if (!scene)
    {
        std::cout << importer.GetErrorString() << '\n';
        ASSERT(false, "");
    }
    
    unsigned int meshIndex = scene->mRootNode->mChildren[0]->mMeshes[0];
    const aiMesh* mesh = scene->mMeshes[meshIndex];

    vertexCount = mesh->mNumVertices;
    vertices.resize(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        vertices[i].position.x = mesh->mVertices[i].x;
        vertices[i].position.y = mesh->mVertices[i].y;
        vertices[i].position.z = mesh->mVertices[i].z;
        
        vertices[i].normal.x = mesh->mNormals[i].x;
        vertices[i].normal.y = mesh->mNormals[i].y;
        vertices[i].normal.z = mesh->mNormals[i].z;

        if (mesh->mColors[0])
        {
            vertices[i].color.r = mesh->mColors[0][i].r;
            vertices[i].color.g = mesh->mColors[0][i].g;
            vertices[i].color.b = mesh->mColors[0][i].b;
        }
        else
        {
            vertices[i].color.r = 1;
            vertices[i].color.g = 1;
            vertices[i].color.b = 1;
        }

        vertices[i].uv.x = mesh->mTextureCoords[0][i].x;
        vertices[i].uv.y = mesh->mTextureCoords[0][i].y;
    }

    indexCount = mesh->mNumFaces * 3;
    indices.resize(indexCount);
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        unsigned int index = i * 3;
        indices[index + 0] = mesh->mFaces[i].mIndices[0];
        indices[index + 1] = mesh->mFaces[i].mIndices[1];
        indices[index + 2] = mesh->mFaces[i].mIndices[2];
    }
    
    Model::mesh.load(vertices, indices);
}

void Model::bind()
{
    mesh.bind();
}
void Model::unBind()
{
    mesh.unBind();
}
void Model::draw()
{
    mesh.draw();
}

} // namespace gfx
