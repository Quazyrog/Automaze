#include <iostream>
#include "World.hpp"
#include "GameWindow.hpp"
#include "Exception.hpp"

#define EXIT_CODE_NORMAL 0
#define EXIT_CODE_INITIALIZATION_FAILED 1


int main(int argc, char **argv)
{
    GameWindow *window;
    try {
        window = new GameWindow();
    } catch (GLEWInitializationError) {
        std::cerr << "ERROR: cannot initialize main window (GLEW initialization failed)." << std::endl;
        return EXIT_CODE_INITIALIZATION_FAILED;
    } catch (GLFWInitializationError) {
        std::cerr << "ERROR: cannot initialize main window (cannot initialize GLFW library)." << std::endl;
        return EXIT_CODE_INITIALIZATION_FAILED;
    }
    
    return EXIT_CODE_NORMAL;
}
