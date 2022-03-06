#include "phys.h"

#include <iostream>
#include <cmath>

using std::cout;
using std::endl;
using std::abs;

/// SIMULATOR METHODS
Simulator::Simulator() :
    elasticity(0.5f),
    gravity(nullptr)
{
    settings.gravityEnabled = true;
    settings.collisionsEnabled = true;
    settings.boundariesEnabled = false;
    settings.boundary = Rect{0, 0, 0, 0};
}

Simulator::Simulator(const Vec& min, const Vec& max) :
    elasticity(0.5f),
    gravity(nullptr)
{
    settings.gravityEnabled = true;
    settings.collisionsEnabled = true;
    settings.boundariesEnabled = true;
    settings.boundary = Rect{min, max};
}

Simulator::Simulator(const SimulatorSettings& ss) :
    elasticity(0.5f),
    gravity(nullptr)
{
    settings.gravityEnabled = ss.gravityEnabled;
    settings.collisionsEnabled = ss.collisionsEnabled;
    settings.boundariesEnabled = ss.boundariesEnabled;
    settings.boundary = ss.boundary;
}

const SimulatorSettings & Simulator::getSettings() const
{
    return settings;
}

const vector<shared_ptr<Object>> & Simulator::objSet() const
{
    return objects;
}

void Simulator::eraseObject(uint index)
{
    if (index < objects.size())
        objects.erase(objects.begin() + index);
}

void Simulator::setGravity(Vec (*gravityField)(const Vec&))
{
    gravity = gravityField;
}

void Simulator::updateObjects(float t)
{
    for(uint i = 0; i < objects.size(); ++i)
        objects[i]->update(t);

    if (settings.gravityEnabled)
        applyGravity(t);
    if (settings.collisionsEnabled)
        checkCollisions(t);
    if (settings.boundariesEnabled)
        checkBoundaries();
}

void Simulator::applyGravity(float t)
{
    if(gravity)
    {
        for(uint i = 0; i < objects.size(); ++i)
        {
            objects.at(i)->v += (*gravity)(objects.at(i)->getCog());
        }
    }
}

void Simulator::checkCollisions(float t)
{
    for(uint i = 0; i+1 < objects.size(); ++i)
        for(uint j = i+1; j < objects.size(); ++j)
            if ( rectOverlap(objects[i]->bound_box, objects[j]->bound_box) )
            {
                Collision c;
                if (dispatchToChecker(objects[i], objects[j], c))
                {
                    resolveCollision(*objects[i], *objects[j], c);

                    normal_marker.p1 = c.contact_pt;
                    normal_marker.p2 = c.contact_pt + c.normal * 20;
                }
            }
}

void Simulator::checkBoundaries()
{
    for(uint i = 0; i < objects.size(); ++i)
    {
        float xmin = objects.at(i)->bound_box.min.x;
        float ymin = objects.at(i)->bound_box.min.y;
        float xmax = objects.at(i)->bound_box.max.x;
        float ymax = objects.at(i)->bound_box.max.y;

        if(xmin < settings.boundary.min.x)
        {
            objects.at(i)->translate(Vec{settings.boundary.min.x - xmin, 0.0f});
            wallCollision(*objects.at(i), objects.at(i)->furthestPoint(Vec{-1, 0}), Vec{1, 0});

            normal_marker.p1 = objects.at(i)->furthestPoint(Vec{-1, 0});
            normal_marker.p2 = normal_marker.p1 + Vec{1, 0} * 20;
        }
        else if(xmax > settings.boundary.max.x)
        {
            objects.at(i)->translate(Vec{settings.boundary.max.x - xmax, 0.0f});
            wallCollision(*objects.at(i), objects.at(i)->furthestPoint(Vec{1, 0}), Vec{-1, 0});

            normal_marker.p1 = objects.at(i)->furthestPoint(Vec{1, 0});
            normal_marker.p2 = normal_marker.p1 + Vec{-1, 0} * 20;
        }

        if(ymin < settings.boundary.min.y)
        {
            objects.at(i)->translate(Vec{0.0f, settings.boundary.min.y - ymin});
            wallCollision(*objects.at(i), objects.at(i)->furthestPoint(Vec{0, -1}), Vec{0, 1});

            normal_marker.p1 = objects.at(i)->furthestPoint(Vec{0, -1});
            normal_marker.p2 = normal_marker.p1 + Vec{0, 1} * 20;
        }
        else if(ymax > settings.boundary.max.y)
        {
            objects.at(i)->translate(Vec{0.0f, settings.boundary.max.y - ymax});
            wallCollision(*objects.at(i), objects.at(i)->furthestPoint(Vec{0, 1}), Vec{0, -1});

            normal_marker.p1 = objects.at(i)->furthestPoint(Vec{0, 1});
            normal_marker.p2 = normal_marker.p1 + Vec{0, -1} * 20;
        }
    }
}

void Simulator::wallCollision(Object& o1, const Vec& cp, const Vec& norm)
{
    Vec cp_r1 = cp - o1.getCog();
    Vec vi_rel = o1.v;
    Vec cp_vi_rel = o1.v + cross(o1.omega, cp_r1);

    float e = o1.elasticity < elasticity? o1.elasticity : elasticity;
    float J1 = -(1 + e) * dot(vi_rel, norm) / ( 1/o1.m );
    float J2 = -(1 + e) * dot(cp_vi_rel, norm) / ( 1/o1.m +
          pow(dot(cp_r1, norm), 2)/o1.i );

    o1.omega += cross(cp_r1, J2 * norm) / o1.i;
    o1.v += (J1 / o1.m) * norm;
}

/// HELPER FUNCTIONS

bool dispatchToChecker(shared_ptr<Object>& ob1, shared_ptr<Object>& ob2, Collision& c)
{
    if(ob1->category() == Object::Polygon && ob2->category() == Object::Polygon)
    {
        return checkCollision(*dynamic_pointer_cast<Polygon>(ob1),
                              *dynamic_pointer_cast<Polygon>(ob2), c);
    }

    else if(ob1->category() == Object::Polygon && ob2->category() == Object::Circle)
    {
        return checkCollision(*dynamic_pointer_cast<Polygon>(ob1),
                              *dynamic_pointer_cast<Circle>(ob2), c);
    }

    else if(ob1->category() == Object::Circle && ob2->category() == Object::Polygon)
    {
        return checkCollision(*dynamic_pointer_cast<Polygon>(ob2),
                              *dynamic_pointer_cast<Circle>(ob1), c);
    }

    else if(ob1->category() == Object::Circle && ob2->category() == Object::Circle)
    {
        return checkCollision(*dynamic_pointer_cast<Circle>(ob1),
                              *dynamic_pointer_cast<Circle>(ob2), c);
    }

    else if(ob1->category() == Object::Cluster)
    {
        return checkCollision(*dynamic_pointer_cast<Cluster>(ob1), ob2, c);
    }

    else if(ob2->category() == Object::Cluster)
    {
        return checkCollision(*dynamic_pointer_cast<Cluster>(ob2), ob1, c);
    }

    return false;
}

bool checkCollision(Polygon& ob1, Polygon& ob2, Collision& c)
{
    Vec overlap;
    vector<Vec> axes;
    c.mintv.set(ob1.bound_box.width(), ob1.bound_box.height());

    for(uint i = 0; i < ob1.verts(); ++i)
        axes.push_back(segmentNormal(ob1.pt(i), ob1.pt(i+1)));
    for(uint i = 0; i < ob2.verts(); ++i)
        axes.push_back(segmentNormal(ob2.pt(i), ob2.pt(i+1)));

    for(uint i = 0; i < axes.size(); ++i)
    {
        if(!segOverlap(ob1.projectOnto(axes[i]), ob2.projectOnto(axes[i]), overlap))
            return false;
        else if(mag(overlap) < mag(c.mintv))
        {
            c.mintv = overlap;
            if(i < ob1.verts())
                getCollisionPoint(ob2, ob1, axes[i], c.contact_pt, c.normal);
            else
                getCollisionPoint(ob1, ob2, axes[i], c.contact_pt, c.normal);
        }
    }

    return true;
}

bool checkCollision(Polygon& ob1, Circle& ob2, Collision& c)
{
    for(uint i = 0; i < ob1.verts(); ++i)
    {
        Vec pt_disp {ob1.pt(i) - ob2.center()};

        if(mag(pt_disp) < ob2.radius())
        {
            c.contact_pt = ob1.pt(i);
            c.normal = unit(pt_disp);
            c.mintv = c.normal * ( mag(pt_disp) - ob2.radius() );
            return true;
        }
    }

    Vec overlap;
    c.mintv.set(ob1.bound_box.width(), ob1.bound_box.height());

    for(uint i = 0; i < ob1.verts(); ++i)
    {
        Vec axis = -1 * segmentNormal(ob1.pt(i), ob1.pt(i+1), ob1.getCog());
        Segment polyProj = ob1.projectOnto(axis);
        Segment circProj = ob2.projectOnto(axis);

        if(!segOverlap(polyProj, circProj, overlap))
            return false;
        else if(mag(overlap) < mag(c.mintv))
        {
            c.normal = unit(axis);
            c.mintv = overlap;
            c.contact_pt = ob2.center();
            if(segContainsPt(polyProj, circProj.p1))
                c.contact_pt += ob2.radius() * unit(circProj.p1 - circProj.p2);
            else
                c.contact_pt += ob2.radius() * unit(circProj.p2 - circProj.p1);
        }
    }

    return true;
}

bool checkCollision(Circle& ob1, Circle& ob2, Collision& c)
{
    if(mag(ob2.center() - ob1.center()) > ob1.radius() + ob2.radius())
        return false;

    c.normal = unit(ob2.center() - ob1.center());
    c.mintv = c.normal * (ob1.radius() + ob2.radius()) - (ob2.center() - ob1.center());
    c.contact_pt = ob1.center() + c.normal * ob1.radius();

    return true;
}

bool checkCollision(Cluster& ob1, shared_ptr<Object>& ob2, Collision& c)
{
    for (uint i = 0; i < ob1.objSet().size(); ++i)
        if(dispatchToChecker(ob1.objAt(i), ob2, c))
            return true;

    return false;
}

void getCollisionPoint(const Polygon& clding, const Polygon& clded,
                       const Vec& axis, Vec& cp, Vec& norm)
{
    Vec df = projectPoint(clded.getCog(), axis);
    Vec di = projectPoint(clding.getCog(), axis);
    norm = (di - df) * (1 / mag(di - df));
    cp = clding.furthestPoint(df - di);
}

void resolveCollision(Object& o1, Object& o2, const Collision& c)
{
    separateObjects(o1, o2, c.mintv);

    Vec cp_r1 = c.contact_pt - o1.getCog();
    Vec cp_r2 = c.contact_pt - o2.getCog();
    Vec norm = c.normal;

    Vec vi_rel = o1.v - o2.v;
    Vec cp_vi_rel = o1.v + cross(o1.omega, cp_r1) - o2.v - cross(o2.omega, cp_r2);

    float e = o1.elasticity < o2.elasticity? o1.elasticity : o2.elasticity;

    float J1 = -(1 + e) * dot(vi_rel, norm) / ( 1/o1.m + 1/o2.m );
    float J2 = -(1 + e) * dot(cp_vi_rel, norm) / ( 1/o1.m + 1/o2.m +
          pow(dot(cp_r1, norm), 2)/o1.i + pow(dot(cp_r2, norm), 2)/o2.i );

    o1.omega += cross(cp_r1, J2 * norm) / o1.i;
    o2.omega -= cross(cp_r2, J2 * norm) / o2.i;

    o1.v += (J1 / o1.m) * norm;
    o2.v -= (J1 / o2.m) * norm;
}

void separateObjects(Object& ob1, Object& ob2, const Vec& transVec)
{
    Vec di = projectPoint(ob1.getCog(), transVec);
    Vec df = projectPoint(ob2.getCog(), transVec);

    if (dot(df - di, transVec) < 0)
    {
        ob1.translate(transVec * 0.5f);
        ob2.translate(transVec * -0.5f);
    }
    else
    {
        ob1.translate(transVec * -0.5f);
        ob2.translate(transVec * 0.5f);
    }
}
