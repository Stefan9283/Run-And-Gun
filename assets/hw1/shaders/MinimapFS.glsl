#version 330

// Uniform properties
uniform vec3 color;
uniform float layer;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    // out_color = vec4(vec3(dot(color, vec3(0.2126, 0.7152, 0.0722))), 1);
    out_color = vec4(vec3(dot(color, vec3(1))) / 3, 1);
}
