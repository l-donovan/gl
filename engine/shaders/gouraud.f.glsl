in vec4 frontColor; // color for front face
in vec4 backColor; // color for back face

layout(location = 0) out vec4 fragColor;

void main(void) {
    if (gl_FrontFacing) { // is the fragment part of a front face?
        fragColor = frontColor;
    } else { // fragment is part of a back face
        fragColor = backColor;
    }
}

