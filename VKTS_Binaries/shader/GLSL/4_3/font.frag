#version 430 core

layout (binding = 4) uniform sampler2D u_texture; 

in vec2 v_texCoord;
in vec4 v_color;

layout (location = 0) out vec4 ob_fragColor;

void main(void)
{
	ob_fragColor = texture(u_texture, v_texCoord) * v_color;
}
