#include "render_sim.h"
#include <iostream>
using namespace std;

void renderPolygon(Polygon& o, sf::RenderWindow& win)
{
    uint vcount = o.verts();

    sf::VertexArray edges(sf::LinesStrip, vcount + 1);
    sf::CircleShape cog(4, 10);
    cog.setFillColor(sf::Color::Green);
    cog.setPosition(o.getCog().x - 4, o.getCog().y - 4);

    for (uint i = 0; i <= vcount; ++i )
        edges[i].position = sf::Vector2f(o.vertexTrans(i).x,
                                         o.vertexTrans(i).y);

    win.draw(cog);
    win.draw(edges);
}

void renderCircle(Circle& c, sf::RenderWindow& win)
{
    sf::CircleShape circ(c.radius(), 30);
    circ.setPosition(c.center().x - c.radius(), c.center().y - c.radius());
    circ.setFillColor(sf::Color::White);
    win.draw(circ);
}

void renderCluster(Cluster& c, sf::RenderWindow& win)
{
    for (uint i = 0; i < c.objSet().size(); ++i)
    {
        if(c.objSet()[i]->category() == Object::Category::Polygon)
            renderPolygon(*(dynamic_pointer_cast<Polygon>(c.objSet()[i])), win);
        else if(c.objSet()[i]->category() == Object::Category::Circle)
            renderCircle(*(dynamic_pointer_cast<Circle>(c.objSet()[i])), win);
        else if(c.objSet()[i]->category() == Object::Category::Cluster)
            renderCluster(*(dynamic_pointer_cast<Cluster>(c.objSet()[i])), win);

        renderBoundBox(*c.objSet()[i], win);
    }

    sf::CircleShape cog(4, 10);
    cog.setFillColor(sf::Color::Green);
    cog.setPosition(c.getCog().x - 4, c.getCog().y - 4);
    win.draw(cog);
}

void renderBoundBox(Object& o, sf::RenderWindow& win)
{
    sf::VertexArray boxlines(sf::LinesStrip, 5);
    boxlines[0].color = sf::Color(128, 0, 0);
    boxlines[1].color = sf::Color(128, 0, 0);
    boxlines[2].color = sf::Color(128, 0, 0);
    boxlines[3].color = sf::Color(128, 0, 0);
    boxlines[4].color = sf::Color(128, 0, 0);
    boxlines[0].position = sf::Vector2f(o.bound_box.min.x,
                                     o.bound_box.min.y);
    boxlines[1].position = sf::Vector2f(o.bound_box.min.x,
                                     o.bound_box.max.y);
    boxlines[2].position = sf::Vector2f(o.bound_box.max.x,
                                     o.bound_box.max.y);
    boxlines[3].position = sf::Vector2f(o.bound_box.max.x,
                                     o.bound_box.min.y);
    boxlines[4].position = sf::Vector2f(o.bound_box.min.x,
                                     o.bound_box.min.y);

    win.draw(boxlines);
}

void renderSimulation(Simulator& p, sf::RenderWindow& win)
{
    for (uint i = 0; i < p.objSet().size(); ++i)
    {
        if(p.objSet()[i]->category() == Object::Category::Polygon)
            renderPolygon(*(dynamic_pointer_cast<Polygon>(p.objSet()[i])), win);
        else if(p.objSet()[i]->category() == Object::Category::Circle)
            renderCircle(*(dynamic_pointer_cast<Circle>(p.objSet()[i])), win);
        else if(p.objSet()[i]->category() == Object::Category::Cluster)
            renderCluster(*(dynamic_pointer_cast<Cluster>(p.objSet()[i])), win);

        renderBoundBox(*p.objSet()[i], win);
    }

    sf::VertexArray normal(sf::LinesStrip, 2);
    normal[0].color = sf::Color(255, 0, 0);
    normal[1].color = sf::Color(255, 0, 0);
    normal[0].position = sf::Vector2f(p.normal_marker.p1.x,
                                      p.normal_marker.p1.y);
    normal[1].position = sf::Vector2f(p.normal_marker.p2.x,
                                      p.normal_marker.p2.y);

    win.draw(normal);
}
