#version 430 core

/** 
*	Pass through shader that takes a point as input and passes it to the fragment shader
**/
layout (points) in;
layout (points, max_vertices = 1) out;

void main() {    
    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();	// emit unmodified vertex position  
    EndPrimitive();	// completes the current output primitive on the first vertex stream
}  