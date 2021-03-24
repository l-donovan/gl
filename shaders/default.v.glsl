attribute vec3 coord_3d;
attribute vec2 coord_uv;
attribute vec3 v_color;
varying vec3 f_color;
varying vec2 f_coord_uv;
uniform mat4 mvp;

void main(void) {
    gl_Position = mvp * vec4(coord_3d, 1.0);
    f_color = v_color;
    f_coord_uv = coord_uv;
}
