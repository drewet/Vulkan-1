#version 450 core

layout(vertices = 3) out;

in vec3 v_c_eye[];
in vec3 v_c_normal[];
in vec3 v_c_bitangent[];
in vec3 v_c_tangent[];
in vec2 v_c_texCoord[];

out vec3 v_e_eye[];
out vec3 v_e_normal[];
out vec3 v_e_bitangent[];
out vec3 v_e_tangent[];
out vec2 v_e_texCoord[];

void main(void)
{
    v_e_eye[gl_InvocationID] = v_c_eye[gl_InvocationID];
    v_e_normal[gl_InvocationID] = v_c_normal[gl_InvocationID];
    v_e_bitangent[gl_InvocationID] = v_c_bitangent[gl_InvocationID];
    v_e_tangent[gl_InvocationID] = v_c_tangent[gl_InvocationID];
    v_e_texCoord[gl_InvocationID] = v_c_texCoord[gl_InvocationID];

	gl_TessLevelOuter[0] = 4.0;
	gl_TessLevelOuter[1] = 4.0;
	gl_TessLevelOuter[2] = 4.0;

	gl_TessLevelInner[0] = 4.0;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
