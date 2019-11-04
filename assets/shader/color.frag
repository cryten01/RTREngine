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

uniform struct DirectionalLight {
	vec3 color;
	vec3 direction;
} dirL;



/**
*	Adds diffuse and specular values to fragment color (phong lighting model)
**/
vec3 phongDiffuseSpecular(vec3 nNormal, vec3 viewDirection, vec3 lightDirection, vec3 lightColor, vec3 objectColor, float diffuseConstant, float specularConstant, float alpha)
{
	// Calculate diffuse impact based on dot-product/angle (cosTheta, lightDirection must be inverted because vector from normal is required)
	float diffuseLightAngle = dot(nNormal, -lightDirection);
	// Max prevents negative values 
	diffuseLightAngle = max(diffuseLightAngle, 0.0);
	// Calculate final diffuse (I * kd * cosTheta)
	float diffuse = diffuseConstant * diffuseLightAngle;


	// Calculate reflect direction
	vec3 reflectDirection = reflect(lightDirection, nNormal);
	// Calculate dot product/angle between viewDir and reflectDir (max prevents negative values)
	float specularLightAngle = dot(viewDirection, reflectDirection);
	// Max prevents negative values 
	specularLightAngle = max(specularLightAngle, 0.0);
	// Raise power based on shininess (cos alpha)
	float shininess = pow(specularLightAngle, alpha);
	// Calculate final specular
	float specular = specularConstant * shininess; 

	// lightColor includes also intensity of the light
	return lightColor * (diffuse + specular);
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

		
		// Calculate ambient impact (outside phong() because calculation only once needed. Mimics ambient reflexions by emitting light)
		float ambientConstant = material.light.x;
		vec3 ambient = dirL.color * ambientConstant;
		

		// Add directional light contribution
		vec3 diffuseSpecular = phongDiffuseSpecular(
			nNormal, 
			viewDir, 
			dirL.direction, 
			dirL.color, 
			objectColor, 
			material.light.y,
			material.light.z,
			material.alpha
		);

		color = vec4((ambient + diffuseSpecular) * objectColor, 1);
	}
}