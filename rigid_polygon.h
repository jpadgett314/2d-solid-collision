#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include "rigid_object.h"

using std::vector;

typedef unsigned int uint;
typedef unsigned char uchar;

class Polygon : public Object
{
public:
    /// Constructors
    Polygon();
    Polygon(Vec in_cog, float in_theta = 0, float in_mass = 8000000);

    /// Accessors
    uint verts() const;
    virtual Category category() const;
    const Vec & pt (uint index) const;
    const Vec & vertexBase(uint index) const;
    const Vec & vertexTrans(uint index) const;

    /// Mutators
    void addVertex(const Vec& vert);

    /// Facilitators
    void update(float t) override;
    Vec closestPoint(const Vec& p) const override;
    Vec furthestPoint(const Vec& dir) const override;
    Segment projectOnto(const Vec& axis) const override;

protected:
    vector<Vec> baseVertices;
    vector<Vec> transformedVertices;

    /// Auxiliary functions
    void transformVertices();
};

#endif // POLYGON_H

