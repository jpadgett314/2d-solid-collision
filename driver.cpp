#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "render_sim.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

void createPolygon(int sides, int rad, Simulator& s, sf::RenderWindow& win);

float elapsedTime(sf::Clock& clock);

Vec grav(const Vec& position)
{
    //return { - 0.2 * (position.x - 896/2), - 0.2 * (position.y - 512/2) };

    return {0, 10};
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow win(sf::VideoMode(896, 512), "SFML Test", sf::Style::Default, settings);
    win.setFramerateLimit(60);
    sf::Clock clock, fpsClock, zoomClock;
    sf::Font mainFont;
    mainFont.loadFromFile("mplus-1c-light.ttf");
    float spf = 0, scale = 1.0f;
    sf::Text fpsText("0", mainFont);
    sf::View view = win.getView();
    srand(time(NULL));
    Vec test1(20,20);
    Vec test2(0,20);
    Vec test3(-20,20);
    Vec test4(-20,0);

    Simulator s(Vec{0, 0}, Vec{896, 512});
    //Simulator s;

    Vec v1{20, 10};
    Vec v2{-20, 10};
    Vec v3{0, -10};
    Polygon poly1{Vec{300, 300}};
    poly1.addVertex(v1);
    poly1.addVertex(v2);
    poly1.addVertex(v3);
    poly1.omega = 0.5f;

    //s.newObject<Polygon>(poly1);

    while (win.isOpen())
    {
        ///  ***************************** EVENT HANDLING ****************************** ///
        sf::Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                win.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::G)
                {
                    s.setGravity(&grav);
                }
                if(event.key.code == sf::Keyboard::C)
                {
                    Circle newCirc{ Vec{ win.mapPixelToCoords(sf::Mouse::getPosition(win)).x,
                                         win.mapPixelToCoords(sf::Mouse::getPosition(win)).y}, 5};
                    s.newObject<Circle>(newCirc);
                }
                if(event.key.code == sf::Keyboard::X)
                {
                    if(s.objs().size() > 0)
                    {
                        s.objs().at(s.objs().size()-1)->omega = 0;
                        s.objs().at(s.objs().size()-1)->v = {0, 0};
                    }
                }
                if(event.key.code == sf::Keyboard::Left)
                {
                    s.objs().at(s.objs().size()-1)->v += {-300, 0};
                }
                else if(event.key.code == sf::Keyboard::Right)
                {
                    s.objs().at(s.objs().size()-1)->v += {300, 0};
                }
                if(event.key.code == sf::Keyboard::Up)
                {
                    s.objs().at(s.objs().size()-1)->v += {0, -300};
                }
                else if(event.key.code == sf::Keyboard::Down)
                {
                    s.objs().at(s.objs().size()-1)->v += {0, 300};
                }
                /*
                else if(event.key.code == sf::Keyboard::Space)
                {

                    Cluster cobj{ Vec{100, 100}, 0 };

                    Polygon newObj1{ Vec{ 140, 140 } };
                    regularPolygon(newObj1, 4, 15);

                    Polygon newObj2{ Vec{ 60, 140 } };
                    regularPolygon(newObj2, 4, 15);

                    Polygon newObj3{ Vec{ 100, 50 } };
                    regularPolygon(newObj3, 4, 15);

                    cobj.newObject<Polygon>(newObj1);
                    cobj.newObject<Polygon>(newObj2);
                    cobj.newObject<Polygon>(newObj3);
                    s.newObject<Cluster>(cobj);
                }
                */
                else if(event.key.code == sf::Keyboard::Num3)
                    createPolygon(3, 25, s, win);
                else if(event.key.code == sf::Keyboard::Num4)
                    createPolygon(4, 30, s, win);
                else if(event.key.code == sf::Keyboard::Num5)
                    createPolygon(5, 25, s, win);
                else if(event.key.code == sf::Keyboard::Num6)
                    createPolygon(6, 25, s, win);
                else if(event.key.code == sf::Keyboard::Num7)
                    createPolygon(7, 25, s, win);
                else if(event.key.code == sf::Keyboard::Num8)
                    createPolygon(8, 25, s, win);
                else if(event.key.code == sf::Keyboard::Num9)
                    createPolygon(9, 25, s, win);
                else if(event.key.code == sf::Keyboard::Num0)
                    s.eraseObject(s.objSet().size()-1);
            }
        }

        ///  **************************** LOOPING PORTION ****************************** ///

        //cout << "Q PRESSED: " << std::boolalpha << sf::Keyboard::isKeyPressed(sf::Keyboard::Q) << endl;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
        {
            scale *= 1 + spf;
            view.zoom( 1 + spf );
            win.setView(view);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
        {
            scale *= 1 - spf;
            view.zoom( 1 - spf );
            win.setView(view);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            view.move( -200.0f * spf * scale, 0.0f );
            win.setView(view);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            view.move( 0.0f, 200.0f * spf * scale);
            win.setView(view);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            view.move( 200.0f * spf * scale, 0.0f  );
            win.setView(view);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            view.move( 0.0f, -200.0f * spf * scale);
            win.setView(view);
        }


        stringstream ss;
        spf = elapsedTime(fpsClock);
        ss << int(1.0f / spf);
        fpsText.setString(ss.str());
        s.updateObjects(elapsedTime(clock)*0.2f);

        ///  **************************** DRAWING PORTION ****************************** ///
        win.clear();

        win.draw(fpsText);
        renderSimulation(s, win);

        win.display();
        ///  ************************** END DRAWING PORTION **************************** ///
    }

    return 0;
}

void createPolygon(int sides, int rad, Simulator& s, sf::RenderWindow& win)
{
    Polygon newObj{ Vec{ win.mapPixelToCoords(sf::Mouse::getPosition(win)).x,
                         win.mapPixelToCoords(sf::Mouse::getPosition(win)).y} };
    newObj.m *= sides;
    regularPolygon(newObj, sides, rad);

    newObj.omega = float(rand()%11)/10 - 0.5f;
    float mag = rand()%50 + 50;
    newObj.v = Vec{mag * cos(rand()%360*acos(-1)/180) ,
                   mag * sin(rand()%360*acos(-1)/180)};

    s.newObject<Polygon>(newObj);
}

float elapsedTime(sf::Clock& clock)
{
    sf::Time t = clock.getElapsedTime();
    clock.restart();
    return t.asSeconds();
}
