varying vec3 f_color;
varying vec2 f_coord_uv;
uniform sampler2D texture;

void main(void) {
    vec2 flipped_coord_uv = vec2(f_coord_uv.x, 1.0 - f_coord_uv.y);
    gl_FragColor = texture2D(texture, flipped_coord_uv);
}
