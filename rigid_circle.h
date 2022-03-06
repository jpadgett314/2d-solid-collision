#ifndef CIRCLE_H
#define CIRCLE_H

#include "rigid_object.h"

class Circle : public Object
{
public:
    /// Constructors
    Circle();
    Circle(Vec in_cog, float in_radius, Vec in_center = {0, 0});

    /// Accessors
    const Vec& center() const;
    const float& radius() const;
    virtual Category category() const;

    /// Facilitators
    void update(float t) override;
    Vec closestPoint(const Vec& p) const override;
    Vec furthestPoint(const Vec& dir) const override;
    Segment projectOnto(const Vec& axis) const override;

private:
    float radius_;
    Vec baseCenter_;
    Vec transCenter_;

    /// Auxiliary functions
    void transformCenter();
};

#endif // CIRCLE_H
