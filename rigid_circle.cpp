#include "rigid_circle.h"
#include "geo_util.h"

Circle::Circle() : Object() {}

Circle::Circle(Vec in_cog, float in_radius, Vec in_center) :
    Object(in_cog),
    radius_(in_radius),
    baseCenter_(in_center) {}

const Vec& Circle::center() const
{
    return transCenter_;
}

const float& Circle::radius() const
{
    return radius_;
}

Object::Category Circle::category() const
{
    return Category::Circle;
}

void Circle::update(float t)
{
    cog += v * t;
    theta += omega * t;

    transformCenter();

    bound_box.min = transCenter_ - Vec{radius(), radius()};
    bound_box.max = transCenter_ + Vec{radius(), radius()};
}

void Circle::transformCenter()
{
    transCenter_ = baseCenter_;
    transCenter_.rotate(theta);
    transCenter_ += cog;
}

Vec Circle::closestPoint(const Vec& p) const
{
    return center() + radius() * unit(p - center());
}

Vec Circle::furthestPoint(const Vec& dir) const
{
    return center() + unit(dir) * radius();
}

Segment Circle::projectOnto(const Vec& axis) const
{
    Segment proj;
    Vec center_pt = projectPoint(center(), axis);
    proj.p1 = center_pt + radius() * unit(axis);
    proj.p2 = center_pt - radius() * unit(axis);

    return proj;
}

