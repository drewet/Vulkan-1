#version 430 core

#define VKTS_DISPLACE_FACTOR 0.5

layout(triangles) in;

layout(triangle_strip, max_vertices = 3) out;

layout (binding = 0, std140) uniform _u_bufferViewProjection {
        mat4 projectionMatrix;
        mat4 viewMatrix;
} u_bufferViewProjection;

layout (binding = 1, std140) uniform _u_bufferTransform {
        mat4 modelMatrix;
        mat3 normalMatrix;
} u_bufferTransform;

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
        float displace = texture(u_displacementTexture, v_g_texCoord[i]).r * VKTS_DISPLACE_FACTOR;
        
        vec4 displaceVector = vec4(0.0, displace, 0.0, 0.0);
    
        //
    
        vec4 vertex = u_bufferViewProjection.viewMatrix * u_bufferTransform.modelMatrix * (gl_in[i].gl_Position + displaceVector);
    
        // TODO: Recalculate tangents depending on displace.
    
        v_f_eye = -vec3(vertex);
        v_f_normal = mat3(u_bufferViewProjection.viewMatrix) * u_bufferTransform.normalMatrix * v_g_normal[i];
        v_f_bitangent = mat3(u_bufferViewProjection.viewMatrix) * u_bufferTransform.normalMatrix * v_g_bitangent[i];
        v_f_tangent = mat3(u_bufferViewProjection.viewMatrix) * u_bufferTransform.normalMatrix * v_g_tangent[i];
        v_f_texCoord = v_g_texCoord[i];
    
        gl_Position = u_bufferViewProjection.projectionMatrix * vertex;

        EmitVertex();
    }
    
    EndPrimitive();
}
