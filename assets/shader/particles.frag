#version 430 core

/*
* This shader is responisble the color and texturing of the particles
*/

in VertexData {
	vec2 uv;			// texture coordinates from the quad
	flat float TTL; 	// the uninterpolated TTL
} fromGeom;


uniform struct Material {
	sampler2D texture_diffuse1;		// don't change naming. requires consinstent naming scheme for loading
	vec3 light;						// x = ambient, y = diffuse, z = specular 
	vec3 color;
	float alpha;
} material;


out vec4 fragColor;		


void main() {
	vec4 uvColor = texture(material.texture_diffuse1, fromGeom.uv);

	if (uvColor.a < 0.1)
		discard;

	fragColor = uvColor;
}