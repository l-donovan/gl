attribute vec4 coord_4d;
attribute vec3 coord_norm;
uniform mat4 mvp;

void main(void) {
    gl_Position = mvp * coord_4d;
    vec3 n = coord_norm;
}
