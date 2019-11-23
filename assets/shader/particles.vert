#version 430

/*
* This shader is responisble for applying the modelViewMatrix to each particle position
*/

layout (location = 0) in vec4 position; // positions from the computeShader


out VertexData {
	float TTL1;	 
} fromVert;


uniform mat4 modelViewMatrix;
uniform mat4 viewProjMatrix;


void main()
{	
	// extracts TTL from vec4
	fromVert.TTL1 = position.w;							

	// gl_Position contains position of current vertex
	gl_Position = modelViewMatrix * position;		
}