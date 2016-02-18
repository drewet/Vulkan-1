#version 430 core

layout (location = 0) in vec4 a_vertex;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec3 a_bitangent;
layout (location = 3) in vec3 a_tangent;
layout (location = 4) in vec2 a_texCoord;

out vec3 v_g_eye;
out vec3 v_g_normal;
out vec3 v_g_bitangent;
out vec3 v_g_tangent;
out vec2 v_g_texCoord;

void main(void)
{
    vec4 vertex = a_vertex;

    v_g_eye = -vec3(vertex);

    v_g_normal = a_normal;
    v_g_bitangent = a_bitangent;
    v_g_tangent = a_tangent;

    v_g_texCoord = a_texCoord;

    gl_Position = vertex;
}
