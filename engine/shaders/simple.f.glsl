uniform sampler2D u_texture;

in vec2 v_uv;

out vec4 fragColor;

void main() {
    fragColor = texture(u_texture, vec2(v_uv.x, 1.0 - v_uv.y));
}
