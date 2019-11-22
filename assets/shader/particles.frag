#version 430 core

/*
* This shader is responisble the color and texturing of the particles
*/

in VertexData {
	vec2 uv;			// texture coordinates from the quad
	flat float TTL; 	// the uninterpolated TTL
} fromGeom;


out vec4 fragColor;		


void main() {
	vec3 color = vec3(0.0, 1.0, 0.0);
	float alpha = 0.2;
	fragColor = vec4(color, 1.0);
}