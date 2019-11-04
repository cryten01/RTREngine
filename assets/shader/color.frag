#version 430 core

/** INPUTS */
in VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 uv;
} vert;

/** UNIFORMS */
uniform vec3 camera_world;
uniform samplerCube skybox;
uniform vec3 diffuseColor;

uniform struct Parameters {
	int state;
} param;

uniform struct Material {
	sampler2D texture_diffuse1;
	vec3 material_coefficients; // x = ambientStrength, y = diffuse, z = specular 
	float shininess;			// shininess constant
} material;

uniform struct DirectionalLight {
	vec3 color;
	vec3 direction;
} dirL;



/** OUTPUTS */
out vec4 color;


/**
*	Calculate fragment color affected by phong lighting model
**/
vec3 phong(vec3 nNormal, vec3 lightDirection, vec3 viewDirection, vec3 diffuseLightColor, float diffuseStrength, vec3 specularLightColor, float specularStrength, float shininess)
{
	// Calculate diffuse impact based on dot-product/angle (max prevents negative values)
	float diffuseLightAngle = max(dot(nNormal, lightDirection), 0.0);
	// Calculate final diffuse
	vec3 diffuse = diffuseLightColor * diffuseLightAngle;


	// Calculate reflect direction (-dirL.direction because vector is pointing from the light to fragment)
	vec3 reflectDirection = reflect(-dirL.direction, nNormal);
	// Calculate dot product/angle between viewDir and reflectDir (max prevents negative values)
	float specularLightAngle = max(dot(viewDirection, reflectDirection), 0.0);
	// Raise power based on shininess
	float spec = pow(specularLightAngle, shininess);
	// Calculate final specular
	vec3 specular = specularStrength * spec * dirL.color; 


	return (diffuse + specular) * diffuseColor;
}

void main() {	

	vec3 I,R;

	if (param.state == 0) 
	{
		vec3 I = normalize(vert.position_world - camera_world);
		vec3 R = reflect(I, normalize(vert.normal_world));
		color = vec4(texture(skybox, R).rgb, 1.0);
	} 
	
	if (param.state == 1)
	{
		float ratio = 1.00 / 1.309;
		vec3 I = normalize(vert.position_world - camera_world);
		vec3 R = refract(I, normalize(vert.normal_world), ratio);
		color = vec4(texture(skybox, R).rgb, 1.0);
	} 
	
	if (param.state == 2)
	{
		color = texture2D(material.texture_diffuse1, vert.uv);
	}

	if (param.state == 3)
	{
		// Normalize normal vector
		vec3 nNormal = normalize(vert.normal_world);
		// Calculate view direction
		vec3 viewDir = normalize(camera_world - vert.position_world);

		// Calculate ambient color (outside phong() because calculation only once needed)
		vec3 ambient = material.material_coefficients.x * diffuseColor;
		color = vec4(ambient, 1);

		// Add directional light contribution
		color.rgb += phong(nNormal, -dirL.direction, viewDir, dirL.color * diffuseColor, material.material_coefficients.y, dirL.color, material.material_coefficients.z, material.shininess);
	}
}