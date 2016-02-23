#version 450 core

layout (location = 0) in vec4 a_vertex;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec3 a_bitangent;
layout (location = 3) in vec3 a_tangent;
layout (location = 4) in vec2 a_texCoord;

out vec3 v_c_eye;
out vec3 v_c_normal;
out vec3 v_c_bitangent;
out vec3 v_c_tangent;
out vec2 v_c_texCoord;

void main(void)
{
    vec4 vertex = a_vertex;

    v_c_eye = -vec3(vertex);

    v_c_normal = a_normal;
    v_c_bitangent = a_bitangent;
    v_c_tangent = a_tangent;

    v_c_texCoord = a_texCoord;

    gl_Position = vertex;
}
