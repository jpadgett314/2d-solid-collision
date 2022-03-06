#ifndef RIGID_OBJECT_H
#define RIGID_OBJECT_H

#include "vec.h"
#include "rect.h"
#include "segment.h"
#include "collision.h"

class Object
{
public:
    /// Object data type
    enum Category {Base, Polygon, Circle, Cluster};

    /// Public data
    Vec v;
    float omega;
    float m;
    float i;
    float elasticity;
    float f_k;
    float f_s_max;
    Rect bound_box;

    /// Constructors
    Object();
    Object(Vec in_cog, float in_theta = 0, float in_mass = 8000000);

    /// Destructor
    virtual ~Object() {};

    /// Accessors
    const Vec& getCog() const;
    const float& getTheta() const;
    virtual Category category() const;

    /// Mutators
    void setCog(const Vec& c);
    void setTheta(const float& t);
    void translate(const Vec& disp);

    /// Facilitators
    virtual void update(float t);
    virtual Vec closestPoint(const Vec& pt) const;
    virtual Vec furthestPoint(const Vec& dir) const;
    virtual Segment projectOnto(const Vec& axis) const;
    virtual bool intersectedBy(Object& o, Collision& c);
    virtual bool checkIntersect(Object& o, Collision& c);

protected:
    Vec cog;
    float theta;
};

#endif // RIGID_OBJECT_H
