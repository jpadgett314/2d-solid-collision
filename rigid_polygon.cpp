#include "rigid_polygon.h"
#include "phys_util.h"
#include "geo_util.h"

Polygon::Polygon() : Object() {}

Polygon::Polygon(Vec in_cog, float in_theta, float in_mass) :
    Object(in_cog, in_theta, in_mass) {}

uint Polygon::verts() const
{
    return baseVertices.size();
}

Object::Category Polygon::category() const
{
    return Category::Polygon;
}

const Vec & Polygon::pt(uint index) const
{
    return transformedVertices[index % verts()];
}

const Vec & Polygon::vertexBase(uint index) const
{
    return baseVertices[index % verts()];
}

const Vec & Polygon::vertexTrans(uint index) const
{
    return transformedVertices[index % verts()];
}

void Polygon::addVertex(const Vec& vert)
{
    baseVertices.push_back(vert);
    transformedVertices.push_back(vert);
}

void Polygon::update(float t)
{
    cog += v * t;
    theta += omega * t;
    transformVertices();
    setAABB(bound_box, *this);
}

Vec Polygon::closestPoint(const Vec& p) const
{
    if(verts() < 1)
        return getCog();

    uint min_index = 0;

    for(uint i = 1; i < verts(); ++i)
        if( mag(pt(i) - p) < mag(pt(min_index) - p) )
            min_index = i;

    return pt(min_index);
}

Vec Polygon::furthestPoint(const Vec& dir) const
{
    Vec di = projectPoint(getCog(), dir);

    uint index = 0;
    float closest = 0;

    for(uint i = 0; i < verts(); ++i)
    {
        Vec dpf = projectPoint(pt(i), dir);

        if( dot(dpf - di, dir) > closest )
        {
            closest = dot(dpf - di, dir);
            index = i;
        }
    }

    return pt(index);
}

Segment Polygon::projectOnto(const Vec& axis) const
{
    Segment proj;
    proj.p1 = projectPoint(pt(0), axis);
    proj.p2 = projectPoint(pt(0), axis);

    for(uint i = 1; i < verts(); ++i)
    {
        Vec next = projectPoint(pt(i), axis);

        if(!segContainsPt(proj, next))
        {
            if(mag(next - proj.p1) < mag(next - proj.p2))
                proj.p1 = next;
            else
                proj.p2 = next;
        }
    }

    return proj;
}

void Polygon::transformVertices()
{
    transformedVertices = baseVertices;

    for(uint i = 0; i < verts(); ++i)
    {
        transformedVertices[i].rotate(theta);
        transformedVertices[i] += cog;
    }
}
