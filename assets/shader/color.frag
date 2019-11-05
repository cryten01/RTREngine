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




/* ---------------------------------- */
// Diffuse, Specular, Attenuation
/* ---------------------------------- */

float calcDiffuse(float diffConstant, vec3 lightDir, vec3 normal)
{
	// diffuse = kd * cosTheta
	float diffLightAngle = max(dot(normal, lightDir), 0.0); 
	return (diffConstant * diffLightAngle);
};


float calcSpecular(float specConstant, float alpha, vec3 lightDir, vec3 eyeDir, vec3 normal)
{	
	// specular = ks * shininess constant     
	vec3 reflection = reflect(-lightDir, normal); 
	float specLightAngle = max(dot(eyeDir, reflection), 0.0);
	float shininess = pow(specLightAngle, alpha);
	return specConstant * shininess;
};


float calcAttenuation(vec3 attenuation, vec3 lightPosition, vec3 fragPosition)
{
	// attenuation
	float distance = length(lightPosition - fragPosition); 
	return (1.0f / (attenuation.x + attenuation.y * distance + attenuation.z * (distance * distance)));
}


/* ---------------------------- */
// Directional Lightsource
/* ---------------------------- */

vec3 calcDirLight(DirLight light, vec3 normal, vec3 eyeDir, Material material)
{	
	// -light.direction because light direction expected from the lightSource towards the fragment

	float diffuse = calcDiffuse(material.light.y, normalize(-light.direction), normal);
	float specular = calcSpecular(material.light.z, material.alpha, normalize(-light.direction), eyeDir, normal);
	
	// final
	return light.color * (diffuse + specular);
}


/* ---------------------------- */
// Point Lightsource
/* ---------------------------- */

vec3 calcPointLight(PointLight light, vec3 normal, vec3 eyeDir, Material material)
{		
	// light(direction)
	vec3 lightDir = normalize(light.position - vert.position_world); 

	float diffuse = calcDiffuse(material.light.y, lightDir, normal);
	float specular = calcSpecular(material.light.z, material.alpha, lightDir, eyeDir, normal);
	float attenuation = calcAttenuation(light.attenuation, light.position, vert.position_world);
	
	// final
	vec3 result = light.color * (diffuse + specular);
	return result * attenuation;
}




// lightDir from fragment to light
// diffuseColor = lightColor * objectColor
vec3 phong(vec3 normal, vec3 lightDir, vec3 eyeDir, vec3 diffCol, float kd, vec3 specCol, float ks, float alpha, bool attenuate, vec3 attenuation) {
	
	// Normalize
	lightDir = normalize(lightDir);

	// Attenuation part
	float distance = length(lightDir);
	float attRes = (attenuate) ?  1.0f / (attenuation.x + distance * attenuation.y + distance * distance * attenuation.z) : 1.0;
	
	// Diffuse part
	float cosTheta = max(0, dot(normal, lightDir));
	
	// Specular part
	vec3 reflectDir = reflect(-lightDir, normal);
	float cosAlpha = max(dot(eyeDir, reflectDir), 0.0);
	float shininess = pow(cosAlpha, alpha);

	// att * [kd * cosTheta + ks * shininess]
	return attRes * (kd * diffCol * cosTheta + ks * specCol * shininess); 
}


void main() {	

	vec3 I,R;

	if (param.state == REFLECTIVE) {
		vec3 I = normalize(vert.position_world - camera_world);
		vec3 R = reflect(I, normalize(vert.normal_world));
		fragColor = vec4(texture(skybox, R).rgb, 1.0);
	} 
	
	if (param.state == REFRACTIVE) {
		float ratio = 1.00 / 1.309;
		vec3 I = normalize(vert.position_world - camera_world);
		vec3 R = refract(I, normalize(vert.normal_world), ratio);
		fragColor = vec4(texture(skybox, R).rgb, 1.0);
	} 
	

	if (param.state == TEXTURE || param.state == DIFFUSE) {
		// Define objectColor
		vec3 objectColor = (param.state == TEXTURE) ? texture2D(material.texture_diffuse1, vert.uv).rgb : material.color;
		
		// Normalize normal vector
		vec3 normal = normalize(vert.normal_world);
		// Calculate eye direction
		vec3 eyeDir = normalize(camera_world - vert.position_world);

		// Calculate ambient (Ia * ka)
		fragColor = vec4((objectColor * material.light.x), 1); // ambient

		// Add dirLight
		fragColor.rgb += phong(
			normal,
			-dirL.direction,
			eyeDir,
			dirL.color * objectColor,
			material.light.y,
			dirL.color,
			material.light.z,
			material.alpha,
			false,
			vec3(0)
		);

		// Add pointLights
		fragColor.rgb += phong(
			normal,
			pointL[0].position - vert.position_world,
			eyeDir,
			pointL[0].color * objectColor, 
			material.light.y, 
			pointL[0].color, 
			material.light.z, 
			material.alpha, 
			true, 
			pointL[0].attenuation
		);
	}
}