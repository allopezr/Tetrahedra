#pragma once

/* 
 * File:   DrawTriangle.h
 * Author: lidia
 *
 * Created on 5 de enero de 2021, 14:11
 */

#include "Triangle.h"
#include "Model3D.h"


namespace Tet
{
    class DrawTriangle : public Model3D
    {
    protected:
        Triangle _triangle;

    public:
        DrawTriangle(const Triangle& t);
        DrawTriangle(const DrawTriangle& drawTriangle) = delete;
        virtual ~DrawTriangle() {};
    };
}