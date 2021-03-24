#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "geometry.h"
#include "image.h"
#include "mesh.h"
#include "shader.h"

using std::vector;

Mesh::Mesh(const char *obj_filename, const char *vertex_shader_filename, const char *fragment_shader_filename) {
    load_obj(obj_filename, this->mesh_vertices, this->mesh_uvs, this->mesh_normals, this->mesh_elements);

    this->vertex_shader_filename = vertex_shader_filename;
    this->fragment_shader_filename = fragment_shader_filename;
    this->object_to_world = glm::mat4(1.0);
    this->obj_filename = obj_filename;
}

Mesh::Mesh(vector<glm::vec4> vertices, vector<glm::vec2> uvs, vector<glm::vec3> normals, vector<GLushort> elements, const char *vertex_shader_filename, const char *fragment_shader_filename) {
    this->mesh_vertices = vertices;
    this->mesh_uvs = uvs;
    this->mesh_normals = normals;
    this->mesh_elements = elements;

    this->vertex_shader_filename = vertex_shader_filename;
    this->fragment_shader_filename = fragment_shader_filename;
    this->object_to_world = glm::mat4(1.0);
    this->obj_filename = "";
}

bool Mesh::load_into_program(GLuint program) {
    // Load data into buffers

    if (!this->mesh_vertices.empty()) {
        glGenBuffers(1, &this->vbo_mesh_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_mesh_vertices);
        glBufferData(GL_ARRAY_BUFFER, this->mesh_vertices.size() * sizeof(glm::vec4), this->mesh_vertices.data(), GL_STATIC_DRAW);
    }

    if (!this->mesh_normals.empty()) {
        glGenBuffers(1, &this->vbo_mesh_normals);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_mesh_normals);
        glBufferData(GL_ARRAY_BUFFER, this->mesh_normals.size() * sizeof(glm::vec3), this->mesh_normals.data(), GL_STATIC_DRAW);
    }

    if (!this->mesh_uvs.empty()) {
        glGenBuffers(1, &this->vbo_mesh_uvs);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_mesh_uvs);
        glBufferData(GL_ARRAY_BUFFER, this->mesh_uvs.size() * sizeof(glm::vec2), this->mesh_uvs.data(), GL_STATIC_DRAW);
    }

    if (!this->mesh_elements.empty()) {
        glGenBuffers(1, &this->ibo_mesh_elements);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_mesh_elements);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh_elements.size() * sizeof(GLushort), this->mesh_elements.data(), GL_STATIC_DRAW);
    }

    // Load shaders

    this->vs = create_shader(this->vertex_shader_filename, GL_VERTEX_SHADER);
    this->fs = create_shader(this->fragment_shader_filename, GL_FRAGMENT_SHADER);

    if (!this->vs) {
        std::cerr << "Failed to load vertex shader " << vertex_shader_filename << std::endl;
        return false;
    }

    if (!this->fs) {
        std::cerr << "Failed to load fragment shader " << fragment_shader_filename << std::endl;
        return false;
    }

    // Load textures TODO

    this->texture_id = load_texture("images/chinese_garden.png", "u_skybox");

    return true;
}

bool Mesh::bind_to_program(GLuint program) {
    // Load attributes

    const char *attribute_name;

    if (!this->mesh_vertices.empty()) {
        attribute_name = "coord_4d";
        this->attribute_coord_4d = glGetAttribLocation(program, attribute_name);
        if (this->attribute_coord_4d == -1) {
            std::cerr << "Could not bind attribute " << attribute_name << std::endl;
            return false;
        }
    }

    if (!this->mesh_normals.empty()) {
        attribute_name = "coord_norm";
        this->attribute_coord_norm = glGetAttribLocation(program, attribute_name);
        if (this->attribute_coord_norm == -1) {
            std::cerr << "Could not bind attribute " << attribute_name << std::endl;
            return false;
        }
    }

    if (!this->mesh_uvs.empty()) {
        attribute_name = "coord_uv";
        this->attribute_coord_uv = glGetAttribLocation(program, attribute_name);
        if (this->attribute_coord_uv == -1) {
            std::cerr << "Could not bind attribute " << attribute_name << std::endl;
            return false;
        }
    }

    // Load uniforms

    const char *uniform_name;

    uniform_name = "m";
    this->uniform_m = glGetUniformLocation(program, uniform_name);
    if (this->uniform_m == -1) {
        std::cerr << "Could not bind uniform " << uniform_name << std::endl;
        return false;
    }

    uniform_name = "v";
    this->uniform_v = glGetUniformLocation(program, uniform_name);
    if (this->uniform_v == -1) {
        std::cerr << "Could not bind uniform " << uniform_name << std::endl;
        return false;
    }

    uniform_name = "p";
    this->uniform_p = glGetUniformLocation(program, uniform_name);
    if (this->uniform_p == -1) {
        std::cerr << "Could not bind uniform " << uniform_name << std::endl;
        return false;
    }

    uniform_name = "m_3x3_inv_transp";
    this->uniform_m_3x3_inv_transp = glGetUniformLocation(program, uniform_name);
    if (this->uniform_m_3x3_inv_transp == -1) {
        std::cerr << "Could not bind uniform " << uniform_name << std::endl;
        return false;
    }

    uniform_name = "u_skybox";
    this->texture_u_skybox = glGetUniformLocation(program, uniform_name);
    if (this->texture_u_skybox == -1) {
        std::cerr << "Could not bind uniform texture " << uniform_name << std::endl;
        return false;
    }

    return true;
}

void Mesh::attach_shader(GLuint program) {
    glAttachShader(program, this->vs);
    glAttachShader(program, this->fs);
}

void Mesh::detach_shader(GLuint program) {
    glDetachShader(program, this->vs);
    glDetachShader(program, this->fs);
}

void Mesh::render(GLuint program) {
    if (!this->mesh_vertices.empty()) {
        glEnableVertexAttribArray(this->attribute_coord_4d);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_mesh_vertices);
        glVertexAttribPointer(
            this->attribute_coord_4d,
            4,
            GL_FLOAT,
            GL_FALSE,
            0,
            0
        );
    }

    if (!this->mesh_normals.empty()) {
        glEnableVertexAttribArray(this->attribute_coord_norm);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_mesh_normals);
        glVertexAttribPointer(
            this->attribute_coord_norm,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            0
        );
    }

    if (!this->mesh_uvs.empty()) {
        glEnableVertexAttribArray(this->attribute_coord_uv);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_mesh_uvs);
        glVertexAttribPointer(
            this->attribute_coord_uv,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            0
        );
    }

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(this->texture_u_skybox, /*GL_TEXTURE*/0);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);

    glDrawArrays(GL_TRIANGLES, 0, this->mesh_vertices.size());

    if (!this->mesh_elements.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_mesh_elements);
        int size;
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    }

    if (!this->mesh_vertices.empty())
        glDisableVertexAttribArray(this->attribute_coord_4d);

    if (!this->mesh_normals.empty())
        glDisableVertexAttribArray(this->attribute_coord_norm);

    if (!this->mesh_uvs.empty())
        glDisableVertexAttribArray(this->attribute_coord_uv);
}

void Mesh::set_rotation(glm::vec3 rotation, int about) {
    if (about == LOCAL) {
        this->local_rotation = rotation;
    } else if (about == GLOBAL) {
        this->global_rotation = rotation;
    } else {
        // TODO
    }
}

void Mesh::rotate(glm::vec3 rotation, int about) {
    if (about == LOCAL) {
        this->local_rotation += rotation;
    } else if (about == GLOBAL) {
        this->global_rotation += rotation;
    } else {
        // TODO
    }
}

void Mesh::set_translation(glm::vec3 translation) {
    this->translation = translation;
}

void Mesh::translate(glm::vec3 translation, int origin) {
    this->translation += translation;
}

void Mesh::update_uniforms(glm::mat4 projection, glm::mat4 view) {
    this->object_to_world = glm::mat4(1.0);

    // Global rotation
    this->object_to_world = glm::rotate(this->object_to_world, glm::radians(this->global_rotation.x), glm::vec3(1.0, 0.0, 0.0));
    this->object_to_world = glm::rotate(this->object_to_world, glm::radians(this->global_rotation.y), glm::vec3(0.0, 1.0, 0.0));
    this->object_to_world = glm::rotate(this->object_to_world, glm::radians(this->global_rotation.z), glm::vec3(0.0, 0.0, 1.0));

    // Translation
    this->object_to_world = glm::translate(this->object_to_world, this->translation);

    // Local rotation
    this->object_to_world = glm::rotate(this->object_to_world, glm::radians(this->local_rotation.x), glm::vec3(1.0, 0.0, 0.0));
    this->object_to_world = glm::rotate(this->object_to_world, glm::radians(this->local_rotation.y), glm::vec3(0.0, 1.0, 0.0));
    this->object_to_world = glm::rotate(this->object_to_world, glm::radians(this->local_rotation.z), glm::vec3(0.0, 0.0, 1.0));

    glm::mat3 m_3x3_inv_transp = glm::transpose(glm::inverse(glm::mat3(this->object_to_world)));

    glUniformMatrix4fv(this->uniform_m, 1, GL_FALSE, glm::value_ptr(this->object_to_world)); // object -> world
    glUniformMatrix4fv(this->uniform_v, 1, GL_FALSE, glm::value_ptr(view)); // world -> camera (constant between objects)
    glUniformMatrix4fv(this->uniform_p, 1, GL_FALSE, glm::value_ptr(projection)); // camera -> screen (constant between objects)
    glUniformMatrix3fv(this->uniform_m_3x3_inv_transp, 1, GL_FALSE, glm::value_ptr(m_3x3_inv_transp));
}

void Mesh::free_resources() {
    glDeleteBuffers(1, &this->vbo_mesh_vertices);
    glDeleteBuffers(1, &this->vbo_mesh_normals);
    glDeleteBuffers(1, &this->ibo_mesh_elements);
    glDeleteTextures(1, &this->texture_id);
}
