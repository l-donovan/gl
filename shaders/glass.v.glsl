// Indices of refraction
const float Air = 1.0;
const float Glass = 1.51714;

// Air to glass ratio of the indices of refraction (Eta)
const float Eta = Air / Glass;

// see http://en.wikipedia.org/wiki/Refractive_index Reflectivity
const float R0 = ((Air - Glass) * (Air - Glass)) / ((Air + Glass) * (Air + Glass));

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform mat3 m_3x3_inv_transp;

in vec4 coord_4d;
in vec3 coord_norm;
in vec2 coord_uv;

out vec3 v_reflection;
out vec3 v_refraction;
out float v_fresnel;

void main(void) {
    vec2 trash = coord_uv;
    mat3 more_trash = m_3x3_inv_transp;

    // We calculate in world space.

    vec4 vertex = m * coord_4d;

    // TODO
    vec4 u_camera = vec4(0.0, 2.0, 4.0, 1.0);
    mat4 u_normalMatrix = transpose(inverse(v * m));

    vec3 incident = normalize(vec3(vertex - u_camera));

    // Assume incoming normal is normalized.
    vec3 normal = vec3(u_normalMatrix * vec4(coord_norm, 1.0));

    v_refraction = refract(incident, normal, Eta);
    v_reflection = reflect(incident, normal);

    // see http://en.wikipedia.org/wiki/Schlick%27s_approximation
    v_fresnel = R0 + (1.0 - R0) * pow((1.0 - dot(-incident, normal)), 5.0);

    gl_Position = p * v * vertex;
}
