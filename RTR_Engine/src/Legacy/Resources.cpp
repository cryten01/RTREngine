#include "rtrpch.h"
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


/* --------------------------------------------- */
// Scene resources
/* --------------------------------------------- */

SceneResources::SceneResources()
{
}


SceneResources::~SceneResources()
{
}


void SceneResources::init(std::shared_ptr<Input> input)
{
	// Textures
	leatherTexture = std::make_shared<Texture>("../assets/textures/leather.jpg", TEX_DIFFUSE);
	floorTexture = std::make_shared<Texture>("../assets/textures/floor.jpg", TEX_DIFFUSE);
	snowflakeTexture = std::make_shared<Texture>("../assets/textures/snowflake.png", TEX_DIFFUSE);

	// Materials
	singleColorMaterial = std::make_shared<Material>(Resources::Instance().standardShader, glm::vec3(0.2f, 0.4f, 0.8f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	iceMaterial = std::make_shared<Material>(Resources::Instance().standardShader, glm::vec3(0.2f, 0.4f, 0.8f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	leatherMaterial = std::make_shared<TextureMaterial>(Resources::Instance().standardShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, leatherTexture);
	floorMaterial = std::make_shared<TextureMaterial>(Resources::Instance().standardShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, floorTexture);
	snowflakeMaterial = std::make_shared<TextureMaterial>(Resources::Instance().particleRenderShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, snowflakeTexture);

	// Debug cam
	float fov = 60.0f, nearZ = 0.1f, farZ = 400.0f; // view frustum dimensions
	orbitCam = std::make_shared<Camera>(fov, 800 / 600, nearZ, farZ, input);

	// Skybox
	std::vector<std::string> cubeMapFileNames = {
		"skybox/right.jpg",
		"skybox/left.jpg",
		"skybox/top.jpg",
		"skybox/bottom.jpg",
		"skybox/back.jpg",
		"skybox/front.jpg"
	};

	skybox = std::make_shared<Skybox>(orbitCam, 60.0f, cubeMapFileNames);

	// Meshes
	sphere1Mesh = std::make_shared<Mesh>(Mesh::createSphereGeometry(24, 24, 0.7f), iceMaterial);
	sphere2Mesh = std::make_shared<Mesh>(Mesh::createSphereGeometry(24, 24, 0.7f), iceMaterial);
	sphere3Mesh = std::make_shared<Mesh>(Mesh::createSphereGeometry(24, 24, 0.7f), iceMaterial);
	cubeMesh = std::make_shared<Mesh>(Mesh::createCubeGeometry(2.0f, 2.0f, 2.0f), singleColorMaterial);
	floorMesh = std::make_shared<Mesh>(Mesh::createCubeGeometry(40.0f, 0.5f, 40.0f), floorMaterial);
	podiumMesh = std::make_shared<Mesh>(Mesh::createCylinderGeometry(24.0f, 2.0f, 3.0f), singleColorMaterial);

	// Particle systems
	snow = std::make_shared<ParticleSystem>(ParticleSystem::createSnowEmitter(), snowflakeMaterial, Resources::Instance().particleComputeShader, Resources::Instance().particleRenderShader, orbitCam);

	// Directional light
	dirLight = std::make_shared<DirectionalLight>(glm::vec3(1.0f), glm::vec3(1, -1, 0));

	// Point lights
	pointLights.push_back(std::make_shared<PointLight>(
		glm::vec3(1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.4f, 0.1f)
		));

	// Spot lights
	spotLights.push_back(std::make_shared<SpotLight>(
		glm::vec3(1.0f),
		glm::vec3(0.0f, 12.0f, 4.0f),
		glm::vec3(1.0f, 0.4f, 0.1f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::cos(glm::radians(10.5f)),
		glm::cos(glm::radians(12.5f))
		));
}


std::shared_ptr<Scene> SceneResources::loadTestScene(std::shared_ptr<Input> input)
{
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();

	// Load sceneObjects
	std::shared_ptr<SceneObject> camObj = std::make_shared<SceneObject>();
	camObj->addComponent(orbitCam);

	std::shared_ptr<SceneObject> floorObj = std::make_shared<SceneObject>();
	floorObj->addComponent(floorMesh);

	std::shared_ptr<SceneObject> cubeObj = std::make_shared<SceneObject>();
	cubeObj->addComponent(cubeMesh);
	cubeObj->getTransform()->setLocalPos(glm::vec3(0.0f, 10.0, 0.0));

	std::shared_ptr<SceneObject> snowObj = std::make_shared<SceneObject>();
	snowObj->addComponent(snow);

	std::shared_ptr<SceneObject> dirLightObj = std::make_shared<SceneObject>();
	dirLightObj->addComponent(dirLight);


	// Add sceneObjects
	scene->addSceneObject(camObj);
	scene->addSceneObject(floorObj);
	scene->addSceneObject(snowObj);
	scene->addSceneObject(cubeObj);
	scene->addSceneObject(dirLightObj);

	// Add per-frame resource pointers
	scene->setActiveSkybox(skybox);
	//scene->setActiveCamera(orbitCam);
	//scene->setActiveDirLight(dirLight);

	return scene;
};