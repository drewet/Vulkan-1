#version 430 core

layout(triangles) in;

layout(triangle_strip, max_vertices = 3) out;

layout (binding = 5) uniform sampler2D u_displacementTexture;

in vec3 v_g_eye[];
in vec3 v_g_normal[];
in vec3 v_g_bitangent[];
in vec3 v_g_tangent[];
in vec2 v_g_texCoord[];

out vec3 v_f_eye;
out vec3 v_f_normal;
out vec3 v_f_bitangent;
out vec3 v_f_tangent;
out vec2 v_f_texCoord;

void main(void)
{
    for(int i = 0; i < gl_in.length(); ++i)
    {
        v_f_eye = v_g_eye[i];
        v_f_normal = v_g_normal[i];
        v_f_bitangent = v_g_bitangent[i];
        v_f_tangent = v_g_tangent[i];
        v_f_texCoord = v_g_texCoord[i];
    
        gl_Position = gl_in[i].gl_Position;

        EmitVertex();
    }
    
    EndPrimitive();
}
