#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <memory>
#include "rigid_object.h"

using std::vector;
using std::shared_ptr;
using std::make_shared;

typedef unsigned int uint;
typedef unsigned char uchar;

class Cluster : public Object
{
public:
    /// Constructors
    Cluster(Vec in_cog = {0, 0}, float in_theta = 0);

    /// Accessors
    shared_ptr<Object>& objAt(uint index);
    const vector<shared_ptr<Object>> & objSet() const;
    virtual Category category() const;

    /// Mutators
    template <class T>
    void newObject(T& newObj);
    void eraseObject(uint index);

    /// Facilitators
    void update(float t) override;
    Vec closestPoint(const Vec& p) const override;
    Vec furthestPoint(const Vec& dir) const override;

private:
    vector<Vec> baseCentroids;
    vector<shared_ptr<Object>> objects;

    /// Auxiliary functions
    void updateObject(uint index, float t);
};

template <class T>
void Cluster::newObject(T& newObj)
{
    newObj.v = {0, 0};
    newObj.omega = 0;
    m += newObj.m;
    i += newObj.i + newObj.m * pow(mag(newObj.getCog() - cog), 2);

    baseCentroids.push_back(newObj.getCog() - cog);
    objects.push_back(make_shared<T>(newObj));
}

#endif // CIRCLE_H

