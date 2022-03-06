#ifndef RECT_H
#define RECT_H

#include "vec.h"

struct Rect
{
    /// Constructors
    Rect() :
        min(0, 0),
        max(0, 0) {}
    Rect(const Vec& v1, const Vec& v2) :
        min(v1),
        max(v2) {}
    Rect(float x1, float y1, float x2, float y2) :
        min(x1, y1),
        max(x2, y2) {}

    /// Facilitators
    float width() const { return max.x - min.x; }
    float height() const { return max.y - min.y; }

    /// Public data
    Vec min;
    Vec max;
};

#endif // RECT_H
