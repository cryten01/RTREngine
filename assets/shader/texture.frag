#version 430 core
/*
* Copyright 2018 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/

in VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 uv;
    vec4 FragPosLightSpace;
} vert;

out vec4 color;

uniform vec3 camera_world;

uniform vec3 materialCoefficients; // x = ambient, y = diffuse, z = specular 
uniform float specularAlpha;
uniform sampler2D diffuseTexture;
uniform bool celShading;
uniform sampler2D shadowMap;
uniform samplerCube skybox;

uniform struct DirectionalLight {
	vec3 color;
	vec3 direction;
} dirL;

uniform struct PointLight {
	vec3 color;
	vec3 position;
	vec3 attenuation;
} pointL;

//diffuse component of light
float diffuseSimple(vec3 L, vec3 N){
   return clamp(dot(L,N),0.0,1.0);
}

//specular component of light
float specularSimple(vec3 L,vec3 N,vec3 H, float alpha, vec3 R, vec3 V){
   if(dot(N,L)>0){
      //return pow(clamp(dot(H,N),0.0,1.0),32.0);
	  return pow(max(0, dot(R, V)), alpha);
   }
   return 0.0;
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 n)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	
	// Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
	
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
	// Calculate bias for reducing shadow acne
    float bias = max(0.05 * (1.0 - dot(n, lightDir)), 0.005);    
    
	// PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0); 
   
    for(int i = -1; i <= 1; ++i)
    {   
        for(int j = -1; j <= 1; ++j)
         {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(i,j) * texelSize).r;
            shadow += (currentDepth - bias) <= pcfDepth ? 1.0 : 0.0;
          }
    }
    shadow /= 9.0;
	 
    if(projCoords.z > 1.0)
        shadow = 1.0;
		
    return shadow; 
}

vec3 phong(vec3 n, vec3 l, vec3 v, vec3 diffuseC, float diffuseF, vec3 specularC, float specularF, float alpha, bool attenuate, vec3 attenuation) {
	float d = length(l);
	l = normalize(l);
	float att = 1.0;	
	if(attenuate) att = 1.0f / (attenuation.x + d * attenuation.y + d * d * attenuation.z);
	vec3 r = reflect(-l, n);
	
	vec3 h = normalize(l + v);
	
	float ambientIntensity = 0.05f;
	float diffuseIntensity = diffuseSimple(l, n);
	float specularIntensity = specularSimple(l, n, h, alpha, r, v) * att;

	float intensity = ambientIntensity + diffuseIntensity + specularIntensity;
	
	if (celShading) { 
		diffuseIntensity = ceil(intensity * 2) / 2;
		specularIntensity = ceil(intensity * 2) / 2;
	}
	
	return (diffuseF * diffuseC * diffuseIntensity) + (specularF * specularC * specularIntensity);
}

void main() {	
	vec3 n = normalize(vert.normal_world);
	vec3 v = normalize(camera_world - vert.position_world);
	
	vec3 r = reflect(v,n);


	vec4 reflectedColour = texture(skybox, -r);
	
	vec3 texColor = texture(diffuseTexture, vert.uv).rgb;
	color = vec4(texColor * materialCoefficients.x, 1); // ambient
	
	// calculate shadow map
    float shadow = ShadowCalculation(vert.FragPosLightSpace, dirL.direction, n);

	// add directional light contribution
	// multiply with shadow * for additional shadow mapping
	color.rgb += phong(n, -dirL.direction, v, dirL.color * texColor, materialCoefficients.y, dirL.color, materialCoefficients.z, specularAlpha, false, vec3(0));
        			
	// add point light contribution
	// multiply with shadow * for additional shadow mapping
	color.rgb += phong(n, pointL.position - vert.position_world, v, pointL.color * texColor, materialCoefficients.y, pointL.color, materialCoefficients.z, specularAlpha, true, pointL.attenuation);

	// represents the final color as a mix between the color and the reflected color
	color = mix(color, reflectedColour, 0.5);
}

