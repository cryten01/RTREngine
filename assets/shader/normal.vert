#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out VertexData {
    vec3 normal_world;
} vert;

uniform mat4 modelMatrix;
uniform mat4 viewProjMatrix;
uniform mat3 normalMatrix;

void main() {

	// Calculate position vector
	vec4 pos_world = modelMatrix * vec4(position, 1.0);

	// Set normal vector
	vert.normal_world = normalMatrix * normal;

	// Calculate vertex position based on mvp matrix
	gl_Position = viewProjMatrix * pos_world;
}