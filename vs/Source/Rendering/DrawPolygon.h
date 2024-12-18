#pragma once

/* 
 * File:   DrawPolygon.h
 * Author: lidia
 *
 * Created on 20 de enero de 2021, 12:18
 */

#include "Polygon.h"
#include "Model3D.h"

namespace Tet
{
    class DrawPolygon : public Model3D {
    protected:
        Polygon _polygon;

    public:
        DrawPolygon(const Polygon& polygon);
        DrawPolygon(const DrawPolygon& ddt) = delete;
        virtual ~DrawPolygon() {};
    };
}