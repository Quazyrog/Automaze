#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <string>

class World;

class GameWindow
{
    GLFWwindow *glfwWindow_ = nullptr;
    
    void loadShaders_();
    void loadTextures_();
    void loadVertices_();
    GLuint compileShader_(std::string path, GLuint shader_type);
    void dumpLogs_(GLuint id, bool is_shader);
    
public:
    GameWindow();
    ~GameWindow();
    
    bool loadShaderProgram(std::string vertex_shader_path, std::string fragment_shader_path);
    
    void setWorld(World *world);
    void displayFrame();
};

#endif