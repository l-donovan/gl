#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <SDL_image.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

#include "image.h"

using std::vector;

void load_rgbe(const char* filename, vector<rgbe_t> &data) {
    ssize_t bytes_read;
    FILE *fp = fopen(filename, "rb");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    data.resize(size, {0});

    char str[1024];
    bool verified = false;

    while (fgets(str, sizeof(str), fp) != NULL) {
        if (strlen(str) < 2)
            break;

        if (strcmp(str, "#?RADIANCE\n") == 0) {
            verified = true;
            continue;
        }

        // Do something with these values
        printf("%s", str);
    }

    if (!verified) {
        std::cerr << "Did not find correct header for file " << filename << std::endl;
        return;
    }

    if (fgets(str, sizeof(str), fp) != NULL) {
        int width, height;

        if (sscanf(str, "-Y %d +X %d", &height, &width) < 2) {
            std::cerr << "Found invalid image size for file " << filename << std::endl;
            return;
        }

        std::cout << "Width: " << width << ", height: " << height << std::endl;
    } else {
        std::cerr << "Failed to get image size for file " << filename << std::endl;
        return;
    }

    unsigned char pixel[4];

    while ((bytes_read = fread(pixel, 1, 4, fp)) == 4) {
    }

    if (bytes_read > 0) {
        std::cerr << "File " << filename << " has unfinished pixel data" << std::endl;
        std::cout << bytes_read << std::endl;
        return;
    }

    fclose(fp);
}

GLuint load_texture(const char* filename, const char* texture_name) {
    GLuint texture_id;

    SDL_Surface *res_texture = IMG_Load(filename);
    if (res_texture == nullptr) {
        std::cerr << "Error: can't load image " << filename << ": " << SDL_GetError() << std::endl;
        return -1; // TODO
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGB, // TODO hardcoded
        res_texture->w,
        res_texture->h,
        0,
        GL_RGB, // TODO hardcoded
        GL_UNSIGNED_BYTE,
        res_texture->pixels);
    SDL_FreeSurface(res_texture);

    return texture_id;
}
