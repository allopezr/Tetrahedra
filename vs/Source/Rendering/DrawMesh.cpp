#include "stdafx.h"
#include "DrawMesh.h"

// Public methods

Tet::DrawMesh::DrawMesh(): Model3D()
{
}

Tet::DrawMesh::~DrawMesh()
= default;

Tet::DrawMesh* Tet::DrawMesh::loadModelOBJ(const std::string& path)
{
    std::string binaryFile = path.substr(0, path.find_last_of('.')) + BINARY_EXTENSION;

    if (std::filesystem::exists(binaryFile))
    {
        this->loadModelBinaryFile(binaryFile);
    }
    else
    {
        const aiScene* scene = _assimpImporter.ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << _assimpImporter.GetErrorString() << '\n';
            return this;
        }

        std::string shortName = aiScene::GetShortFilename(path.c_str());
        std::string folder = path.substr(0, path.length() - shortName.length());

        this->processNode(scene->mRootNode, scene, folder);
        this->writeBinaryFile(binaryFile);
    }

    for (auto& component : _components)
    {
        Tet::DrawMesh::buildVao(component.get());
    }

    return this;
}

// Protected methods

Tet::Model3D::Component* Tet::DrawMesh::processMesh(const aiMesh* mesh, const aiScene* scene, const std::string& folder)
{
    std::vector<VAO::Vertex> vertices(mesh->mNumVertices);
    std::vector<GLuint> indices(mesh->mNumFaces * 3);
    AABB aabb;

    // Vertices
    int numVertices = static_cast<int>(mesh->mNumVertices);

    for (int i = 0; i < numVertices; i++)
    {
        VAO::Vertex vertex;
        vertex._position = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex._normal = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0]) vertex._textCoord = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

        vertices[i] = vertex;
        aabb.update(vertex._position);
    }

    // Indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices[i * 3 + j] = face.mIndices[j];
    }

    Component* component = new Component;
    component->_vertices = std::move(vertices);
    component->_indices[VAO::IBO_TRIANGLE] = std::move(indices);
    component->_aabb = aabb;
    component->completeTopology();

    return component;
}

void Tet::DrawMesh::processNode(const aiNode* node, const aiScene* scene, const std::string& folder)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _components.push_back(std::unique_ptr<Component>(this->processMesh(mesh, scene, folder)));
        _aabb.update(_components[_components.size() - 1]->_aabb);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene, folder);
    }
}
