#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

#include "lgll/geometry.h"
#include "lgll/image.h"
#include "lgll/entity.h"
#include "lgll/scene.h"
#include "lgll/shader.h"

Scene::Scene() {
}

Scene::~Scene() {
    for (Entity *entity : this->entities) {
        std::cout << "Freeing resources associated with " << entity->obj_filename << std::endl;
        entity->free_resources();
    }
}

void Scene::set_program(GLuint program) {
    this->program = program;
}

void Scene::set_camera(camera_t *camera) {
    this->camera = camera;
}

bool Scene::init_resources() {
    for (Entity *entity : this->entities) {
        std::cout << "Loading entity with mesh " << entity->obj_filename << " into program" << std::endl;
        entity->load_into_program(this->program);
    }

    // This is necessary for... some reason?
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    this->entities[0]->attach_shader(this->program);
    glLinkProgram(this->program);

    GLint link_ok = GL_FALSE;
    glGetProgramiv(this->program, GL_LINK_STATUS, &link_ok);

    if (!link_ok) {
        std::cerr << "Error in glLinkProgram" << std::endl;
        return false;
    }

    return true;
}

void Scene::render(GLFWwindow *window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float ratio = 1.0f * width / height;

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);

    glUseProgram(this->program);

    glm::mat4 projection = glm::perspective(this->camera->vfov, ratio, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(this->camera->position, this->camera->target, this->camera->up);

    for (Entity *entity : this->entities) {
        //mesh->attach_shader(this->program);

        //glLinkProgram(this->program);

        //GLint link_ok = GL_FALSE;
        //glGetProgramiv(this->program, GL_LINK_STATUS, &link_ok);

        //if (!link_ok) {
            //std::cerr << "Error in glLinkProgram" << std::endl;
            //print_log(this->program);
            //break;
        //}

        entity->update_uniforms(projection, view, this->camera->position);

        entity->bind_to_program(program);
        entity->render(this->program);
        //mesh->detach_shader(this->program);
    }
}

void Scene::register_entity(Entity *entity) {
    this->entities.push_back(entity);

    // Load geometry

    string filename(entity->obj_filename);

    auto it = this->meshes.find(filename);
    if (it == this->meshes.end()) {
        std::cout << "Loading new mesh " << filename << std::endl;
        this->meshes[filename] = load_obj(filename.c_str());
    } else {
        std::cout << "Loaded existing mesh " << filename << std::endl;
    }

    entity->mesh = &this->meshes[filename];
}

void Scene::unregister_entity(Entity *entity) {
    this->entities.erase(std::remove(this->entities.begin(), this->entities.end(), entity), this->entities.end());
}

