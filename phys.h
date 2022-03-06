#ifndef PHYS_H
#define PHYS_H

#include <string>
#include <vector>
#include <memory>

#include "geo_util.h"
#include "phys_util.h"
#include "collision.h"
#include "rigid_polygon.h"
#include "rigid_circle.h"
#include "rigid_cluster.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::dynamic_pointer_cast;

typedef unsigned int uint;
typedef unsigned char uchar;

struct SimulatorSettings
{
    SimulatorSettings() {}

    bool gravityEnabled;
    bool collisionsEnabled;
    bool boundariesEnabled;

    Rect boundary;
};

class Simulator
{
public:
    /// Constructors
    Simulator();
    Simulator(const Vec&, const Vec&);
    Simulator(const SimulatorSettings&);

    /// Accessors
    const SimulatorSettings & getSettings() const;
    const vector<shared_ptr<Object>> & objSet() const;

    /// Mutators
    template <class T>
    void newObject(T newObj) { objects.push_back(make_shared<T>(newObj)); }
    void eraseObject(uint index);
    void setGravity(Vec (*gravityField)(const Vec&));

    /// Facilitators
    void updateObjects(float t);

    // Temporary Methods
            vector<shared_ptr<Object>> & objs() { return objects; };
            Segment normal_marker;

private:
    float elasticity;
    Vec (*gravity)(const Vec&);
    SimulatorSettings settings;
    vector<shared_ptr<Object>> objects;

    /// Auxiliary functions
    void applyGravity(float t);
    void checkCollisions(float t);
    void checkBoundaries();
    void wallCollision(Object& o1, const Vec& cp, const Vec& norm);
};

///Separate axis theorem test
bool dispatchToChecker(shared_ptr<Object>& ob1, shared_ptr<Object>& ob2, Collision& c);

bool checkCollision(Polygon& ob1, Polygon& ob2, Collision& c);

bool checkCollision(Polygon& ob1, Circle& ob2, Collision& c);

bool checkCollision(Circle& ob1, Circle& ob2, Collision& c);

bool checkCollision(Cluster& ob1, shared_ptr<Object>& ob2, Collision& c);

void getCollisionPoint(const Polygon&, const Polygon&, const Vec&, Vec&, Vec&);

void resolveCollision(Object& o1, Object& o2, const Collision& c);

void separateObjects(Object& ob1, Object& ob2, const Vec& transVec);

#endif // PHYS_H
