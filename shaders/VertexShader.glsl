#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 object_color;
uniform vec3 car_position;

// Output value to fragment shader
out vec3 color;


void main()
{
    color = object_color;
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
    gl_Position.y = gl_Position.y - pow(length((car_position, 1.0) - gl_Position), 2) * 0.025f;
}
