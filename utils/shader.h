#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

char* file_read(const char*);
void print_log(GLuint);
GLuint create_shader(const char*, GLenum);

struct attributes {
    GLfloat coord_3d[3];
    GLfloat v_color[3];
    GLfloat coord_uv[2];
};
