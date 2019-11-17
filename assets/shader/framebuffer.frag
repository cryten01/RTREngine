#version 430 core

in VertexData {
	vec2 uv;
} vert;

out vec4 color;

uniform sampler2D screenTexture;

void main()
{ 
    color = texture(screenTexture, vert.uv);
}