#ifndef WORLD_HPP
#define WORLD_HPP

#include "Exception.hpp"


/**
 * \brief Stores information about all fields in maze.
 * The <c>World</c> class is used to store and manage data about entire maze. It can be used to generate random maze as well.
 */
class World 
{
public:
    /**
     * \brief All possible types of fields in the maze.
     */
    enum class Brick : unsigned char
    {
        ///Floot... You can roam here
        FLOOR = 0,
        ///Only ghosts are capable to pass this one
        WALL,
    };
    
protected:
    /**
     * \brief Recrangular area of the maze.
     * Two-dimmensional array of size width_*height_. It stores information about fields in the maze.
     */
    Brick **data_;
    
    /**
     * \brief World's width (in fields)
     */
    unsigned int width_;
    
    /**
     * \brief World's height (in fields)
     */
    unsigned int height_;
    
    /**
     * \brief Counts walls around given field.
     * The <c>countAdjacentWalls_</c> function counts how many of eight fields surrounding given coordinates are walls. Fields outside world are treated like floors.
     * \param x x coordinate of the field
     * \param y y coordinate of the field
     * \return Number of walls (an integer from 0 to 8) surrounding field at (x, y) coordinates
     * \throws IndexError when coordinates supplied are outside the world
     */
    unsigned int countAdjacentWalls_(unsigned int x, unsigned int y);
    
public:
    /**
     * \brief Creates new empty maze of given size
     * Constructs new instance of this class, allocates memory for World::data_ array and fills it with zeros (0 = floor). Size of created world will be immutable once the array is allocated.
     */
    World(unsigned int width, unsigned int height);
    
    /**
     * \brief Deletes this object and it's memory.
     */
    ~World();
    
    /**
     * \biref Returns maze's width.
     * \return Maze's width (in fields)
     */
    unsigned int width() const noexcept {
        return width_;
    }
    
    /**
     * \biref Returns maze's height.
     * \return Maze's height (in fields)
     */
    unsigned int height() const noexcept {
        return height_;
    }
    
    /**
     * \brief Returns type of floor at given coordinates.
     * \param x the x coordinate of the field
     * \param y the y coordinate of the field
     * \returns Type of field at given (x,y) coordinates (aka World::data_[x][y])
     * \throw IndexError when coordinates are outside worlds bounds
     */
    Brick get(unsigned int x, unsigned int y) const {
        if (x >= width_ || y >= height_)
            throw IndexError();
        return data_[x][y];
    }
    
    /**
     * \brief Sets type of floor at given coordinates.
     * \param x the x coordinate of the field
     * \param y the y coordinate of the field
     * \param field new type of given field
     * \throw IndexError when coordinates are outside worlds bounds
     */
    void set(unsigned int x, unsigned int y, Brick field) {
        if (x >= width_ || y >= height_)
            throw IndexError();
        data_[x][y] = field;
    }
    
    /**
     * \brief Replaces current maze with a newly-generated one.
     * The World::makeRandom function replaces maze stored in this instance with an new one. All changes made to the maze by World::set or previous call to WorldL::makeRandom
     * will be lost. It is possible to control what maze will be generated with the seed paramater.
     * 
     * Maze generation consists of following stages:
     * - Generate totally random mix of walls and floors (statistically in ratio 1:3)
     * - Use cellular automaton on generated maze to make it look (much) better
     * - Fill borders with walls
     * - Remove interior walls to make it possible to reach every part of maze from any other (fix coherency)
     * 
     * \param seed argument for srand function called before the generation starts
     */
    void makeRandom(unsigned int seed) noexcept;
};

#endif
