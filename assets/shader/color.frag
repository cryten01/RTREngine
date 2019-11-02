#version 430 core

/** INPUTS */
in VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 uv;
} vert;

/** UNIFORMS */
uniform vec3 camera_world;
uniform samplerCube skybox;

uniform struct ControlParams {
	bool reflection;
} param;

uniform struct Material {
	sampler2D texture_diffuse1;
} material;

uniform vec3 diffuseColor;

/** OUTPUTS */
out vec4 color;


void main() {	
	color = vec4(diffuseColor, 1);

//	vec3 texColor = texture(diffuseTexture, vert.uv).rgb;
//	color = vec4(texColor, 1); 

	if (param.reflection) 
	{
		vec3 I = normalize(vert.position_world - camera_world);
		vec3 R = reflect(I, normalize(vert.normal_world));
		color = vec4(texture(skybox, R).rgb, 1.0);
	} 
	else 
	{
		color = texture2D(material.texture_diffuse1, vert.uv);
	}
}