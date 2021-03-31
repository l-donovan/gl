#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "lgll/shader.h"

using std::string;

std::map<std::string, GLuint> shaders;

char* file_read(const char* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char *buffer = (char*) malloc(size + 1);

    if (file.read(buffer, size)) {
        /* worked! */
    }

    buffer[size] = 0;

    return buffer;
}

/**
 * Display compilation errors from the OpenGL shader compiler
 */
void print_log(GLuint object) {
    GLint log_length = 0;

    if (glIsShader(object)) {
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    } else if (glIsProgram(object)) {
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    } else {
        std::cerr << "printlog: Not a shader or a program" << std::endl;
        return;
    }

    char* log = (char*) malloc(log_length);

    if (glIsShader(object))
        glGetShaderInfoLog(object, log_length, NULL, log);
    else if (glIsProgram(object))
        glGetProgramInfoLog(object, log_length, NULL, log);

    std::cerr << log;

    free(log);
}

/**
 * Compile the shader from file 'filename', with error handling
 */
GLuint create_shader(const char* filename, GLenum type) {
    auto it = shaders.find(filename);
    if (it != shaders.end()) {
        std::cout << "Loaded existing shader " << filename << std::endl;
        return it->second;
    }

    const GLchar* source = file_read(filename);

    if (source == nullptr) {
        std::cerr << "Error opening " << filename << std::endl;
        return 0;
    }

    GLuint res = glCreateShader(type);

    const GLchar* sources[] = {
#ifdef GL_ES_VERSION_2_0
        "#version 100\n"  // OpenGL ES 2.0
#else
        "#version 410\n"  // OpenGL 4.1
#endif
    , source };

    glShaderSource(res, 2, sources, NULL);
    free((void*) source);
    glCompileShader(res);
    GLint compile_ok = GL_FALSE;
    glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);

    if (compile_ok == GL_FALSE) {
        std::cerr << filename << ":";
        print_log(res);
        glDeleteShader(res);
        return 0;
    }

    shaders[filename] = res;

    std::cout << "Compiled new shader " << filename << std::endl;

    return res;
}
