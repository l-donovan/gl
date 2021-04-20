#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <SDL_image.h>

#include <bitset>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>

#include "lgll/image.h"

// For `ssize_t` support in VS
#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

using std::vector;

std::map<std::string, GLuint> textures;

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
    auto it = textures.find(filename);
    if (it != textures.end()) {
        std::cout << "Loaded existing texture " << filename << std::endl;
        return it->second;
    }

    GLuint texture_id;

    SDL_Surface *res_texture = IMG_Load(filename);
    if (res_texture == nullptr) {
        std::cerr << "Error: can't load image " << filename << ": " << SDL_GetError() << std::endl;
        return -1; // TODO
    }

    GLint internal_fmt;
    GLenum fmt;

    if (res_texture->format->Amask == 0) {
        std::cout << filename << " does not appear to have an alpha channel" << std::endl;
        internal_fmt = GL_RGB;

        switch (res_texture->format->BytesPerPixel) {
            case 1:
                fmt = GL_RGB8;
                break;
            case 2:
                fmt = GL_RGB16_SNORM;
                break;
            case 3:
                break;
            case 4:
                break;
            default:
                break;
        }
    } else {
        internal_fmt = GL_RGBA;
    }

    std::cout << filename << " bits/pixel: " << static_cast<int>(res_texture->format->BitsPerPixel) << std::endl;
    std::cout << filename << " R mask: " << std::bitset<32>(res_texture->format->Rmask) << std::endl;
    std::cout << filename << " G mask: " << std::bitset<32>(res_texture->format->Gmask) << std::endl;
    std::cout << filename << " B mask: " << std::bitset<32>(res_texture->format->Bmask) << std::endl;
    std::cout << filename << " A mask: " << std::bitset<32>(res_texture->format->Amask) << std::endl;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA, // TODO hardcoded
        res_texture->w,
        res_texture->h,
        0,
        GL_RGBA, // TODO hardcoded
        GL_UNSIGNED_BYTE,
        res_texture->pixels);
    SDL_FreeSurface(res_texture);

    textures[filename] = texture_id;

    std::cout << "Loaded new texture " << filename << std::endl;

    return texture_id;
}
