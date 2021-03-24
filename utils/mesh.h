#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

#define LOCAL  0
#define GLOBAL 1

using std::vector;

class Mesh {
    private:
        GLuint vbo_mesh_vertices, vbo_mesh_normals, vbo_mesh_uvs, ibo_mesh_elements;
        GLint attribute_coord_4d, attribute_coord_norm, attribute_coord_uv;
        GLint uniform_m, uniform_v, uniform_p, uniform_m_3x3_inv_transp, texture_u_skybox;
        vector<glm::vec4> mesh_vertices;
        vector<glm::vec2> mesh_uvs;
        vector<glm::vec3> mesh_normals;
        vector<GLushort> mesh_elements;
        GLuint vs, fs;
        const char *vertex_shader_filename, *fragment_shader_filename;
        glm::mat4 object_to_world;
        glm::vec3 local_rotation, global_rotation, translation;
        GLuint texture_id;
    public:
        Mesh(const char *obj_filename, const char *vertex_shader_filename, const char *fragment_shader_filename);
        Mesh(vector<glm::vec4> vertices, vector<glm::vec2> uvs, vector<glm::vec3> normals, vector<GLushort> elements, const char *vertex_shader_filename, const char *fragment_shader_filename);
        std::string obj_filename;
        bool load_into_program(GLuint program);
        bool bind_to_program(GLuint program);
        void render(GLuint program);
        void free_resources();
        void update_uniforms(glm::mat4 projection, glm::mat4 view);
        void set_rotation(glm::vec3 rotation, int about=GLOBAL);
        void rotate(glm::vec3 rotation, int about=GLOBAL);
        void set_translation(glm::vec3 translation);
        void translate(glm::vec3 translation, int origin=LOCAL);
        void attach_shader(GLuint program);
        void detach_shader(GLuint program);
};
