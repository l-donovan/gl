#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <vector>

using std::vector;

struct rgbe_t {
    unsigned char r, g, b, e;
};

void load_rgbe(const char* filename, vector<rgbe_t> &data);
GLuint load_texture(const char* filename, const char* texture_name);
