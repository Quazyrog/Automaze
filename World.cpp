#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <functional>
#include "World.hpp"
#include "FindAndUnion.hpp"


#include <iostream>
#include <iomanip>

World::World(unsigned int width, unsigned int height)
{
    data_ = new Brick* [width];
    for (unsigned int i = 0; i < width; ++i) {
        data_[i] = new Brick[height];
        std::memset(data_[i], 0, height * sizeof(Brick));
    }
    width_ = width;
    height_ = height;
}


World::~World()
{
    for (unsigned int i = 0; i < width_; ++i)
        delete [] data_[i];
    delete [] data_;
}


unsigned int World::countAdjacentWalls_(unsigned int x, unsigned int y)
{
    if (x >= width_ || y >= height_)
        throw IndexError();
    
    unsigned int result = 0;
    unsigned int x0, x1, y0, y1;
    x0 = std::max(0, int(x) - 1);
    x1 = std::min(width_ - 1, x + 1);
    y0 = std::max(0, int(y) - 1);
    y1 = std::min(height_ - 1, y + 1);
    for (unsigned int xi = x0; xi <= x1; ++xi) {
        for (unsigned int yi = y0; yi <= y1; ++yi) {
            if (xi == x && yi == y)
                continue;
            result += (data_[xi][yi] == Brick::WALL ? 1 : 0);
        }
    }
    
    return result;
}


void World::makeRandom(unsigned int seed) noexcept
{
    std::srand(seed);
    for (unsigned int xi = 0; xi < width_; ++xi) {
        for (unsigned int yi = 0; yi < height_; ++yi) {
            if (std::rand() % 4 == 0)
                data_[xi][yi] = Brick::WALL;
            else 
                data_[xi][yi] = Brick::FLOOR;
        }
    }
    
    
    Brick **tmp;
    tmp = new Brick* [width_];
    for (unsigned int i = 0; i < width_; ++i) {
        tmp[i] = new Brick[height_];
        std::memset(tmp[i], 0, height_ * sizeof(Brick));
    }
    
    for (unsigned int generation = 0; generation < 20; ++generation) {
        for (unsigned int xi = 0; xi < width_; ++xi) {
            for (unsigned int yi = 0; yi < height_; ++yi) {
                unsigned int adj_walls = countAdjacentWalls_(xi, yi);
                if (data_[xi][yi] == Brick::WALL) 
                    tmp[xi][yi] = ((adj_walls >= 1 && adj_walls <= 5) ? Brick::WALL : Brick::FLOOR);
                else 
                    tmp[xi][yi] = (adj_walls == 3 ? Brick::WALL : Brick::FLOOR);
            }
        }
        std::swap(tmp, data_);
    }
    
    for (unsigned int i = 0; i < width_; ++i)
        delete [] tmp[i];
    delete [] tmp;
    
    
    for (unsigned int xi = 0; xi < width_; ++xi) 
        data_[xi][0] = data_[xi][height_ - 1] = Brick::WALL;
    for (unsigned int yi = 0; yi < height_; ++yi) 
        data_[0][yi] = data_[width_ - 1][yi] = Brick::WALL;
    
    
    FindAndUnion parts;
    unsigned int **colors = new unsigned int* [width_];
    for (unsigned int i = 0; i < width_; ++i) {
        colors[i] = new unsigned int [height_];
        std::memset(colors[i], static_cast<unsigned char>(-1), height_ * sizeof(unsigned int));
    }
    std::function<void(unsigned int, unsigned int, unsigned int)> dfs = [&colors, &dfs, this](unsigned int x, unsigned int y, unsigned int color) {
        colors[x][y] = color;
        
        if (data_[x - 1][y] == Brick::FLOOR && colors[x - 1][y] != color)
            dfs(x - 1, y, color);
        if (data_[x + 1][y] == Brick::FLOOR && colors[x + 1][y] != color)
            dfs(x + 1, y, color);
        if (data_[x][y - 1] == Brick::FLOOR && colors[x][y - 1] != color)
            dfs(x, y - 1, color);
        if (data_[x][y + 1] == Brick::FLOOR && colors[x][y + 1] != color)
            dfs(x, y + 1, color);
    };
    
    for (unsigned int xi = 1; xi < width_ - 1; ++xi) {
        for (unsigned int yi = 1; yi < height_ - 1; ++yi) {
            if (data_[xi][yi] == Brick::WALL || colors[xi][yi] != static_cast<unsigned int>(-1))
                continue;
            unsigned int color = parts.newSet();
            dfs(xi, yi, color);
        }
    }
    
    for (unsigned int xi = 1; xi < width_ - 1; ++xi) {
        for (unsigned int yi = 1; yi < height_ - 1; ++yi) {
            if (data_[xi][yi] != Brick::WALL)
                continue;
            unsigned int new_color = static_cast<unsigned int>(-1);
            
            if (data_[xi - 1][yi] == Brick::FLOOR && data_[xi + 1][yi] == Brick::FLOOR && parts.representative(colors[xi - 1][yi]) != parts.representative(colors[xi + 1][yi]))
                parts.merge(colors[xi - 1][yi], colors[xi + 1][yi]),  new_color = parts.representative(colors[xi - 1][yi]);
            if (data_[xi - 1][yi] == Brick::FLOOR && data_[xi][yi - 1] == Brick::FLOOR && parts.representative(colors[xi - 1][yi]) != parts.representative(colors[xi][yi - 1]))
                parts.merge(colors[xi - 1][yi], colors[xi][yi - 1]),  new_color = parts.representative(colors[xi - 1][yi]);
            if (data_[xi - 1][yi] == Brick::FLOOR && data_[xi][yi + 1] == Brick::FLOOR && parts.representative(colors[xi - 1][yi]) != parts.representative(colors[xi][yi + 1]))
                parts.merge(colors[xi - 1][yi], colors[xi][yi + 1]),  new_color = parts.representative(colors[xi - 1][yi]);
            if (data_[xi + 1][yi] == Brick::FLOOR && data_[xi][yi - 1] == Brick::FLOOR && parts.representative(colors[xi + 1][yi]) != parts.representative(colors[xi][yi - 1]))
                parts.merge(colors[xi + 1][yi], colors[xi][yi - 1]),  new_color = parts.representative(colors[xi + 1][yi]);
            if (data_[xi + 1][yi] == Brick::FLOOR && data_[xi][yi + 1] == Brick::FLOOR && parts.representative(colors[xi + 1][yi]) != parts.representative(colors[xi][yi + 1]))
                parts.merge(colors[xi + 1][yi], colors[xi][yi + 1]),  new_color = parts.representative(colors[xi + 1][yi]);
            if (data_[xi][yi - 1] == Brick::FLOOR && data_[xi][yi + 1] == Brick::FLOOR && parts.representative(colors[xi][yi - 1]) != parts.representative(colors[xi][yi + 1]))
                parts.merge(colors[xi][yi - 1], colors[xi][yi + 1]),  new_color = parts.representative(colors[xi][yi - 1]);
            
            if (new_color != static_cast<unsigned int>(-1)) {
                data_[xi][yi] = Brick::FLOOR;
                colors[xi][yi] = new_color;
            }
        }
    }
}


