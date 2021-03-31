in vec4 coord_4d;
in vec3 coord_norm;
in vec2 coord_uv;

out vec2 v_uv;
out vec3 v_n;
out vec3 v_v;
out vec3 v_l;

uniform mat4 m, v, p;
uniform mat3 m_3x3_inv_transp;

void main() {
    mat3 trash = m_3x3_inv_transp;

    v_uv = coord_uv;
    v_n = coord_norm;
    v_v = coord_norm;
    v_l = vec3(0.0, 1.0, 0.0);

    gl_Position = p * v * m * coord_4d;
}
