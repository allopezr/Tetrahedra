#include "stdafx.h"
#include "DrawRay.h"

// Public methods

Tet::DrawRay::DrawRay (const RayLine &ray): Model3D(), _ray(ray)
{
    //
    this->updateAABB();
}