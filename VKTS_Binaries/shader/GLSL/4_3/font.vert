#version 430 core

layout (binding = 0, std140) uniform _u_bufferViewProjection {
        mat4 projectionMatrix;
        mat4 viewMatrix;
} u_bufferViewProjection;

layout (binding = 1, std140) uniform _u_bufferTransform {
        mat4 modelMatrix;
} u_bufferTransform;

layout (binding = 2, std140) uniform _u_bufferColor {
        vec4 color;
} u_bufferColor;

layout (location = 0) in vec4 a_vertex;
layout (location = 1) in vec2 a_texCoord;

out vec2 v_texCoord;
out vec4 v_color;

void main(void)
{
    v_texCoord = a_texCoord;
    v_color = u_bufferColor.color; 

    gl_Position = u_bufferViewProjection.projectionMatrix * u_bufferViewProjection.viewMatrix * u_bufferTransform.modelMatrix * a_vertex;
}
