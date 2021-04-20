#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "lgll/geometry.h"
#include "lgll/image.h"
#include "lgll/entity.h"
#include "lgll/shader.h"

using std::vector;

Entity::Entity(std::string obj_filename, std::string vertex_shader_filename, std::string fragment_shader_filename) {
    this->vertex_shader_filename = vertex_shader_filename;
    this->fragment_shader_filename = fragment_shader_filename;
    this->object_to_world = glm::mat4(1.0);
    this->obj_filename = obj_filename;
    this->obj_scale = glm::vec3(1.0);
}

bool Entity::load_into_program(GLuint program) {
    // Load data into buffers

    if (!this->mesh->vertices.empty()) {
        glGenBuffers(1, &this->vbo_mesh_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_mesh_vertices);
        glBufferData(GL_ARRAY_BUFFER, this->mesh->vertices.size() * sizeof(glm::vec4), this->mesh->vertices.data(), GL_STATIC_DRAW);
    }

    if (!this->mesh->normals.empty()) {
        glGenBuffers(1, &this->vbo_mesh_normals);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_mesh_normals);
        glBufferData(GL_ARRAY_BUFFER, this->mesh->normals.size() * sizeof(glm::vec3), this->mesh->normals.data(), GL_STATIC_DRAW);
    }

    if (!this->mesh->uvs.empty()) {
        glGenBuffers(1, &this->vbo_mesh_uvs);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_mesh_uvs);
        glBufferData(GL_ARRAY_BUFFER, this->mesh->uvs.size() * sizeof(glm::vec2), this->mesh->uvs.data(), GL_STATIC_DRAW);
    }

    if (!this->mesh->faces.empty()) {
        glGenBuffers(1, &this->ibo_mesh_faces);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_mesh_faces);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh->faces.size() * sizeof(GLushort), this->mesh->faces.data(), GL_STATIC_DRAW);
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

    if (!this->texture_filename.empty()) {
        this->texture_id = load_texture(this->texture_filename.c_str(), "mat_texture");
    }

    return true;
}

void Entity::set_texture(std::string texture_filename) {
    // Load multiple textures TODO

    this->texture_filename = texture_filename;
}

bool Entity::bind_to_program(GLuint program) {
    // Load attributes

    const char *attribute_name;

    if (!this->mesh->vertices.empty()) {
        attribute_name = "coord_4d";
        this->attribute_coord_4d = glGetAttribLocation(program, attribute_name);
        if (this->attribute_coord_4d == -1) {
            std::cerr << "Could not bind attribute " << attribute_name << std::endl;
            return false;
        }
    }

    if (!this->mesh->normals.empty()) {
        attribute_name = "coord_norm";
        this->attribute_coord_norm = glGetAttribLocation(program, attribute_name);
        if (this->attribute_coord_norm == -1) {
            std::cerr << "Could not bind attribute " << attribute_name << std::endl;
            return false;
        }
    }

    if (!this->mesh->uvs.empty()) {
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

    uniform_name = "camera";
    this->uniform_camera = glGetUniformLocation(program, uniform_name);
    if (this->uniform_camera == -1) {
        std::cerr << "Could not bind uniform " << uniform_name << std::endl;
        return false;
    }

    // Load material uniforms

    uniform_name = "mat_texture";
    this->uniform_mat_texture = glGetUniformLocation(program, uniform_name);
    if (this->uniform_mat_texture == -1) {
        std::cerr << "Could not bind uniform texture " << uniform_name << std::endl;
        return false;
    }

    uniform_name = "mat_color";
    this->uniform_mat_color = glGetUniformLocation(program, uniform_name);
    if (this->uniform_mat_color == -1) {
        std::cerr << "Could not bind material uniform " << uniform_name << std::endl;
        return false;
    }

    uniform_name = "mat_metallic";
    this->uniform_mat_metallic = glGetUniformLocation(program, uniform_name);
    if (this->uniform_mat_metallic == -1) {
        std::cerr << "Could not bind material uniform " << uniform_name << std::endl;
        return false;
    }

    uniform_name = "mat_roughness";
    this->uniform_mat_roughness = glGetUniformLocation(program, uniform_name);
    if (this->uniform_mat_roughness == -1) {
        std::cerr << "Could not bind material uniform " << uniform_name << std::endl;
        return false;
    }

    uniform_name = "mat_reflectance";
    this->uniform_mat_reflectance = glGetUniformLocation(program, uniform_name);
    if (this->uniform_mat_reflectance == -1) {
        std::cerr << "Could not bind material uniform " << uniform_name << std::endl;
        return false;
    }

    uniform_name = "mat_clear_coat";
    this->uniform_mat_clear_coat = glGetUniformLocation(program, uniform_name);
    if (this->uniform_mat_clear_coat == -1) {
        std::cerr << "Could not bind material uniform " << uniform_name << std::endl;
        return false;
    }

    uniform_name = "mat_clear_coat_roughness";
    this->uniform_mat_clear_coat_roughness = glGetUniformLocation(program, uniform_name);
    if (this->uniform_mat_clear_coat_roughness == -1) {
        std::cerr << "Could not bind material uniform " << uniform_name << std::endl;
        return false;
    }

    return true;
}

void Entity::attach_shader(GLuint program) {
    glAttachShader(program, this->vs);
    glAttachShader(program, this->fs);
}

void Entity::detach_shader(GLuint program) {
    glDetachShader(program, this->vs);
    glDetachShader(program, this->fs);
}

void Entity::render(GLuint program) {
    if (!this->mesh->vertices.empty()) {
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

    if (!this->mesh->normals.empty()) {
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

    if (!this->mesh->uvs.empty()) {
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
    glUniform1i(this->uniform_mat_texture, /*GL_TEXTURE*/0);
    glBindTexture(GL_TEXTURE_2D, this->texture_id);

    glDrawArrays(GL_TRIANGLES, 0, this->mesh->vertices.size());

    if (!this->mesh->faces.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_mesh_faces);
        int size;
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    }

    if (!this->mesh->vertices.empty())
        glDisableVertexAttribArray(this->attribute_coord_4d);

    if (!this->mesh->normals.empty())
        glDisableVertexAttribArray(this->attribute_coord_norm);

    if (!this->mesh->uvs.empty())
        glDisableVertexAttribArray(this->attribute_coord_uv);
}

void Entity::set_material_attributes(attributes *material_attributes) {
    this->material_attributes = material_attributes;
}

void Entity::set_rotation(glm::vec3 rotation, int about) {
    if (about == LOCAL) {
        this->obj_local_rotation = rotation;
    } else if (about == GLOBAL) {
        this->obj_global_rotation = rotation;
    } else {
        // TODO
    }
}

void Entity::rotate(glm::vec3 rotation, int about) {
    if (about == LOCAL) {
        this->obj_local_rotation += rotation;
    } else if (about == GLOBAL) {
        this->obj_global_rotation += rotation;
    } else {
        // TODO
    }
}

void Entity::set_translation(glm::vec3 translation) {
    this->obj_translation = translation;
}

void Entity::translate(glm::vec3 translation, int origin) {
    this->obj_translation += translation;
}

void Entity::set_scale(glm::vec3 scale) {
    this->obj_scale = scale;
}

void Entity::scale(glm::vec3 scale) {
    this->obj_scale *= scale;
}

void Entity::update_uniforms(glm::mat4 projection, glm::mat4 view, glm::vec3 camera_position) {
    this->object_to_world = glm::mat4(1.0);

    // Global rotation
    this->object_to_world = glm::rotate(this->object_to_world, glm::radians(this->obj_global_rotation.x), glm::vec3(1.0, 0.0, 0.0));
    this->object_to_world = glm::rotate(this->object_to_world, glm::radians(this->obj_global_rotation.y), glm::vec3(0.0, 1.0, 0.0));
    this->object_to_world = glm::rotate(this->object_to_world, glm::radians(this->obj_global_rotation.z), glm::vec3(0.0, 0.0, 1.0));

    // Translation
    this->object_to_world = glm::translate(this->object_to_world, this->obj_translation);

    // Local rotation
    this->object_to_world = glm::rotate(this->object_to_world, glm::radians(this->obj_local_rotation.x), glm::vec3(1.0, 0.0, 0.0));
    this->object_to_world = glm::rotate(this->object_to_world, glm::radians(this->obj_local_rotation.y), glm::vec3(0.0, 1.0, 0.0));
    this->object_to_world = glm::rotate(this->object_to_world, glm::radians(this->obj_local_rotation.z), glm::vec3(0.0, 0.0, 1.0));

    // Scaling
    this->object_to_world = glm::scale(this->object_to_world, this->obj_scale);

    glm::mat3 m_3x3_inv_transp = glm::transpose(glm::inverse(glm::mat3(this->object_to_world)));

    glUniformMatrix4fv(this->uniform_m, 1, GL_FALSE, glm::value_ptr(this->object_to_world)); // object -> world
    glUniformMatrix4fv(this->uniform_v, 1, GL_FALSE, glm::value_ptr(view)); // world -> camera (constant between objects)
    glUniformMatrix4fv(this->uniform_p, 1, GL_FALSE, glm::value_ptr(projection)); // camera -> screen (constant between objects)
    glUniformMatrix3fv(this->uniform_m_3x3_inv_transp, 1, GL_FALSE, glm::value_ptr(m_3x3_inv_transp));
    glUniform3fv(this->uniform_camera, 1, glm::value_ptr(camera_position));

    // Material uniforms
    glUniform3fv(this->uniform_mat_color, 1, glm::value_ptr(this->material_attributes->color));
    glUniform1f(this->uniform_mat_metallic, this->material_attributes->metallic);
    glUniform1f(this->uniform_mat_roughness, this->material_attributes->roughness);
    glUniform1f(this->uniform_mat_reflectance, this->material_attributes->reflectance);
    glUniform1f(this->uniform_mat_clear_coat, this->material_attributes->clear_coat);
    glUniform1f(this->uniform_mat_clear_coat_roughness, this->material_attributes->clear_coat_roughness);
}

void Entity::free_resources() {
    glDeleteBuffers(1, &this->vbo_mesh_vertices);
    glDeleteBuffers(1, &this->vbo_mesh_normals);
    glDeleteBuffers(1, &this->ibo_mesh_faces);
    glDeleteTextures(1, &this->texture_id);
}
