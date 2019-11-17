#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out VertexData {
	vec2 uv;
} vert;


void main()
{
	vert.uv = uv;

	// Ignore z and w value because vertices are rendered as screen quad
    gl_Position = vec4(position.x, position.y, 0.0, 1.0); 

}  