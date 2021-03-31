#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

char* file_read(const char*);
void print_log(GLuint);
GLuint create_shader(const char*, GLenum);

struct attributes {
    glm::vec3 color = glm::vec3(0.6, 0.5, 0.7);
    GLfloat metallic = 0.5;
    GLfloat roughness = 0.1;
    GLfloat reflectance = 0.5;
    GLfloat clear_coat = 0.1;
    GLfloat clear_coat_roughness = 0.1;
};
