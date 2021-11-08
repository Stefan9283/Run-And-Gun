#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float scale;
uniform vec2 offset;

// Output
out vec3 frag_normal;
out vec2 tex_coord;

void main()
{
    frag_normal = v_normal;
    tex_coord = v_texture_coord;
    gl_Position = Projection * View * Model * vec4(v_position * scale + vec3(offset, 0), 1.0);
}
