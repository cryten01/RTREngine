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
	const float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, vert.uv).rgb;

    if(hdr)
    {
        // reinhard
        // vec3 result = hdrColor / (hdrColor + vec3(1.0));
        // exposure
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        // also gamma correct while we're at it       
        result = pow(result, vec3(1.0 / gamma));
        color = vec4(result, 1.0);
    }
    else
    {
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        color = vec4(result, 1.0);
    }


//    color = texture(screenTexture, vert.uv);
}