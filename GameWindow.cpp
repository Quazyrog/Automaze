#include "GameWindow.hpp"
#include "Exception.hpp"

#include <fstream>

GameWindow::GameWindow()
{
    if (glfwInit() != GL_TRUE)
        throw GLFWInitializationError();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWmonitor *main_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *vidmode = glfwGetVideoMode(main_monitor);
    glfwWindow_ = glfwCreateWindow(vidmode->width, vidmode->height, "Automaze", main_monitor, nullptr);
    glfwMakeContextCurrent(glfwWindow_);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        throw GLEWInitializationError();
        return 1;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}


GameWindow::~GameWindow()
{
    glfwDestroyWindow(glfwWindow_);
}


GLuint GameWindow::compileShader_(std::string path, GLuint shader_type)
{
    GLuint shader_id = glCreateShader(shader_type);
    std::ifstream source_stream(path);
    
}
