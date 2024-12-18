#pragma once

#include <bvh/v2/bvh.h>
#include <bvh/v2/vec.h>
#include <bvh/v2/ray.h>
#include <bvh/v2/node.h>
#include <bvh/v2/tri.h>

#include "Model3D.h"
#include "VAO.h"

namespace Tet
{
    class BoundaryVolumeHierarchy
    {
    protected:
        using Scalar = float;
        using Vec3 = bvh::v2::Vec<Scalar, 3>;
        using BBox = bvh::v2::BBox<Scalar, 3>;
        using Tri = bvh::v2::Tri<Scalar, 3>;
        using Node = bvh::v2::Node<Scalar, 3>;
        using Bvh = bvh::v2::Bvh<Node>;
        using Ray = bvh::v2::Ray<Scalar, 3>;
        using PrecomputedTri = bvh::v2::PrecomputedTri<Scalar>;

    protected:
        Bvh                         _bvh;
        std::vector<PrecomputedTri> _triangles;

    public:
        BoundaryVolumeHierarchy();
        ~BoundaryVolumeHierarchy();

        void build(const std::vector<VAO::Vertex>& vertices, const std::vector<GLuint>& triangleIndices);
        bool intersect(const Model3D* model, const glm::mat4& srcModelMatrix, const glm::mat4& targetModelMatrix) const;
    };
} // namespace Tet

