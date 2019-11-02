#version 430 core

in VertexData {
	vec3 uv;
} vert;


out vec4 color;

uniform samplerCube skybox;

void main()
{    
    color = texture(skybox, vert.uv);
}