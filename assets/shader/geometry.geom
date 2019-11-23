#version 430 core

/** 
*	This shader dynamically creates geometry on the GPU 
**/

layout (points) in;								// the input type
layout (triangle_strip, max_vertices = 4) out;	// the output type (triangle_strip allows for connected triangles)


in VertexData {
    vec3 color;
} gs_in[];		// interface block that contains values of each vertex of the primitive from the vertex shader (e.g. size 3 when triangles are rendered)


out vec3 geoColor;


// Generate a basic house (for testing purposes only)
void build_quad(vec4 position) 
{    
	// EmitVertex() emits a vertex to the first vertex stream
	// EndPrimitive() completes the current output primitive on the first vertex stream

	// gs_in[0] since there's only one input vertex
    geoColor = gs_in[0].color; 

	// 1:bottom-left 
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);   
    EmitVertex();   

	// 2:bottom-right
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); 
    EmitVertex();

	// 3:top-left
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); 
    EmitVertex();

	// 4:top-right
	geoColor = vec3(1.0, 1.0, 1.0); // sets color of current primitive vertex (must be before vertex emission)
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); 
    EmitVertex();

    EndPrimitive();
}


void main() 
{    
	build_quad(gl_in[0].gl_Position);	// passes the first position of the primitive
}  