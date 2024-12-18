#include "stdafx.h"
#include "Triangle.h"

// Public methods

Triangle::Triangle(const Vect2d &aa, const Vect2d &bb, const Vect2d &cc): _a(aa), _b(bb), _c(cc)
{
}

Triangle::Triangle(const Triangle &tt): _a(tt._a), _b(tt._b), _c(tt._c) 
{
} 
