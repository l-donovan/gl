uniform sampler2D mat_texture;
uniform vec3 mat_color;
uniform float mat_metallic;
uniform float mat_roughness;
uniform float mat_reflectance;
uniform float mat_clear_coat;
uniform float mat_clear_coat_roughness;

in vec2 v_uv;
in vec3 v_norm;
in vec3 v_refraction;
in vec3 v_reflection;
in float v_fresnel;

out vec4 frag_color;

void main(void) {
    vec2 uv_reflection = vec2(
        atan(-v_reflection.z, -v_reflection.x) * 0.15915494 + 0.5,
        v_reflection.y * 0.5 + 0.5
    );

    vec2 uv_refraction = vec2(
        atan(-v_reflection.z, -v_reflection.x) * 0.15915494 + 0.5,
        v_reflection.y * 0.5 + 0.5
    );

    vec4 refraction_color = texture(mat_texture, vec2(uv_refraction));
    vec4 reflection_color = texture(mat_texture, vec2(uv_reflection));

    vec4 base_color = mix(texture(mat_texture, v_uv), vec4(mat_color, 1.0), mat_roughness);
    base_color = vec4(base_color.rgb * pow(v_fresnel, 0.2), base_color.a);

    vec3 t0 = mat_color;
    float t1 = mat_metallic;
    float t2 = mat_roughness;
    float t3 = mat_reflectance;
    float t4 = mat_clear_coat;
    float t5 = mat_clear_coat_roughness;

    frag_color = mix(base_color, mix(reflection_color, refraction_color, v_fresnel), mat_metallic);
}
