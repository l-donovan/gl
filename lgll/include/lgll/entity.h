#pragma once

#include <glm/glm.hpp>

#include "shader.h"

#include <string>
#include <vector>

#define LOCAL  0
#define GLOBAL 1

using std::vector;

class Entity {
    private:
        GLuint vbo_mesh_vertices, vbo_mesh_normals, vbo_mesh_uvs, ibo_mesh_faces;
        GLint attribute_coord_4d, attribute_coord_norm, attribute_coord_uv;
        GLint uniform_m, uniform_v, uniform_p, uniform_m_3x3_inv_transp, uniform_camera;
        GLint uniform_mat_color, uniform_mat_metallic, uniform_mat_roughness, uniform_mat_reflectance, uniform_mat_clear_coat, uniform_mat_clear_coat_roughness, uniform_mat_texture;
        GLuint vs, fs;
        std::string vertex_shader_filename, fragment_shader_filename;
        glm::mat4 object_to_world;
        glm::vec3 obj_local_rotation, obj_global_rotation, obj_translation, obj_scale;
        GLuint texture_id;
        struct attributes *material_attributes;
    public:
        Entity(std::string obj_filename, std::string vertex_shader_filename, std::string fragment_shader_filename);
        std::string obj_filename;
        std::string texture_filename;
        struct mesh_t *mesh;
        bool load_into_program(GLuint program);
        bool bind_to_program(GLuint program);
        void render(GLuint program);
        void free_resources();
        void update_uniforms(glm::mat4 projection, glm::mat4 view, glm::vec3 camera_position);
        void set_rotation(glm::vec3 rotation, int about=GLOBAL);
        void rotate(glm::vec3 rotation, int about=GLOBAL);
        void set_translation(glm::vec3 translation);
        void translate(glm::vec3 translation, int origin=LOCAL);
        void set_scale(glm::vec3 scale);
        void scale(glm::vec3 scale);
        void attach_shader(GLuint program);
        void detach_shader(GLuint program);
        void set_texture(std::string texture_filename);
        void set_material_attributes(attributes *material_attributes);
};
