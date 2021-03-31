#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <string>

#include "scene.h"

using std::string;

class App {
    private:
        Scene *active_scene;
        string window_title;
        int initial_width;
        int initial_height;
        GLuint program;
        GLFWwindow *window;
        void on_resize(int width, int height);
        void (*logic)();
        static void (*cursor_pos)(double x_pos, double y_pos);
        static void (*window_size)(int width, int height);
        static void global_cursor_pos_callback(GLFWwindow* window, double x_pos, double y_pos);
        static void global_window_size_callback(GLFWwindow* window, int width, int height);
    public:
        App(string window_title, int initial_width, int initial_height);
        ~App();
        void set_active_scene(Scene *scene);
        void register_logic_fn(void (*fp)());
        void register_cursor_pos_fn(void (*fp)(double x_pos, double y_pos));
        void register_window_size_fn(void (*fp)(int width, int height));
        bool startup();
        void main_loop();
};
