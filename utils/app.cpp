#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>

#include "app.h"
#include "scene.h"

using std::string;

App::App(string window_title, int initial_width, int initial_height) {
    this->window_title = window_title;
    this->initial_width = initial_width;
    this->initial_height = initial_height;
}

App::~App() {
    glDeleteProgram(this->program);
    // glDeleteTextures(1, &texture_id);
}

void App::set_active_scene(Scene *scene) {
    this->active_scene = scene;
}

void (*App::cursor_pos)(double x_pos, double y_pos) = NULL;
void (*App::window_size)(int width, int height) = NULL;

void App::global_cursor_pos_callback(GLFWwindow* window, double x_pos, double y_pos) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    double x_adj = x_pos / width;
    double y_adj = y_pos / height;

    if (x_adj < 0.0 || x_adj > 1.0 || y_adj < 0.0 || y_adj > 1.0)
        return;

    App::cursor_pos(x_adj, y_adj);
}

void App::global_window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    App::window_size(width, height);
}

void App::register_cursor_pos_fn(void (*fp)(double x_pos, double y_pos)) {
    App::cursor_pos = fp;
}

void App::register_window_size_fn(void (*fp)(int width, int height)) {
    App::window_size = fp;
}

void App::register_logic_fn(void (*fp)()) {
    this->logic = fp;
}

void App::main_loop() {
    while (!glfwWindowShouldClose(this->window)) {
        this->logic();
        this->active_scene->render(this->window);

        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }

    glfwDestroyWindow(this->window);
    glfwTerminate();
}

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void glfw_cursor_pos_callback(GLFWwindow* window, double x_pos, double y_pos) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    double x_adj = x_pos / width;
    double y_adj = y_pos / height;

    if (x_adj < 0.0 || x_adj > 1.0 || y_adj < 0.0 || y_adj > 1.0)
        return;

    std::cout << x_adj << std::endl;
}

bool App::startup() {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        std::cerr << "Error: glfwInit" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    this->window = glfwCreateWindow(this->initial_width, this->initial_height, this->window_title.c_str(), nullptr, nullptr);

    if (!this->window) {
        glfwTerminate();
        std::cerr << "Error: can't create window" << std::endl;
        return false;
    }

    glfwSetKeyCallback(this->window, glfw_key_callback);
    glfwSetCursorPosCallback(this->window, App::global_cursor_pos_callback);
    glfwSetWindowSizeCallback(window, App::global_window_size_callback);

    glfwMakeContextCurrent(this->window);
    gladLoadGL();
    glfwSwapInterval(1);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->program = glCreateProgram();

    this->active_scene->set_program(this->program);
    this->active_scene->init_resources();

    return true;
}
