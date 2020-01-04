#include "Resources.h"

using namespace RTREngine;

Resources::Resources()
{
}


Resources::~Resources() 
{
}


void Resources::init()
{
	// Framebuffers
	screenQuadBuffer = std::make_shared<FrameBuffer>(800, 600, DEFAULT);
	hdrBuffer = std::make_shared<FrameBuffer>(800, 600, FLOAT);

	// Shaders
	standardShader = std::make_shared<Shader>("standard.vert", "standard.frag", "standard.geom");
	skyboxShader = std::make_shared<Shader>("skybox.vert", "skybox.frag");
	postProcessShader = std::make_shared<Shader>("framebuffer.vert", "framebuffer.frag");
	geometryShader = std::make_shared<Shader>("geometry.vert", "geometry.frag", "geometry.geom");
	particleRenderShader = std::make_shared<Shader>("particles.vert", "particles.frag", "particles.geom");
	particleComputeShader = std::make_shared<Shader>("particles.comp");

	// ModelLoader
	modelLoader = std::make_shared<Model>();
}

std::shared_ptr<Scene> Resources::loadTestScene()
{
	// Load textures here
	std::shared_ptr<Texture> leatherTexture = std::make_shared<Texture>("../assets/textures/leather.jpg", TEX_DIFFUSE);
	std::shared_ptr<Texture> floorTexture = std::make_shared<Texture>("../assets/textures/floor.jpg", TEX_DIFFUSE);
	std::shared_ptr<Texture> snowflakeTexture = std::make_shared<Texture>("../assets/textures/snowflake.png", TEX_DIFFUSE);

	// Create materials here
	std::shared_ptr<Material> singleColorMaterial = std::make_shared<Material>(standardShader, glm::vec3(0.2f, 0.4f, 0.8f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	std::shared_ptr<Material> iceMaterial = std::make_shared<Material>(standardShader, glm::vec3(0.2f, 0.4f, 0.8f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	std::shared_ptr<Material> leatherMaterial = std::make_shared<TextureMaterial>(standardShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, leatherTexture);
	std::shared_ptr<Material> floorMaterial = std::make_shared<TextureMaterial>(standardShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, floorTexture);
	std::shared_ptr<Material> snowflakeMaterial = std::make_shared<TextureMaterial>(particleRenderShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, snowflakeTexture);

	// Create debug cam here
	float fov = 60.0f, nearZ = 0.1f, farZ = 400.0f; // view frustum dimensions
	std::shared_ptr<Camera> orbitCam = std::make_shared<Camera>(fov, 800 / 600, nearZ, farZ);

	// Create skybox here
	std::vector<std::string> cubeMapFileNames = {
		"skybox/right.jpg",
		"skybox/left.jpg",
		"skybox/top.jpg",
		"skybox/bottom.jpg",
		"skybox/back.jpg",
		"skybox/front.jpg"
	};
	std::shared_ptr<Skybox> skybox = std::make_shared<Skybox>(orbitCam, 60.0f, cubeMapFileNames);

	// Load sceneObjects
	std::shared_ptr<SceneObject> camObj = std::make_shared<SceneObject>();
	camObj->addComponent(orbitCam);

	// Load scene
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	scene->addSceneObject(camObj);
	scene->setActiveSkybox(skybox);

	return scene;
};