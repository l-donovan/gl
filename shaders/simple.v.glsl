in vec4 coord_4d;
in vec3 coord_norm;
in vec2 coord_uv;

out vec2 v_uv;

uniform mat4 m, v, p;
uniform mat3 m_3x3_inv_transp;

void main() {
    vec3 trash1 = coord_norm;
    mat3 trash2 = m_3x3_inv_transp;

    v_uv = coord_uv;
    gl_Position = p * v * m * coord_4d;
}
