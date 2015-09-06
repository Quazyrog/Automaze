#include <iostream>
#include "World.hpp"


int main(int argc, char **argv)
{
    World world(200, 70);
    world.makeRandom(0);
    for (unsigned int yi = 0; yi < world.height(); ++yi) {
        for (unsigned int xi = 0; xi < world.width(); ++xi) 
            std::cerr << (world.get(xi, yi) == World::Brick::WALL ? '#' : ' ');
        std::cerr << '\n';
    }
    return 0;
}
