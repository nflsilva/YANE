#version 330 core

out vec4 output_color;
in vec2 texture_coordinates;

uniform sampler2D texture1;

void main(void)  
{    
	output_color = texture(texture1, texture_coordinates);
} 