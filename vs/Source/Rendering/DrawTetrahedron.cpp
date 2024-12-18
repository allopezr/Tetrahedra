#include "stdafx.h"
#include "DrawTetrahedron.h"

using namespace std;

Tet::DrawTetrahedron::DrawTetrahedron(Model3D& model)
{
    float dop[14];
    glm::vec3 vDop[14];
    dopInit(dop);

    for (std::unique_ptr<Component>& component : model._components)
        for (VAO::Vertex& vertex : component->_vertices)
            bvFitWithPoints(dop, vertex._position, vDop);

    for (int i = 0; i < 14; i++)
    {
        float d;
        glm::vec3 direction;

        getFace(dop, i, direction, d);

        // Generate plane vertices using direction and d
        constexpr glm::vec3 vertices [4] = {
            glm::vec3(0.f, 0.f, 0.f) * 100.0f,
            glm::vec3(1.f, 0.f, 0.f) * 100.0f,
            glm::vec3(0.f, 1.f, 0.f) * 100.0f,
            glm::vec3(0.f, 0.f, 1.f) * 100.0f
        };
    }

    //
    Component* component = new Component;
    for (auto& idx : vDop)
    {
        component->_vertices.emplace_back(idx);
        component->_aabb.update(component->_vertices.back()._position);
    }

    // Topology
    component->_indices[VAO::IBO_POINT].insert(
        component->_indices[VAO::IBO_POINT].end(),
        {
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13
        });

    component->_indices[VAO::IBO_TRIANGLE].insert(
        component->_indices[VAO::IBO_TRIANGLE].end(),
        {
            2, 8, 0, RESTART_PRIMITIVE_INDEX,
            6, 2, 0, RESTART_PRIMITIVE_INDEX,
            0, 8, 12, RESTART_PRIMITIVE_INDEX,
            2, 8, 11, RESTART_PRIMITIVE_INDEX,
            6, 0, 10, RESTART_PRIMITIVE_INDEX,
            6, 2, 13, RESTART_PRIMITIVE_INDEX,
            2, 11, 13, RESTART_PRIMITIVE_INDEX,
            6, 10, 4, RESTART_PRIMITIVE_INDEX,
            12, 8, 5, RESTART_PRIMITIVE_INDEX,
            4, 9, 13, RESTART_PRIMITIVE_INDEX,
            9, 3, 7, RESTART_PRIMITIVE_INDEX,
            1, 7, 9, RESTART_PRIMITIVE_INDEX,
            1, 13, 11, RESTART_PRIMITIVE_INDEX,
            1, 9, 13, RESTART_PRIMITIVE_INDEX,
            5, 8, 11, RESTART_PRIMITIVE_INDEX,
            5, 1, 11, RESTART_PRIMITIVE_INDEX,
            5, 1, 7, RESTART_PRIMITIVE_INDEX,
            5, 7, 3, RESTART_PRIMITIVE_INDEX,
            5, 3, 12, RESTART_PRIMITIVE_INDEX,
            9, 4, 10, RESTART_PRIMITIVE_INDEX,
            9, 10, 3, RESTART_PRIMITIVE_INDEX,
            3, 10, 0, RESTART_PRIMITIVE_INDEX,
            0, 12, 3, RESTART_PRIMITIVE_INDEX,
            4, 6, 13, RESTART_PRIMITIVE_INDEX,
        });

    // Check if indices are defined in counter-clockwise order
    for (size_t i = 0; i < component->_indices[VAO::IBO_TRIANGLE].size(); i += 4)
    {
               vec3 v0 = component->_vertices[component->_indices[VAO::IBO_TRIANGLE][i + 0]]._position;
        vec3 v1 = component->_vertices[component->_indices[VAO::IBO_TRIANGLE][i + 1]]._position;
        vec3 v2 = component->_vertices[component->_indices[VAO::IBO_TRIANGLE][i + 2]]._position;

        if (dot(cross(v1 - v0, v2 - v0), v0) < 0.f)
        {
            std::swap(component->_indices[VAO::IBO_TRIANGLE][i + 0], component->_indices[VAO::IBO_TRIANGLE][i + 1]);
        }
    }

    // Calculate normals
    std::vector<vec3> normals (component->_vertices.size(), vec3(0.f));
    std::vector<unsigned> count (component->_vertices.size(), 0);
    for (size_t i = 0; i < component->_indices[VAO::IBO_TRIANGLE].size(); i += 4)
    {
        vec3 v0 = component->_vertices[component->_indices[VAO::IBO_TRIANGLE][i + 0]]._position;
        vec3 v1 = component->_vertices[component->_indices[VAO::IBO_TRIANGLE][i + 1]]._position;
        vec3 v2 = component->_vertices[component->_indices[VAO::IBO_TRIANGLE][i + 2]]._position;

        vec3 normal = normalize(cross(v1 - v0, v2 - v0));
        normals[component->_indices[VAO::IBO_TRIANGLE][i + 0]] += normal;
        normals[component->_indices[VAO::IBO_TRIANGLE][i + 1]] += normal;
        normals[component->_indices[VAO::IBO_TRIANGLE][i + 2]] += normal;

        count[component->_indices[VAO::IBO_TRIANGLE][i + 0]]++;
        count[component->_indices[VAO::IBO_TRIANGLE][i + 1]]++;
        count[component->_indices[VAO::IBO_TRIANGLE][i + 2]]++;
    }

    for (size_t i = 0; i < component->_vertices.size(); i++)
    {
        normals[i] /= count[i];
        component->_vertices[i]._normal = glm::normalize(normals[i]);
    }

    //
    component->generateWireframe();

    Tet::DrawTetrahedron::buildVao(component);
    this->_components.push_back(std::unique_ptr<Component>(component));
    this->_aabb = component->_aabb;
}

Tet::DrawTetrahedron::~DrawTetrahedron()
{

}

//

float Tet::DrawTetrahedron::surface_area_dop_14(float dop[14])
{
    for (int i = 0; i < 14; i++)
        dop[i] *= 1e3;

    vec3 diag = vec3((dop[1] - dop[0]), (dop[3] - dop[2]), (dop[5] - dop[4]));
    float result = 2.f * (diag.x * diag.y + diag.x * diag.z + diag.z * diag.y);

    // fast path for dummy dop (max area)
    if (dop[0] <= -1e30f && dop[1] >= 1e30f)
        return result;

    float d[8];
    d[0] = dop[6] - dot_dop14_n3(vec3(dop[0], dop[2], dop[4]));
    d[1] = dot_dop14_n3(vec3(dop[1], dop[3], dop[5])) - dop[7];
    d[2] = dop[8] - dot_dop14_n4(vec3(dop[0], dop[2], dop[5]));
    d[3] = dot_dop14_n4(vec3(dop[1], dop[3], dop[4])) - dop[9];
    d[4] = dop[10] - dot_dop14_n5(vec3(dop[0], dop[3], dop[4]));
    d[5] = dot_dop14_n5(vec3(dop[1], dop[2], dop[5])) - dop[11];
    d[6] = dop[12] - dot_dop14_n6(vec3(dop[0], dop[3], dop[5]));
    d[7] = dot_dop14_n6(vec3(dop[1], dop[2], dop[4])) - dop[13];

    // dop normals are not normalized, so we need to multiply by 1/sqrt(3)
    float accToSubtract = d[0] * d[0] + d[1] * d[1] + d[2] * d[2] + d[3] * d[3] + d[4] * d[4] + d[5] * d[5] + d[6] * d[6] + d[7] * d[7];
    accToSubtract *= .6339745962155614f;

    float s[12];
    // X pairs: 0:7, 1:6, 2:5, 3:4
    s[0] = max(0.f, d[0] + d[7] - diag.x);
    s[1] = max(0.f, d[1] + d[6] - diag.x);
    s[2] = max(0.f, d[2] + d[5] - diag.x);
    s[3] = max(0.f, d[3] + d[4] - diag.x);
    // Y pairs: 0:4, 1:5, 2:6, 3:7
    s[4] = max(0.f, d[0] + d[4] - diag.y);
    s[5] = max(0.f, d[1] + d[5] - diag.y);
    s[6] = max(0.f, d[2] + d[6] - diag.y);
    s[7] = max(0.f, d[3] + d[7] - diag.y);
    // Z pairs: 0:2, 1:3, 4:6, 5:7
    s[8] = max(0.f, d[0] + d[2] - diag.z);
    s[9] = max(0.f, d[1] + d[3] - diag.z);
    s[10] = max(0.f, d[4] + d[6] - diag.z);
    s[11] = max(0.f, d[5] + d[7] - diag.z);

    float accToAdd = s[0] * s[0] + s[1] * s[1] + s[2] * s[2] + s[3] * s[3]
        + s[4] * s[4] + s[5] * s[5] + s[6] * s[6] + s[7] * s[7]
        + s[8] * s[8] + s[9] * s[9] + s[10] * s[10] + s[11] * s[11];
    accToAdd *= .13397459621556135f;

    return (result - accToSubtract + accToAdd) * 1e-6;
}
