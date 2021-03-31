uniform sampler2D u_texture;

in vec3 v_refraction;
in vec3 v_reflection;
in float v_fresnel;

out vec4 fragColor;

void main(void) {
    vec4 refractionColor = texture(u_texture, vec2(v_refraction));
    vec4 reflectionColor = texture(u_texture, vec2(v_reflection));

    fragColor = mix(refractionColor, reflectionColor, v_fresnel);
}
