#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

/**
 * \brief Exception thrown when invalid array index is detected.
 */
class IndexError
{
public:
    virtual const char *what() const noexcept;
};


/**
 * \brief Exception thrown when program fails to initialize GLFW.
 */
class GLFWInitializationError
{
public:
    virtual const char *what() const noexcept;
};

/**
 * \brief Exception thrown when program fails to initialize GLEW.
 */
class GLEWInitializationError
{
public:
    virtual const char *what() const noexcept;
};

#endif
