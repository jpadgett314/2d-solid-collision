#include "geo_util.h"

using std::abs;

bool rectOverlap(const Rect& a, const Rect& b)
{
    Vec d1 = b.min - a.max;
    Vec d2 = a.min - b.max;

    if (d1.x > 0.0f || d1.y > 0.0f)
        return false;

    if (d2.x > 0.0f || d2.y > 0.0f)
        return false;

    return true;
}

bool segOverlap(const Segment& s1, const Segment& s2)
{
    if (dot(s1.p2 - s1.p1, s2.p1 - s1.p1) <= 0 &&
        dot(s1.p2 - s1.p1, s2.p2 - s1.p1) <= 0)
        return false;

    if (dot(s1.p1 - s1.p2, s2.p1 - s1.p2) <= 0 &&
        dot(s1.p1 - s1.p2, s2.p2 - s1.p2) <= 0)
        return false;

    return true;
}

bool segOverlap(const Segment& s1, const Segment& s2, Vec& overlap)
{
    if (!segOverlap(s1, s2))
        return false;

    Vec u, v;
    if ( dot(s2.p2 - s2.p1, s1.p2 - s1.p1) > 0 )
    {
        u = vectorProject(s2.p2 - s1.p1, s1.p2 - s1.p1);
        v = vectorProject(s2.p1 - s1.p2, s1.p2 - s1.p1);
    }
    else
    {
        u = vectorProject(s2.p1 - s1.p1, s1.p2 - s1.p1);
        v = vectorProject(s2.p2 - s1.p2, s1.p2 - s1.p1);
    }

    if (mag(v) < mag(u))
         overlap = v;
    else
         overlap = u;

    return true;
}

Vec segmentNormal(const Vec& e1, const Vec& e2)
{
    return Vec{-((e2 - e1).y), (e2 - e1).x};
}

Vec segmentNormal(const Vec& e1, const Vec& e2, const Vec& towards)
{
    Vec target{-((e2 - e1).y), (e2 - e1).x};
    Vec v = towards - e1;

    if(dot(target, v) < 0)
        target = target * -1;

    return target;
}

Vec projectPoint(const Vec& pt, const Vec& axis)
{
    /** Derivation

    y = m*x; // equation of axis. m is known.
    v = xi + yj  // vector along axis. x and y are unknowns.
    u = ai + bj  // vector to some arbitrary point; a and b are known.
    d = v - u = (x-a)i + (y-b)j  // vector from u to v

    // find x and y which causes the dot of v and d to be 0
    v . d = 0   ==>    x*(x-a) + y*(y-b) = 0
                ==>    y*(y-b) = -x*(x-a)
                ==>    m*(y-b) = -(x-a)
                ==>    m*(m*x-b) = -(x-a)
                ==>    m^2 * x - m*b = -x + a
                ==>    m^2 * x + x = a + m*b
                ==>    x * ( m^2 + 1 ) = a + m*b
                ==>    x = ( a + m*b ) / ( m^2 + 1 )
                ==>    y = m * x = m * ( a + m*b ) / ( m^2 + 1 )
    */

    if( abs(axis.y) > abs(axis.x) )
    {
        float m = axis.x / axis.y;
        float y = ( pt.y + m * pt.x ) / ( m*m + 1 );
        return {m*y, y};
    }
    else
    {
        float m = axis.y / axis.x;
        float x = ( pt.x + m * pt.y ) / ( m*m + 1 );
        return {x, m*x};
    }
}

bool projectPoint(const Vec& pt, const Segment& onto, Vec& result)
{
    Vec m = {onto.p2.x - onto.p1.x, onto.p2.y - onto.p1.y};
    result = projectPoint(pt, m);
    Segment seg = { projectPoint(onto.p1, m),
                    projectPoint(onto.p2, m) };

    return segContainsPt(seg, result);
}

bool segContainsPt(const Segment& seg, const Vec& pt)
{
    /** Derivation

        // endpoints of segment
        p1 = <u1, u2>
        p2 = <v1, v2>

        // parametric equation for segment, given above endpoints
        { x = (v1 - u1) * t1 + u1
          y = (v2 - u2) * t2 + u2 }
        (t from 0 to 1)

        // those equations can be rewritten as:
        t1 = (x - u1) / (v1 - u1)
        t2 = (y - u2) / (v2 - u2)

        // a point is on the segment if 0 <= t1 <= 1,
        // and if t2 approximately equals t1.
    */

    float t1 = (pt.x - seg.p1.x) / (seg.p2.x - seg.p1.x);
    float t2 = (pt.y - seg.p1.y) / (seg.p2.y - seg.p1.y);
    float epsilon  = 0.001f;

    if(abs(t1 - t2) < epsilon && t1 >= 0 && t1 <= 1)
        return true;
    else
        return false;
}
