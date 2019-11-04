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
out vec4 color;

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


struct DirLight {
	vec3 color;
	vec3 direction;
};

uniform DirLight dirL[1];




/* ---------------------------------- */
// Diffuse, Specular, Attenuation
/* ---------------------------------- */

float calcDiffuse(float diffConstant, vec3 lightDir, vec3 normal)
{
	// diffuse = lightIntensity * kd * Lambert's law
	float diffLightAngle = max(dot(normal, -lightDir), 0.0); 
	return (diffConstant * diffLightAngle);
};


float calcSpecular(float specConstant, float alpha, vec3 lightDir, vec3 eyeDir, vec3 normal)
{	
	// specular = lightIntensity * ks * shininess constant     
	vec3 reflection = reflect(lightDir, normal); 
	float specLightAngle = max(dot(eyeDir, reflection), 0.0);
	float shininess = pow(specLightAngle, alpha);
	return specConstant * shininess;
};


vec3 calcAttenuation(vec3 attenuation, vec3 lightPosition, vec3 fragPosition)
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
	float diffConstant = material.light.y;
	float specConstant = material.light.z;

	float diffuse = calcDiffuse(diffConstant, normalize(light.direction), normal);
	float specular = calcSpecular(specConstant, material.alpha, normalize(light.direction), eyeDir, normal);
	
	// final
	return light.color * (diffuse + specular);
}



void main() {	

	vec3 I,R;

	if (param.state == REFLECTIVE) 
	{
		vec3 I = normalize(vert.position_world - camera_world);
		vec3 R = reflect(I, normalize(vert.normal_world));
		color = vec4(texture(skybox, R).rgb, 1.0);
	} 
	
	if (param.state == REFRACTIVE)
	{
		float ratio = 1.00 / 1.309;
		vec3 I = normalize(vert.position_world - camera_world);
		vec3 R = refract(I, normalize(vert.normal_world), ratio);
		color = vec4(texture(skybox, R).rgb, 1.0);
	} 
	

	if (param.state == TEXTURE || param.state == DIFFUSE)
	{
		// Define objectColor
		vec3 objectColor = (param.state == TEXTURE) ? texture2D(material.texture_diffuse1, vert.uv).rgb : material.color;
		
		// Normalize normal vector
		vec3 normal = normalize(vert.normal_world);
		
		// Calculate eye direction
		vec3 eyeDir = normalize(camera_world - vert.position_world);

		// Calculate ambient
		color = vec4(material.light.x * objectColor, 1);

		// Add lights to color
		for (int i = 0; i < dirL.length(); i++)
		{
			vec3 dirLight = calcDirLight(dirL[i], normal, eyeDir, material);
			color.rgb += dirLight * objectColor;
		}
	}
}