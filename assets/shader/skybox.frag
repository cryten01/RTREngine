#version 430 core

in VertexData {
	vec3 uv;
} vert;

out vec4 color;

uniform samplerCube skybox;
uniform vec3 testColor;

void main()
{    
    color = texture(skybox, vert.uv);
}