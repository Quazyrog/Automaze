#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <SOIL/SOIL.h>
#include "World.hpp"
#include "GameWindow.hpp"
#include "Exception.hpp"
#include "GLMatrix.hpp"

using namespace std;
using std::string;



void DumpLogs(GLuint id, bool shader)
{
    GLint log_length;
    if (shader)
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
    else
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_length);
    
    std::string log(log_length, ' ');
    if (shader)
        glGetShaderInfoLog(id, log_length, nullptr, &log[0]);
    else
        glGetShaderInfoLog(id, log_length, nullptr, &log[0]);
    
    if (log.length() < 3) {
        std::cerr << "  NA!" << std::endl;
    }
    
    log.resize(log.length() - 2);
    std::cerr << "  ";
    for (char c : log) {
        if (c == '\n')
            std::cerr << "\n  ";
        else 
            std::cerr << c;
    }
    std::cerr << std::endl;
}


GLuint CompileShader(string file, GLenum shader_type)
{
    std::ifstream stream(file);
    if (!stream.good()) {
        std::cerr << "Failed to open shader source '" << file << "'" << std::endl;
        return 0;
    }
    string source;
    while (!stream.eof()) {
        string line;
        std::getline(stream, line);
        source += line + "\n";
    }
    
    const char *source_cstr = source.c_str();
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source_cstr, nullptr);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        std::cerr << "Failed to compile shader '"<< file << "'. Compilation log:\n";
        DumpLogs(shader, true);
        return 0;
    }
    
    return shader;
}


int main()
{
    if (glfwInit() != GL_TRUE) {
        std::cerr << "GLFW initialization failed." << std::endl;
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow *main_window;
    GLFWmonitor *main_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *vidmode = glfwGetVideoMode(main_monitor);
    main_window = glfwCreateWindow(vidmode->width, vidmode->height, "Automaze", main_monitor, nullptr);
    glfwMakeContextCurrent(main_window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed." << std::endl;
        return 1;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    
    
    GLuint shader_program, vertex_shader, fragment_shader;
    vertex_shader = CompileShader("shader.vert", GL_VERTEX_SHADER);
    if (vertex_shader == 0) {
        std::cerr << "Vertex shader compilation failed." << std::endl;
        return 1;
    }
    fragment_shader = CompileShader("shader.frag", GL_FRAGMENT_SHADER);
    if (vertex_shader == 0) {
        glDeleteShader(vertex_shader);
        std::cerr << "Vertex shader compilation failed." << std::endl;
        return 1;
    }
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glBindFragDataLocation(shader_program, 0, "o_color");
    glLinkProgram(shader_program);
    GLint link_status;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE) {
        std::cerr << "Shader program linking failed. Log:" << std::endl;
        DumpLogs(shader_program, false);
        return 1;
    }
    glDetachShader(vertex_shader, shader_program);
    glDeleteShader(vertex_shader);
    glDetachShader(fragment_shader, shader_program);
    glDeleteShader(fragment_shader);
    glUseProgram(shader_program);
    
    
    GLuint vao[2];
    glGenVertexArrays(2, vao);
    GLuint attrib;
    World world(40, 20);
    world.makeRandom(time(nullptr));
    GLuint *vbo = new GLuint [2];
    glGenBuffers(2, vbo);
    
    const GLfloat wall_vertices[] = {
        -1,  -1,   1,  0,  0,
        -1,   1,   1,  0,  1,
         1,  -1,   1,  1,  0,
         1,   1,   1,  1,  1,
         1,  -1,  -1,  2,  0,
         1,   1,  -1,  2,  1,
        -1,  -1,  -1,  3,  0,
        -1,   1,  -1,  3,  1,
        -1,  -1,   1,  4,  0,
        -1,   1,   1,  4,  1,
    };
    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall_vertices), wall_vertices, GL_STATIC_DRAW);
    attrib = glGetAttribLocation(shader_program, "position");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    attrib = glGetAttribLocation(shader_program, "texcoord");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    
    const GLfloat floor_vertices[] = {
        -(float)world.width(),  -1,  -(float)world.height(),  -(float)world.width() / 2.0f,  -(float)world.height() / 2.0f,
        -(float)world.width(),  -1,   (float)world.height(),  -(float)world.width() / 2.0f,   (float)world.height() / 2.0f,
         (float)world.width(),  -1,  -(float)world.height(),   (float)world.width() / 2.0f,  -(float)world.height() / 2.0f,
         (float)world.width(),  -1,   (float)world.height(),   (float)world.width() / 2.0f,   (float)world.height() / 2.0f,
    };
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);
    attrib = glGetAttribLocation(shader_program, "position");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    attrib = glGetAttribLocation(shader_program, "texcoord");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    
    
    GLuint textures[2];
    glGenTextures(2, textures);
    int texwidth, texheight;
    unsigned char *image;
    image = SOIL_load_image("wall.png", &texwidth, &texheight, nullptr, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    image = SOIL_load_image("floor.png", &texwidth, &texheight, nullptr, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    
    
    const GLMatrix projection_matrix = GLMatrix::Projection(static_cast<float>(vidmode->width) / std::max(vidmode->width, vidmode->height), 
                                                            static_cast<float>(vidmode->height) / std::max(vidmode->width, vidmode->height), 
                                                            0.5, 20);
    float player_angle = 0;
    world.set(world.width() / 2 + 1, world.height() / 2 - 1, World::Brick::FLOOR);
    world.set(world.width() / 2 + 1, world.height() / 2,     World::Brick::FLOOR);
    world.set(world.width() / 2 + 1, world.height() / 2 + 1, World::Brick::FLOOR);
    world.set(world.width() / 2,     world.height() / 2 - 1, World::Brick::FLOOR);
    world.set(world.width() / 2,     world.height() / 2,     World::Brick::FLOOR);
    world.set(world.width() / 2,     world.height() / 2 + 1, World::Brick::FLOOR);
    world.set(world.width() / 2 - 1, world.height() / 2 - 1, World::Brick::FLOOR);
    world.set(world.width() / 2 - 1, world.height() / 2,     World::Brick::FLOOR);
    world.set(world.width() / 2 - 1, world.height() / 2 + 1, World::Brick::FLOOR);
    const float x_shift = -(float)world.width();
    const float y_shift = -(float)world.height();
    world.setPlayerPosition(world.width() / 2.0f, world.height() / 2.0f);
    
    auto last_frame = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(main_window)) {
        auto now = std::chrono::high_resolution_clock::now();
        float time_factor = std::chrono::duration<float>(now - last_frame).count() * 40.0f;
        last_frame = now;
        glfwPollEvents();
        if (glfwGetKey(main_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(main_window, GL_TRUE);
        if (glfwGetKey(main_window, GLFW_KEY_UP) == GLFW_PRESS) 
            world.stepPlayer(sin(player_angle) * 0.04f * time_factor, cos(player_angle) * 0.04f * time_factor);
        if (glfwGetKey(main_window, GLFW_KEY_DOWN) == GLFW_PRESS) 
            world.stepPlayer(sin(player_angle) * -0.04f * time_factor, cos(player_angle) * -0.04f * time_factor);
        if (glfwGetKey(main_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            if (glfwGetKey(main_window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(main_window, GLFW_KEY_UP) == GLFW_PRESS) 
                player_angle -= M_PI / 150.0f * time_factor;
            else
                player_angle -= M_PI / 50.0f * time_factor;
        }
        if (glfwGetKey(main_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            if (glfwGetKey(main_window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(main_window, GLFW_KEY_UP) == GLFW_PRESS) 
                player_angle += M_PI / 150.0f * time_factor;
            else
                player_angle += M_PI / 50.0f * time_factor;
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLMatrix player_matrix = GLMatrix::Rotation(player_angle, 0, 1, 0) * GLMatrix::Translation(world.playerX() * 2.0f + x_shift, 0, world.playerY() * 2.0f + y_shift);
        GLMatrix mvp_matrix;
        
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glBindVertexArray(vao[1]);
        mvp_matrix = projection_matrix * player_matrix;
        glUniformMatrix4fv(glGetUniformLocation(shader_program, "MVPMatrix"), 1, GL_FALSE, mvp_matrix.data());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        mvp_matrix = projection_matrix * player_matrix * GLMatrix::Translation(0, 2, 0);
        glUniformMatrix4fv(glGetUniformLocation(shader_program, "MVPMatrix"), 1, GL_FALSE, mvp_matrix.data());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glBindVertexArray(vao[0]);
        for (unsigned int x = 0; x < world.width(); ++x) {
            for (unsigned int y = 0; y < world.height(); ++y) {
                if (world.get(x, y) != World::Brick::WALL) {
                    continue;
                }
                mvp_matrix = projection_matrix * player_matrix * GLMatrix::Translation(-2.0f * x - x_shift - 1.0f, 0, -2.0f * y - y_shift - 1.0f);
                glUniformMatrix4fv(glGetUniformLocation(shader_program, "MVPMatrix"), 1, GL_FALSE, mvp_matrix.data());
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);
            }
        }
        
        glfwSwapBuffers(main_window);
    }
    
    glfwDestroyWindow(main_window);
    return 0;
}
    