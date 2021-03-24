varying vec3 f_color;
varying vec2 f_coord_uv;
uniform float fade;

void main(void) {
    gl_FragColor = vec4(f_color.r, f_color.g, f_color.b, fade);
}
