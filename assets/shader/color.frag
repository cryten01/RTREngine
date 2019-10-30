#version 430 core
/*
* Copyright 2010 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/

in VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 uv;
} vert;

uniform vec3 diffuseColor;
uniform sampler2D diffuseTexture;

out vec4 color;

void main() {	
	color = vec4(diffuseColor, 1);
//	vec3 texColor = texture(diffuseTexture, vert.uv).rgb;
//	color = vec4(texColor, 1); 
}