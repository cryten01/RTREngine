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
	int type; // 0 reflective, 1 refractive, 2 matte, 3 diffuse
} param;

uniform struct Material {
	sampler2D texture_diffuse1;
} material;

uniform vec3 diffuseColor;

/** OUTPUTS */
out vec4 color;


void main() {	

	vec3 I,R;

	if (param.type == 0) 
	{
		vec3 I = normalize(vert.position_world - camera_world);
		vec3 R = reflect(I, normalize(vert.normal_world));
		color = vec4(texture(skybox, R).rgb, 1.0);
	} 
	
	if (param.type == 1)
	{
		float ratio = 1.00 / 1.52;
		vec3 I = normalize(vert.position_world - camera_world);
		vec3 R = refract(I, normalize(vert.normal_world), ratio);
		color = vec4(texture(skybox, R).rgb, 1.0);
	} 
	
	if (param.type == 2)
	{
		color = texture2D(material.texture_diffuse1, vert.uv);
	}

	if (param.type == 3)
	{
		color = vec4(diffuseColor, 1);
	}
}