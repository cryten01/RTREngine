#version 430 core

layout (triangles) in;								// the points that represent the geometry location in space
layout (triangle_strip, max_vertices = 3) out; 		// the generated geometry


/** INPUTS */
in VertexData {
    vec3 position_world;
	vec3 normal_world;
	vec2 uv;
} vert_in[];		// represents an interface block (better because shader inputs can get quite large)


/** OUTPUTS */
out VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 uv;
} vert_out;


/** UNIFORMS */
uniform float time;
uniform bool enableGeometryShader;



vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 1.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0); // translates the position vector along the direction of the normal vector
}


/** 
/*	Retrieves the normal vector using 3 input vertex coordinates
**/
vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {    

	if (enableGeometryShader) {
		vec3 normal = GetNormal();

		gl_Position = explode(gl_in[0].gl_Position, normal);
		vert_out.normal_world = vert_in[0].normal_world;
		vert_out.uv = vert_in[0].uv;
		EmitVertex();
		
		gl_Position = explode(gl_in[1].gl_Position, normal);
		vert_out.normal_world = vert_in[1].normal_world;
		vert_out.uv = vert_in[1].uv;
		EmitVertex();
		
		gl_Position = explode(gl_in[2].gl_Position, normal);
		vert_out.normal_world = vert_in[2].normal_world;
		vert_out.uv = vert_in[2].uv;
		EmitVertex();
		
		EndPrimitive();

	} else {
		
		gl_Position = gl_in[0].gl_Position; 
		vert_out.normal_world = vert_in[0].normal_world;
		vert_out.uv = vert_in[0].uv;
		EmitVertex();

		gl_Position = gl_in[1].gl_Position; 
		vert_out.normal_world = vert_in[1].normal_world;
		vert_out.uv = vert_in[1].uv;
		EmitVertex();
		
		gl_Position = gl_in[2].gl_Position; 
		vert_out.normal_world = vert_in[2].normal_world;
		vert_out.uv = vert_in[2].uv;
		EmitVertex();
		
		EndPrimitive();
	}
}