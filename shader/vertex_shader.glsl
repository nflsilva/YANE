#version 330 core

in layout(location=0) vec2 input_position;
in layout(location=1) vec2 input_texture_coordinates;

out vec2 texture_coordinates;


void main(void)
{
	texture_coordinates = input_texture_coordinates;
	gl_Position = vec4(input_position, 0.0, 1.0);
}       