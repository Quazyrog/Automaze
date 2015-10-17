#include "Exception.hpp"


const char* IndexError::what() const noexcept
{
    return "IndexError";
}

const char* GLFWInitializationError::what() const noexcept
{
    return "GLFWInitializationError";
}

const char* GLEWInitializationError::what() const noexcept
{
    return "GLEWInitializationError";
}

