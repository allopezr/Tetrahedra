#pragma once

/* 
 *  File:   DrawLine.h
 *  Author: lidia
 *
 *  Created on 20 de enero de 2021, 12:06
 */

#include "Line.h"
#include "Model3D.h"

namespace Tet
{
    class DrawLine : public Model3D 
    {
    protected:
        Line _line;

    public:
        DrawLine(const Line& t);
        DrawLine(const DrawLine& drawLine) = delete;
        virtual ~DrawLine() {};
    };
}

