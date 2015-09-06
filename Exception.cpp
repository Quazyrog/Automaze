#include "Exception.hpp"


const char* IndexError::what() const noexcept
{
    return "IndexError";
}
