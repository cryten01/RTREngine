#version 430 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VertexData {
    vec3 normal_world;
} vert_in[];

const float MAGNITUDE = 0.4;

void GenerateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(vert_in[index].normal_world, 0.0) * MAGNITUDE;
    EmitVertex();

    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}  