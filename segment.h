#ifndef SEGMENT_H
#define SEGMENT_H

#include "vec.h"

struct Segment
{
    /// Constructors
    Segment() :
        p1(0, 0),
        p2(0, 0) {}
    Segment(const Segment& s) :
        p1(s.p1),
        p2(s.p2) {}
    Segment(const Vec& v1, const Vec& v2) :
        p1(v1),
        p2(v2) {}
    Segment(float x1, float y1, float x2, float y2) :
        p1(x1, y1),
        p2(x2, y2) {}

    /// Facilitators
    float length() const { return mag(p2 - p1); }

    /// Public data
    Vec p1;
    Vec p2;
};

#endif // SEGMENT_H
