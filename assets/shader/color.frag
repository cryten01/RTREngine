#version 430 core

/** STATE ENUMS */
const uint REFLECTIVE = 0;
const uint REFRACTIVE = 1;
const uint TEXTURE = 2;
const uint DIFFUSE = 3;


/** INPUTS */
in VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 uv;
} vert;


/** OUTPUTS */
out vec4 fragColor;


/** UNIFORMS */
uniform vec3 camera_world;
uniform samplerCube skybox;


uniform struct ControlParameters {
	int state;
	bool illuminated;
} param;


uniform struct Material {
	sampler2D texture_diffuse1;		// don't change naming. requires consinstent naming scheme for loading
	vec3 light;						// x = ambient, y = diffuse, z = specular 
	vec3 color;
	float alpha;
} material;


uniform struct DirLight {
	vec3 color;
	vec3 direction;	 // from the lightSource towards the fragment
} dirL;


struct PointLight {
	vec3 color;
	vec3 position;
	vec3 attenuation;
};
uniform PointLight pointL[10];
uniform uint NR_POINT_LIGHTS;


struct SpotLight {
	vec3 color;
	vec3 position;
	vec3 direction;
	vec3 attenuation;
	float innerAngle;
	float outerAngle;
};
uniform SpotLight spotL[10];
uniform uint NR_SPOT_LIGHTS;



/* ---------------------------------- */
// Diffuse, Specular, Attenuation
/* ---------------------------------- */

// Diffuse color = objectColor * light.color
// Specular color = light.Color

vec3 calcDiffuse(vec3 diffCol, float kd, vec3 lightDir, vec3 normal)
{
	float cosTheta = max(0, dot(normal, lightDir));

	return diffCol * kd * cosTheta;
};


vec3 calcSpecular(vec3 specCol, float ks, float alpha, vec3 lightDir, vec3 eyeDir, vec3 normal)
{	
	vec3 reflectDir = reflect(-lightDir, normal);
	float cosAlpha = max(dot(eyeDir, reflectDir), 0.0);
	float shininess = pow(cosAlpha, alpha);
	
	return specCol * ks * shininess;
};


float calcAttenuation(vec3 attenuation, vec3 lightDir)
{
	float distance = length(lightDir);

	return 1.0f / (attenuation.x + distance * attenuation.y + distance * distance * attenuation.z);
}


/* ---------------------------- */
// Directional Lightsource
/* ---------------------------- */

vec3 calcDirLight(DirLight light, Material material, vec3 objectColor, vec3 normal, vec3 eyeDir)
{
	// Normalize
	vec3 lightDir = normalize(light.direction);

	// Invert lightDir because from fragment to light needed
	lightDir = -lightDir;

	vec3 diffuse = calcDiffuse(light.color * objectColor, material.light.y, lightDir, normal);
	vec3 specular = calcSpecular(light.color, material.light.z, material.alpha, lightDir, eyeDir, normal);
	
	// [kd * cosTheta + ks * shininess]
	return diffuse + specular; 
}


/* ---------------------------- */
// Point Lightsource
/* ---------------------------- */

vec3 calcPointLight(PointLight light, Material material, vec3 objectColor, vec3 normal, vec3 eyeDir)
{	
	// Normalize
	vec3 lightDir = normalize(light.position - vert.position_world);

	vec3 diffuse = calcDiffuse(light.color * objectColor, material.light.y, lightDir, normal);
	vec3 specular = calcSpecular(light.color, material.light.z, material.alpha, lightDir, eyeDir, normal);
	float attenuation = calcAttenuation(light.attenuation, lightDir);

	// att * [kd * cosTheta + ks * shininess]
	return attenuation * (diffuse + specular);
}


/* ---------------------------- */
// Spot Lightsource
/* ---------------------------- */

vec3 calcSpotLight(SpotLight light, Material material, vec3 objectColor, vec3 normal, vec3 eyeDir)
{
	// Normalize
	vec3 lightDir = normalize(light.position - vert.position_world);

	vec3 diffuse = calcDiffuse(light.color * objectColor, material.light.y, lightDir, normal);
	vec3 specular = calcSpecular(light.color, material.light.z, material.alpha, lightDir, eyeDir, normal);
	float attenuation = calcAttenuation(light.attenuation, lightDir);


	// SpotLight part
	float theta = dot(lightDir, normalize(-light.direction));				 // the angle between the LightDir and the SpotDir
	float epsilon = light.innerAngle - light.outerAngle;                     // the difference between inner - outerCutoff ==> (theta - outerCutoff) / epsilon
	float intensity = clamp((theta - light.outerAngle) / epsilon, 0.0, 1.0); // clamping avoids if/else statments


	// final
	return (diffuse + specular) * attenuation * intensity;
}



void main() {	

	vec3 objectColor,R;

	// Normalize normal vector
	vec3 normal = normalize(vert.normal_world);
	// Calculate eye direction
	vec3 eyeDir = normalize(camera_world - vert.position_world);


	// Define objectColor
	if (param.state == REFLECTIVE) {
		vec3 R = reflect(-eyeDir, normal);
		objectColor = texture(skybox, R).rgb;
	} 
	if (param.state == REFRACTIVE) {
		float ratio = 1.00 / 1.309;
		vec3 R = refract(-eyeDir, normal, ratio);
		objectColor = texture(skybox, R).rgb;
	} 
	if (param.state == TEXTURE) {
		objectColor = texture2D(material.texture_diffuse1, vert.uv).rgb; 
	} 
	if (param.state == DIFFUSE) {
		objectColor = material.color;
	}
	

	if (param.illuminated) {
		// Calculate ambient (Ia * ka)
		fragColor = vec4((objectColor * material.light.x), 1); // ambient
		
		// Add directional light
		fragColor.rgb += calcDirLight(dirL, material, objectColor, normal, eyeDir);
		
		// Add point lights
		for (int i = 0; i < NR_POINT_LIGHTS; i++) {
			fragColor.rgb += calcPointLight(pointL[i], material, objectColor, normal, eyeDir);
		}

		// Add spot lights		
		for (int i = 0; i < NR_SPOT_LIGHTS; i++) {
			fragColor.rgb += calcSpotLight(spotL[i], material, objectColor, normal, eyeDir);
		}
	} else {
		fragColor = vec4(objectColor, 1);
	}
}