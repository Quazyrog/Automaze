#ifndef WORLD_HPP
#define WORLD_HPP

#include "Exception.hpp"


class World 
{
public:
    enum class Brick : unsigned char
    {
        FLOOR = 0,
        WALL,
    };
    
protected:
    Brick **data_;
    unsigned int width_;
    unsigned int height_;
    
    unsigned int countAdjacentWalls_(unsigned int x, unsigned int y);
    
public:
    World(unsigned int width, unsigned int height);
    ~World();
    
    unsigned int width() const noexcept {
        return width_;
    }
    
    unsigned int height() const noexcept {
        return height_;
    }
    
    Brick get(unsigned int x, unsigned int y) const {
        if (x >= width_ || y >= height_)
            throw IndexError();
        return data_[x][y];
    }
    
    void set(unsigned int x, unsigned int y, Brick field) {
        if (x >= width_ || y >= height_)
            throw IndexError();
        data_[x][y] = field;
    }
    
    void makeRandom(unsigned int seed) noexcept;
};

#endif
