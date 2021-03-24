#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

#include "image.h"
#include "mesh.h"
#include "scene.h"
#include "shader.h"

Scene::Scene() { }

Scene::~Scene() {
    for (Mesh *mesh : this->meshes) {
        std::cout << "Freeing resources associated with " << mesh->obj_filename << std::endl;
        mesh->free_resources();
    }
}

void Scene::set_program(GLuint program) {
    this->program = program;
}

void Scene::set_camera(camera_t *camera) {
    this->camera = camera;
}

bool Scene::init_resources() {
    for (Mesh *mesh : this->meshes) {
        std::cout << "Loading mesh " << mesh->obj_filename << " into program" << std::endl;
        mesh->load_into_program(this->program);
    }

    // This is necessary for... some reason?
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    this->meshes[0]->attach_shader(this->program);
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
    float ratio;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);

    glUseProgram(this->program);

    glm::mat4 projection = glm::perspective(this->camera->vfov, 1.0f * this->screen_width / this->screen_height, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(this->camera->position, this->camera->target, this->camera->up);

    for (Mesh *mesh : this->meshes) {
        //mesh->attach_shader(this->program);

        //glLinkProgram(this->program);

        //GLint link_ok = GL_FALSE;
        //glGetProgramiv(this->program, GL_LINK_STATUS, &link_ok);

        //if (!link_ok) {
            //std::cerr << "Error in glLinkProgram" << std::endl;
            //print_log(this->program);
            //break;
        //}

        mesh->update_uniforms(projection, view);
        mesh->bind_to_program(program);
        mesh->render(this->program);
        //mesh->detach_shader(this->program);
    }
}

void Scene::register_mesh(Mesh *mesh) {
    this->meshes.push_back(mesh);
}

void Scene::unregister_mesh(Mesh *mesh) {
    this->meshes.erase(std::remove(this->meshes.begin(), this->meshes.end(), mesh), this->meshes.end());
}

