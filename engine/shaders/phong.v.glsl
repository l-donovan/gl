/**
 * From the OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
 * This file is in the public domain.
 * Contributors: Martin Kraus, Sylvain Beucler
 */
attribute vec4 coord_4d;
attribute vec3 coord_norm;
varying vec4 position;  // position of the vertex (and fragment) in world space
varying vec3 varyingNormalDirection;  // surface normal vector in world space
uniform mat4 m, v, p;
uniform mat3 m_3x3_inv_transp;

void main() {
    position = m * coord_4d;
    varyingNormalDirection = normalize(m_3x3_inv_transp * coord_norm);

    mat4 mvp = p * v * m;
    gl_Position = mvp * coord_4d;
}

