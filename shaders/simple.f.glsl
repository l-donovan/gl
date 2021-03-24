uniform sampler2D u_skybox;

in vec2 v_uv;

out vec4 fragColor;

void main() {
    fragColor = texture(u_skybox, vec2(v_uv.x, 1.0 - v_uv.y));
}
