#version 430 core

/** 
*	This shader dynamically creates geometry on the GPU 
**/

layout (points) in;								// the points that represent the geometry location in space
layout (triangle_strip, max_vertices = 5) out;	// the generated geometry


in VertexData {
    vec3 color;
} gs_in[];		// represents an interface block (better because shader inputs can get quite large)


out vec3 geoColor;


// Generate a basic house (for testing purposes only)
void build_quad(vec4 position) 
{    
	// EmitVertex() emits a vertex to the first vertex stream
	// EndPrimitive() completes the current output primitive on the first vertex stream

    geoColor = gs_in[0].color; // gs_in[0] since there's only one input vertex
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // 1:bottom-left   
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); // 4:top-right
	geoColor = vec3(1.0, 1.0, 1.0); // color can be changed before vertex emission
    EmitVertex();
    EndPrimitive();
}


void main() 
{    
	build_quad(gl_in[0].gl_Position);
}  