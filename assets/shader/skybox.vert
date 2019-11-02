#version 430 core

layout(location = 0) in vec3 position;

out VertexData {
	vec3 uv;
} vert;

uniform mat4 viewProjMatrix;

void main()
{
	// Set position coordinates as uv coordinates 
	// Because each position vector is also direction vector from the origin
    vert.uv = position;

	// Calculate vertex position based on vp matrix
    gl_Position = viewProjMatrix * vec4(position, 1);
} 