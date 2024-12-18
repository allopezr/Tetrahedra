#include "stdafx.h"
#include "DrawAABB.h"


Tet::DrawAABB::DrawAABB(const AABB& aabb) : Model3D()
{
    Component* component = Tet::DrawAABB::getVoxel();
    vec3 aabbCenter(aabb.center());
    vec3 extent(aabb.extent());

    for (VAO::Vertex& vertex : component->_vertices)
    {
        vertex._position = (vertex._position * 2.0f * extent) + aabbCenter;
    }

    Tet::DrawAABB::buildVao(component);
    this->_components.push_back(std::unique_ptr<Component>(component));
}

Tet::DrawAABB::~DrawAABB()
= default;
