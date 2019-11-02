#version 430 core


in VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 uv;
} vert;

uniform vec3 diffuseColor;

uniform struct Material {
	sampler2D texture_diffuse1;
} material;


out vec4 color;

void main() {	
	color = vec4(diffuseColor, 1);

//	vec3 texColor = texture(diffuseTexture, vert.uv).rgb;
//	color = vec4(texColor, 1); 

	color = texture2D(material.texture_diffuse1, vert.uv);
}