#define PI 3.1415926538

uniform sampler2D u_texture;

in vec2 v_uv;
in vec3 v_n;
in vec3 v_v;
in vec3 v_l;

out vec4 fragColor;

float perceptualRoughness = 0.1;
float reflectance = 0.5;
float metallic = 0.5;
vec3 baseColor = vec3(0.5, 1.0, 0.5);
float clearCoatPerceptualRoughness = 0.1;
float clearCoat = 0.1;

float D_GGX(float NoH, float a) {
    float a2 = a * a;
    float f = (NoH * a2 - NoH) * NoH + 1.0;
    return a2 / (PI * f * f);
}

vec3 F_Schlick(float u, vec3 f0) {
    return f0 + (vec3(1.0) - f0) * pow(1.0 - u, 5.0);
}

float V_SmithGGXCorrelated(float NoV, float NoL, float a) {
    float a2 = a * a;
    float GGXL = NoV * sqrt((-NoL * a2 + NoL) * NoL + a2);
    float GGXV = NoL * sqrt((-NoV * a2 + NoV) * NoV + a2);
    return 0.5 / (GGXV + GGXL);
}

float V_Kelemen(float LoH) {
    return 0.25 / (LoH * LoH);
}

float Fd_Lambert() {
    return 1.0 / PI;
}

void main() {
    // v_v - View unit vector
    // v_l - Incident light unit vector
    // v_n - Surface normal unit vector (DONE)
    // v_h - Half unit vector between l and v
    // perceptualRoughness - (parameter)
    // reflectance - (parameter)
    // metallic - (parameter)
    // baseColor - (parameter)



    vec3 diffuseColor = (1.0 - metallic) * baseColor;
    vec3 v_h = normalize(v_v + v_l);

    float NoV = abs(dot(v_n, v_v)) + 1e-5;
    float NoL = clamp(dot(v_n, v_l), 0.0, 1.0);
    float NoH = clamp(dot(v_n, v_h), 0.0, 1.0);
    float LoH = clamp(dot(v_l, v_h), 0.0, 1.0);

    // perceptually linear roughness to roughness (see parameterization)
    float roughness = perceptualRoughness * perceptualRoughness;

    float a = NoH * roughness; // Maybe
    vec3 f0 = 0.16 * reflectance * reflectance * (1.0 - metallic) + baseColor * metallic;

    float D = D_GGX(NoH, a);
    vec3  F = F_Schlick(LoH, f0);
    float V = V_SmithGGXCorrelated(NoV, NoL, roughness);

    // specular BRDF
    vec3 Fr = (D * V) * F;

    // diffuse BRDF
    vec3 Fd = diffuseColor * Fd_Lambert();

    clearCoatPerceptualRoughness = clamp(clearCoatPerceptualRoughness, 0.089, 1.0);
    float clearCoatRoughness = clearCoatPerceptualRoughness * clearCoatPerceptualRoughness;
    float  Dc = D_GGX(clearCoatRoughness, NoH);
    float  Vc = V_Kelemen(clearCoatRoughness);
    float  Fc = (F_Schlick(0.04, vec3(LoH)) * clearCoat).x; // clear coat strength
    float Frc = (Dc * Vc) * Fc;

    fragColor = vec4(baseColor * ((Fd + Fr * (1.0 - Fc)) * (1.0 - Fc) + Frc), 1.0);

    // apply lighting...

    vec4 trash = texture(u_texture, v_uv);
}
