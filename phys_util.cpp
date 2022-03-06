
#include <cmath>
#include "phys_util.h"

using std::abs;
using std::cos;
using std::sin;
using std::acos;

void regularPolygon(Polygon& poly, int sides, int rad)
{
    for(int i = 0; i < sides; ++i)
    {
        Vec vertex{rad * cos( 2*acos(-1) / sides * i ),
                   rad * sin( 2*acos(-1) / sides * i )};
        poly.addVertex(vertex);
    }
}

void setAABB(Rect& AABB, const Polygon& o, float margin)
{
    if(o.verts() > 0)
    {
        float xmin = o.vertexTrans(0).x;
        float xmax = xmin;
        float ymin = o.vertexTrans(0).y;
        float ymax = ymin;

        for(uint i = 1; i < o.verts(); ++i)
        {
            if(o.vertexTrans(i).x < xmin)
                xmin = o.vertexTrans(i).x;
            else if(o.vertexTrans(i).x > xmax)
                xmax = o.vertexTrans(i).x;

            if(o.vertexTrans(i).y < ymin)
                ymin = o.vertexTrans(i).y;
            else if(o.vertexTrans(i).y > ymax)
                ymax = o.vertexTrans(i).y;
        }

        AABB.min.set(xmin - margin, ymin - margin);
        AABB.max.set(xmax + margin, ymax + margin);
    }
}

void setAABB(Rect& AABB, const Cluster& o, float margin)
{
    if(o.objSet().size() > 0)
    {
        float xmin = o.objSet()[0]->bound_box.min.x;
        float xmax = xmin;
        float ymin = o.objSet()[0]->bound_box.min.y;
        float ymax = ymin;

        for(uint i = 0; i < o.objSet().size(); ++i)
        {
            if(o.objSet()[i]->bound_box.min.x < xmin)
                xmin = o.objSet()[i]->bound_box.min.x;
            else if(o.objSet()[i]->bound_box.max.x > xmax)
                xmax = o.objSet()[i]->bound_box.max.x;

            if(o.objSet()[i]->bound_box.min.y < ymin)
                ymin = o.objSet()[i]->bound_box.min.y;
            else if(o.objSet()[i]->bound_box.max.y > ymax)
                ymax = o.objSet()[i]->bound_box.max.y;
        }

        AABB.min.set(xmin - margin, ymin - margin);
        AABB.max.set(xmax + margin, ymax + margin);
    }
}
