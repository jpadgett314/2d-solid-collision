#ifndef COLLISION_H
#define COLLISION_H

#include "vec.h"

struct Collision
{
    Collision() :
        mintv(0, 0),
        normal(0, 0),
        contact_pt(0, 0) {}

    Vec mintv;
    Vec normal;
    Vec contact_pt;
};

#endif // COLLISION_H
