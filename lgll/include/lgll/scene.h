#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <map>
#include <string>
#include <vector>

#include "entity.h"
#include "geometry.h"

using std::string;

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
        std::map<string, mesh_t> meshes;
        std::vector<Entity*> entities;
    public:
        Scene();
        ~Scene();
        void set_program(GLuint program);
        void set_camera(camera_t *camera);
        bool init_resources();
        void render(GLFWwindow *window);
        void register_entity(Entity *entity);
        void unregister_entity(Entity *entity);
};
