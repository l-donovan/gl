#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

#include <vector>

#include "shader.h"

using std::vector;

void load_obj(const char *filename, vector<glm::vec4> &vertices, vector<glm::vec2> &uvs, vector<glm::vec3> &normals, vector<GLushort> &elements);
struct attributes* generate_attributes(GLfloat **coord_3d, GLfloat **v_color, GLfloat **coord_uv);
