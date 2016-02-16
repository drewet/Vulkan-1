#version 430 core

layout (location = 0) in vec4 a_vertex;
layout (location = 1) in vec2 a_texCoord;

out vec2 v_texCoord;

void main(void)
{
	v_texCoord = a_texCoord;

	gl_Position = a_vertex;
}