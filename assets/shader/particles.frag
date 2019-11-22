#version 430 core

in VertexData {
	vec2 uv;			// texture coordinates from the quad
	flat float TTL; 	// the uninterpolated TTL
} fromGeom;


out vec4 fragColor;		


void main() {
	vec3 color = vec3(1.0, 1.0, 0.0);
	float alpha = 0.2;
	fragColor = vec4(color, alpha);
}