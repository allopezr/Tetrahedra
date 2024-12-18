#include "stdafx.h"
#include "BoundaryVolumeHierarchy.h"

#include <bvh/v2/default_builder.h>
#include <bvh/v2/thread_pool.h>
#include <bvh/v2/executor.h>
#include <bvh/v2/stack.h>

//

Tet::BoundaryVolumeHierarchy::BoundaryVolumeHierarchy()
{
}

Tet::BoundaryVolumeHierarchy::~BoundaryVolumeHierarchy()
{
}

void Tet::BoundaryVolumeHierarchy::build(const std::vector<VAO::Vertex>& vertices, const std::vector<GLuint>& triangleIndices)
{
    std::vector<Tri> triangles (triangleIndices.size() / 3);

    #pragma omp parallel for
    for (int i = 0; i < static_cast<int>(triangleIndices.size()); i += 3)
    {
        triangles[i / 3] = Tri(
            Vec3(vertices[triangleIndices[i + 0]]._position.x, vertices[triangleIndices[i + 0]]._position.y, vertices[triangleIndices[i + 0]]._position.z),
            Vec3(vertices[triangleIndices[i + 1]]._position.x, vertices[triangleIndices[i + 1]]._position.y, vertices[triangleIndices[i + 1]]._position.z),
            Vec3(vertices[triangleIndices[i + 2]]._position.x, vertices[triangleIndices[i + 2]]._position.y, vertices[triangleIndices[i + 2]]._position.z)
        );
    }

    bvh::v2::ThreadPool threadPool;
    bvh::v2::ParallelExecutor executor(threadPool);

    // Get triangle centers and bounding boxes (required for BVH builder)
    std::vector<BBox> bboxes(triangles.size());
    std::vector<Vec3> centers(triangles.size());
    executor.for_each(0, triangles.size(), [&](size_t begin, size_t end)
    {
        for (size_t i = begin; i < end; ++i)
        {
            bboxes[i] = triangles[i].get_bbox();
            centers[i] = triangles[i].get_center();
        }
    });

    typename bvh::v2::DefaultBuilder<Node>::Config config;
    config.quality = bvh::v2::DefaultBuilder<Node>::Quality::High;
    auto bvh = bvh::v2::DefaultBuilder<Node>::build(threadPool, bboxes, centers, config);

    // This precomputes some data to speed up traversal further.
    _triangles = std::vector<PrecomputedTri>(triangles.size());
    executor.for_each(0, triangles.size(), [&](size_t begin, size_t end)
    {
        for (size_t i = begin; i < end; ++i)
            _triangles[i] = triangles[bvh.prim_ids[i]];
    });
}

bool Tet::BoundaryVolumeHierarchy::intersect(const Model3D* model, const glm::mat4& srcModelMatrix, const glm::mat4& targetModelMatrix) const
{
    bool bFoundIntersection = false;
    std::vector<VAO::Vertex> vertices = model->vertices();
    std::vector<GLuint> triangleIndices = model->indices(VAO::IBO_TRIANGLE);

    #pragma omp parallel for
    for (int i = 0; i < static_cast<int>(triangleIndices.size()); i += 3)
    {
        glm::vec3 transformedVertices[3];
        for (size_t j = 0; j < 3; j++)
        {
            glm::vec4 vertex = targetModelMatrix * srcModelMatrix * glm::vec4(vertices[triangleIndices[i + j]]._position, 1.0);
            transformedVertices[j] = glm::vec3(vertex.x, vertex.y, vertex.z);
        }

        for (size_t j = 0; j < 3 && !bFoundIntersection; j++)
        {
            const glm::vec3& origin = transformedVertices[j];
            const glm::vec3& direction = glm::normalize(transformedVertices[(j + 1) % 3] - transformedVertices[j]);

            auto ray = Ray {
                Vec3(origin.x, origin.y, origin.z),
                Vec3(direction.x, direction.y, direction.z),
                FLT_EPSILON,        
                FLT_MAX               
            };

            static constexpr size_t stackSize = 64;
            static constexpr bool useRobustTraversal = false;

            // Traverse the BVH
            bvh::v2::SmallStack<Bvh::Index, stackSize> stack;
            _bvh.intersect<true, useRobustTraversal>(ray, _bvh.get_root().index, stack, [&](size_t begin, size_t end)
            {
                for (size_t i = begin; i < end; ++i)
                    if (auto hit = _triangles[i].intersect(ray))
                    {
                        bFoundIntersection = true;
                        return true;
                    }

                return false;
            });
        }
    }

    _bvh.nodes()[0];

    return bFoundIntersection;
}
