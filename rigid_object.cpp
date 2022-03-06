#include "rigid_object.h"
#include "geo_util.h"

Object::Object() :
    v(0, 0),
    omega(0),
    m(8000000),
    i(20000000),
    elasticity(0.2),
    f_k(0.2f),
    f_s_max(0.5f),
    bound_box(0, 0, 0, 0),
    cog(0, 0),
    theta(0) {}

Object::Object(Vec in_cog, float in_theta, float in_mass) :
    v(0, 0),
    omega(0),
    m(in_mass),
    i(20000000),
    elasticity(0.2),
    f_k(0.2f),
    f_s_max(0.5f),
    bound_box(0, 0, 0, 0),
    cog(in_cog),
    theta(in_theta) {}

const Vec& Object::getCog() const
{
    return cog;
}

const float& Object::getTheta() const
{
    return theta;
}

Object::Category Object::category() const
{
    return Category::Base;
}

void Object::setCog(const Vec& c)
{
    cog = c;
}

void Object::setTheta(const float& t)
{
    theta = t;
}

void Object::translate(const Vec& disp)
{
    cog = cog + disp;
}

void Object::update(float t)
{
    cog += v * t;
    theta += omega * t;
}

Vec Object::closestPoint(const Vec& pt) const
{
    return cog;
}

Vec Object::furthestPoint(const Vec& dir) const
{
    return cog;
}

Segment Object::projectOnto(const Vec& axis) const
{
    return {projectPoint(cog, axis), projectPoint(cog, axis)};
}

bool Object::intersectedBy(Object& o, Collision& c)
{
    return false;
}

bool Object::checkIntersect(Object& o, Collision& c)
{
    // Guaranteed to evaluate the left condition first.
    return (intersectedBy(o, c) && o.intersectedBy(*this, c));
}
