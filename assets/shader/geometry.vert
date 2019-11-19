#version 430 core

layout (location = 0) in vec2 position;
layout (location = 2) in vec3 color;


out VertexData {
    vec3 color;
} vert;


void main() {
    gl_Position = vec4(position.x, position.y, 0.0, 1.0); 

	vert.color = color; // Note that the color here gets set per vertex (not per fragment)
}