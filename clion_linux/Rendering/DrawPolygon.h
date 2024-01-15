#pragma once

/* 
 * File:   DrawPolygonGeo.h
 * Author: lidia
 *
 * Created on 20 de enero de 2021, 12:18
 */

#include "PolygonGeo.h"
#include "Model3D.h"

namespace AlgGeom
{
    class DrawPolygon : public Model3D {
    protected:
        PolygonGeo _polygon;

    public:
        DrawPolygon(PolygonGeo& polygon);
        DrawPolygon(const DrawPolygon& ddt) = delete;
        virtual ~DrawPolygon() {};
    };
}