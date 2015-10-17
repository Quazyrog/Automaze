#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

class GameWindow
{
    GLFWwindow *glfwWindow_ = nullptr;
    
public:
    GameWindow();
    ~GameWindow();
};

#endif