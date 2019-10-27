#version 430 core
/*
* Copyright 2010 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/

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
uniform vec3 camera_world;

void main() {
	
	// Set uv coordinates
	vert.uv = uv;

	// Calculate vertex position based on mvp matrix
	gl_Position = viewProjMatrix * modelMatrix * vec4(position, 1);
}