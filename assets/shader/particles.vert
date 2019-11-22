#version 430


layout (location = 0) in vec4 position; // positions from the computeShader

out VertexData {
	float TTL1;	 // additional vertexData (color could also be here)
} fromVert;

uniform mat4 modelViewMatrix;	// the camera's ViewMatrix


void main()
{
	gl_Position = modelViewMatrix * position;		// gl_Position contains position of current vertex
	fromVert.TTL1 = position.w;						// extracts TTL from vec4
}