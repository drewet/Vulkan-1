#version 450 core

layout (binding = 2, std140) uniform _u_bufferFrag {
	vec3 light;
} u_bufferFrag;

layout (binding = 3) uniform sampler2D u_emissiveTexture;
layout (binding = 4) uniform sampler2D u_alphaTexture;
layout (binding = 5) uniform sampler2D u_displacementTexture;
layout (binding = 6) uniform sampler2D u_normalTexture;
layout (binding = 7) uniform sampler2D u_ambientTexture;
layout (binding = 8) uniform sampler2D u_diffuseTexture;
layout (binding = 9) uniform sampler2D u_specularTexture;
layout (binding = 10) uniform sampler2D u_specularShininessTexture;

in vec3 v_eye;
in vec3 v_normal;
in vec3 v_bitangent;
in vec3 v_tangent;
in vec2 v_texCoord;

layout (location = 0) out vec4 ob_fragColor;

void main(void)
{
	// Ambient and emissive color.
	vec4 color = texture(u_ambientTexture, v_texCoord) + texture(u_emissiveTexture, v_texCoord);

    mat3 objectToWorldMatrix = mat3(normalize(v_tangent), normalize(v_bitangent), normalize(v_normal));
	
    vec3 normal = objectToWorldMatrix * normalize((texture(u_normalTexture, v_texCoord) * 2.0 - 1.0).xyz);
	
	float nDotL = max(dot(u_bufferFrag.light, normal), 0.0);
	
	if (nDotL > 0.0)
	{
		// Diffuse color.
		color += texture(u_diffuseTexture, v_texCoord) * nDotL;
		
		
		vec3 eye = normalize(v_eye);
	
		// Incident vector is opposite light direction vector.
		vec3 reflection = reflect(-u_bufferFrag.light, normal);
		
		float eDotR = max(dot(eye, reflection), 0.0);
		
		if (eDotR > 0.0)
		{
			// Shininess is stored as 1.0/128.0.
			float shininess = texture(u_specularShininessTexture, v_texCoord).r * 128.0;
		
			// Specular color.
			color += texture(u_specularTexture, v_texCoord) * pow(eDotR, shininess);
		}
	}
	
	ob_fragColor = color; 
}
