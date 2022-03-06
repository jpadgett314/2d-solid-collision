#include "rigid_cluster.h"
#include "phys_util.h"

#include <cmath>

using std::pow;

Cluster::Cluster(Vec in_cog, float in_theta) :
    Object(in_cog, in_theta, 0)
{
    i = 0;
}

shared_ptr<Object>& Cluster::objAt(uint index)
{
    return objects[index];
}

const vector<shared_ptr<Object>> & Cluster::objSet() const
{
    return objects;
}

Object::Category Cluster::category() const
{
    return Category::Cluster;
}

void Cluster::eraseObject(uint index)
{
    if(index < objects.size())
    {
        m -= objects.at(index)->m;
        i -= objects.at(index)->i +
             objects.at(index)->m * pow(mag(objects.at(index)->getCog() - cog), 2);

        baseCentroids.erase(baseCentroids.begin() + index);
        objects.erase(objects.begin() + index);
    }
}

void Cluster::update(float t)
{
    cog += v * t;
    theta += omega * t;

    for(uint i = 0; i < objects.size(); ++i)
        updateObject(i, t);

    setAABB(bound_box, *this);
}

Vec Cluster::closestPoint(const Vec& p) const
{
    if(objects.size() < 1)
        return getCog();

    uint min_index = 0;
    float min_mag = mag(objSet().at(0)->closestPoint(p) - p);

    for(uint i = 1; i < objects.size(); ++i)
    {
        float cmag = mag(objSet().at(i)->closestPoint(p) - p);

        if( cmag < min_mag )
        {
            min_mag = cmag;
            min_index = i;
        }
    }

    return objSet().at(min_index)->closestPoint(p);
}

Vec Cluster::furthestPoint(const Vec& dir) const
{
    // Next problem: MTV for cluster objects has
    //      wrong direction in some instances
    if(objects.size() > 0)
    {
        Vec pt = objSet().at(0)->furthestPoint(dir);

        for(uint i = 1; i < objects.size(); ++i)
        {
            Vec next_pt = objSet().at(i)->furthestPoint(dir);
            if(dot(next_pt - pt, dir) > 0)
                pt = next_pt;
        }

        return pt;
    }

    return cog;
}

void Cluster::updateObject(uint index, float t)
{
    objects.at(index)->setTheta(theta);
    objects.at(index)->setCog(rotate(baseCentroids.at(index), theta) + cog);
    objects.at(index)->update(t);
}
