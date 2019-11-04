#version 430 core


layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 uv;
} vert;

uniform mat4 modelMatrix;
uniform mat4 viewProjMatrix;
uniform mat3 normalMatrix;

void main() {
	
	// Set position vector
	vec4 position_world = modelMatrix * vec4(position, 1.0);
	vert.position_world = position_world.xyz;
	
	// Set normal vector
	vert.normal_world = normalMatrix * normal;

	// Set uv coordinates
	vert.uv = uv;

	// Calculate vertex position based on mvp matrix
	gl_Position = viewProjMatrix * position_world;
}