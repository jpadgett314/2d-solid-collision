#ifndef GEO_UTIL_H
#define GEO_UTIL_H

#include <cmath>
#include "vec.h"
#include "rect.h"
#include "segment.h"

bool rectOverlap(const Rect& a, const Rect& b);

bool segOverlap(const Segment& s1, const Segment& s2);

bool segOverlap(const Segment& s1, const Segment& s2, Vec& overlap);

Vec segmentNormal(const Vec& e1, const Vec& e2);

Vec segmentNormal(const Vec& e1, const Vec& e2, const Vec& towards);

Vec projectPoint(const Vec& pt, const Vec& axis);

bool projectPoint(const Vec& pt, const Segment& onto, Vec& result);

bool segContainsPt(const Segment&, const Vec&);

#endif // GEO_UTIL_H
