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
	sampler2D texture_diffuse1;
	vec3 material_coefficients; // x = ambient, y = diffuse, z = specular 
	vec3 color;
	float shininess;
} material;

uniform struct DirectionalLight {
	vec3 color;
	vec3 direction;
} dirL;



/**
*	Adds diffuse and specular values to fragment color (phong lighting model)
**/
vec3 phong(vec3 nNormal, vec3 lightDirection, vec3 viewDirection, vec3 lightColor, vec3 objectColor, float diffuseStrength, float specularStrength, float shininess)
{
	// Calculate diffuse impact based on dot-product/angle (max prevents negative values)
	float diffuseLightAngle = max(dot(nNormal, lightDirection), 0.0);
	// Calculate final diffuse
	vec3 diffuse = lightColor * diffuseLightAngle;


	// Calculate reflect direction (-dirL.direction because vector is pointing from the light to fragment)
	vec3 reflectDirection = reflect(-dirL.direction, nNormal);
	// Calculate dot product/angle between viewDir and reflectDir (max prevents negative values)
	float specularLightAngle = max(dot(viewDirection, reflectDirection), 0.0);
	// Raise power based on shininess
	float spec = pow(specularLightAngle, shininess);
	// Calculate final specular
	vec3 specular = lightColor * specularStrength * spec; 


	// Must be multiplied with objectColor because phong is splitted
	return (diffuse + specular) * objectColor;
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
		vec3 nNormal = normalize(vert.normal_world);
		// Calculate view direction
		vec3 viewDir = normalize(camera_world - vert.position_world);

		
		// Calculate ambient color (outside phong() because calculation only once needed)
		vec3 ambient = material.material_coefficients.x * objectColor;
		color = vec4(ambient, 1);


		// Add directional light contribution
		vec3 diffuseAndSpecular = phong(nNormal, -dirL.direction, viewDir, dirL.color, objectColor, material.material_coefficients.y, material.material_coefficients.z, material.shininess);
		color.rgb += diffuseAndSpecular;
	}
}