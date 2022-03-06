#include <SFML/Graphics.hpp>
#include "phys.h"

void renderPolygon(Polygon& o, sf::RenderWindow& win);

void renderCircle(Circle& c, sf::RenderWindow& win);

void renderCluster(Cluster& c, sf::RenderWindow& win);

void renderBoundBox(Object& o, sf::RenderWindow& win);

void renderSimulation(Simulator& p, sf::RenderWindow& win);
