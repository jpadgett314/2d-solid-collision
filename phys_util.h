#ifndef PHYS_UTIL_H
#define PHYS_UTIL_H

#include "rigid_polygon.h"
#include "rigid_cluster.h"

void regularPolygon(Polygon& poly, int sides, int rad);

void setAABB(Rect& AABB, const Polygon& o, float margin = 0);

void setAABB(Rect& AABB, const Cluster& o, float margin = 0);

#endif // PHYS_UTIL_H
