#version 430 core

in VertexData {
	vec2 uv;
} vert;

out vec4 color;

uniform sampler2D screenTexture;
uniform bool hdr;
uniform float exposure;

void main()
{ 
	// Average gamma of most displays
	const float gamma = 2.2;

	// The color from the HDR buffer
    vec3 hdrColor = texture(screenTexture, vert.uv).rgb;

    if(hdr)
    {
        // Exposure tone mapping (default at 1.0)
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

		// Gamma correction (tone mapped color from buffer is raised to the power of the gamma correction)
        result = pow(result, vec3(1.0 / gamma));
        color = vec4(result, 1.0);
    }
    else
    {
		// Gamma correction (color from buffer is raised to the power of the gamma correction)
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        color = vec4(result, 1.0);
    }

//	color = texture(screenTexture, vert.uv);
}