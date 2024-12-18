#pragma once

/* 
 * File:   DrawRay.h
 * Author: lidia
 *
 * Created on 19 de enero de 2021, 20:00
 */

#include "RayLine.h"
#include "Model3D.h"

namespace Tet
{
    class DrawRay : public Model3D 
    {
    protected:
        RayLine _ray;

    public:
        DrawRay(const RayLine& t);
        DrawRay(const DrawRay& drawRay) = delete;
        virtual ~DrawRay() {};
    };
}

