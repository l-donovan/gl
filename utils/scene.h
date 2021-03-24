#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <vector>

#include "mesh.h"

struct camera_t {
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float vfov;
};

class Scene {
    private:
        GLuint program;
        camera_t* camera;
        GLuint vao;
        int screen_width = 800;
        int screen_height = 600;
    public:
        Scene();
        ~Scene();
        std::vector<Mesh*> meshes;
        void set_program(GLuint program);
        void set_camera(camera_t *camera);
        bool init_resources();
        void render(GLFWwindow *window);
        void register_mesh(Mesh *mesh);
        void unregister_mesh(Mesh *mesh);
};
