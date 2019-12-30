#pragma once
#include "Utils.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "ParticleSystem.h"
#include "SceneObject.h"
#include "Model.h"
#include "Skybox.h"
#include "Camera.h"
#include "Controls.h"


// Create framebuffers here
FrameBuffer screenQuadBuffer(800, 600, DEFAULT);
FrameBuffer hdrBuffer(800, 600, FLOAT);

// Load shaders here (location starts at solution folder)
std::shared_ptr<Shader> standardShader = std::make_shared<Shader>("standard.vert", "standard.frag", "standard.geom");
std::shared_ptr<Shader> skyboxShader = std::make_shared<Shader>("skybox.vert", "skybox.frag");
std::shared_ptr<Shader> postProcessShader = std::make_shared<Shader>("framebuffer.vert", "framebuffer.frag");
std::shared_ptr<Shader> geometryShader = std::make_shared<Shader>("geometry.vert", "geometry.frag", "geometry.geom");
std::shared_ptr<Shader> particleRenderShader = std::make_shared<Shader>("particles.vert", "particles.frag", "particles.geom");
std::shared_ptr<Shader> particleComputeShader = std::make_shared<Shader>("particles.comp");


//*********************//
//	Scene resources	   //
//*********************//

// Load textures here
Texture leatherTexture("../assets/textures/leather.jpg", TEX_DIFFUSE);
Texture floorTexture("../assets/textures/floor.jpg", TEX_DIFFUSE);
Texture snowflakeTexture("../assets/textures/snowflake.png", TEX_DIFFUSE);

// Create materials here
std::shared_ptr<Material> singleColorMaterial = std::make_shared<Material>(standardShader, glm::vec3(0.2f, 0.4f, 0.8f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
std::shared_ptr<Material> iceMaterial = std::make_shared<Material>(standardShader, glm::vec3(0.2f, 0.4f, 0.8f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
std::shared_ptr<Material> leatherMaterial = std::make_shared<TextureMaterial>(standardShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, leatherTexture);
std::shared_ptr<Material> floorMaterial = std::make_shared<TextureMaterial>(standardShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, floorTexture);
std::shared_ptr<Material> snowflakeMaterial = std::make_shared<TextureMaterial>(particleRenderShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, snowflakeTexture);

// Create meshes here
std::shared_ptr<Mesh> sphere1Mesh = std::make_shared<Mesh>(
	Mesh::createSphereGeometry(24, 24, 0.7f),
	iceMaterial
);

std::shared_ptr<Mesh> sphere2Mesh = std::make_shared<Mesh>(
	Mesh::createSphereGeometry(24, 24, 0.7f),
	iceMaterial
);

std::shared_ptr<Mesh> sphere3Mesh = std::make_shared<Mesh>(
	Mesh::createSphereGeometry(24, 24, 0.7f),
	iceMaterial
);

std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>(
	Mesh::createCubeGeometry(1.0f, 1.0f, 2.5f),
	singleColorMaterial
);

std::shared_ptr<Mesh> floorMesh = std::make_shared<Mesh>(
	Mesh::createCubeGeometry(40.0f, 0.5f, 40.0f),
	floorMaterial
);

std::shared_ptr<Mesh> podiumMesh = std::make_shared<Mesh>(
	Mesh::createCylinderGeometry(24.0f, 2.0f, 3.0f),
	singleColorMaterial
);


// Create particle systems here
std::shared_ptr<ParticleSystem> snow = std::make_shared<ParticleSystem>(ParticleSystem::createSnowEmitter(), snowflakeMaterial, particleComputeShader, particleRenderShader);

// Create scene objects here
std::shared_ptr<SceneObject> sphere1 = std::make_shared<SceneObject>(standardShader, glm::mat4(1));
std::shared_ptr<SceneObject> sphere2 = std::make_shared<SceneObject>(standardShader, glm::mat4(1));
std::shared_ptr<SceneObject> sphere3 = std::make_shared<SceneObject>(standardShader, glm::mat4(1));
std::shared_ptr<SceneObject> cube = std::make_shared<SceneObject>(standardShader, glm::mat4(1));
std::shared_ptr<SceneObject> podium = std::make_shared<SceneObject>(standardShader, glm::mat4(1));
std::shared_ptr<SceneObject> nanoMan = std::make_shared<SceneObject>(standardShader, glm::mat4(1));
std::shared_ptr<SceneObject> floorObj = std::make_shared<SceneObject>(standardShader, glm::mat4(1));

std::vector<std::shared_ptr<SceneObject>> renderableObjects;

// Create model loader here (object files must be in separate directory)
Model modelLoader;

// Create directional lights here
DirectionalLight dirLight(glm::vec3(1.0f), glm::vec3(0, -1, 0));

// Create point lights here
std::vector<std::shared_ptr<PointLight>> pointLights;

// Create spot lights here
std::vector<std::shared_ptr<SpotLight>> spotLights;

// Create skybox here
std::vector<std::string> cubeMapFileNames = {
	"skybox/right.jpg",
	"skybox/left.jpg",
	"skybox/top.jpg",
	"skybox/bottom.jpg",
	"skybox/back.jpg",
	"skybox/front.jpg"
};
Skybox skybox(skyboxShader, 60.0f, cubeMapFileNames);

// Create debug cam here

// View frustum dimensions
float fov = 60.0f, nearZ = 0.1f, farZ = 400.0f;

Camera orbitCam(fov, 800 / 600, nearZ, farZ);



void initLights() 
{
	pointLights.push_back(std::make_shared<PointLight>(
		glm::vec3(1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.4f, 0.1f)
		));

	//pointLights.push_back(std::make_shared<PointLight>(
	//	glm::vec3(0.6f),
	//	glm::vec3(0.0f, -1.0f, 0.0f),
	//	glm::vec3(1.0f, 0.4f, 0.1f)
	//	));

	spotLights.push_back(std::make_shared<SpotLight>(
		glm::vec3(1.0f),
		glm::vec3(0.0f, 12.0f, 4.0f),
		glm::vec3(1.0f, 0.4f, 0.1f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::cos(glm::radians(10.5f)),
		glm::cos(glm::radians(12.5f))
		));
}


void initSceneObjects() 
{
	// Load models here
	modelLoader.load(nanoMan, "../assets/models/nanosuit/nanosuit.obj");

	//*********************//
	//	Intial transforms  //
	//*********************//

	// Set initial transformations here
	sphere1->getTransform()->setLocalPos(glm::vec3(-10.0f, 10.0, 0.0));
	sphere2->getTransform()->setLocalPos(glm::vec3(3.0f, 4.0, 0.0));
	sphere3->getTransform()->setLocalPos(glm::vec3(1.0f, 4.0, 0.0));
	podium->getTransform()->setLocalPos(glm::vec3(0.0f, 1.0, 0.0));
	cube->getTransform()->setLocalPos(glm::vec3(0, 10, 6));
	nanoMan->getTransform()->setLocalPos(glm::vec3(0, 1, 0));

	// Push back scene objects that should be rendered here
	renderableObjects.push_back(cube);
	renderableObjects.push_back(floorObj);
	renderableObjects.push_back(sphere1);

	//****************************//
	//	Scene object attachments  //
	//****************************//

	// Attach meshes here
	sphere1->addMesh(sphere1Mesh);
	sphere2->addMesh(sphere2Mesh);
	sphere3->addMesh(sphere3Mesh);
	cube->addMesh(cubeMesh);
	podium->addMesh(podiumMesh);
	floorObj->addMesh(floorMesh);

	// Attach children here
	floorObj->addChild(podium);
	podium->addChild(nanoMan);
	sphere1->addChild(sphere2);
	sphere2->addChild(sphere3);

	// Attach lights here
	cube->setLight(pointLights.at(0));
}



// Test variables (for debugging only!)
float range = 40;
float threshold = 30;
float step = 20;
bool up = true;

// For debugging only
void updateScene(float deltaTime, Controls& controls)
{
	// Update controls
	controls.update();

	// Update camera
	orbitCam.update(controls.getMouseX(), controls.getMouseY(), controls.getZoom(), controls.getDragging(), controls.getStrafing(), controls.getHeight());

	// Update scene here
	if (controls.getFreezeScene() == false)
	{
		// Update test (For debugging purposes only!)
		range += step * deltaTime;

		if (range > 360.0f)
			range = 0.0f;

		// Update transformations here
		nanoMan->getTransform()->setLocalRot(glm::vec3(0, range * 2.0, 0));
		cube->getTransform()->setLocalPos(glm::vec3(0, 8 + range * 0.3, 6));
		sphere1->getTransform()->setLocalRot(glm::vec3(0, range * 2.0, 0));
		sphere2->getTransform()->setLocalRot(glm::vec3(range * 2.0, 0, 0));

		// Update scene objects here
		cube->update();
		sphere1->updateAll();
		floorObj->updateAll();
		snow->update(deltaTime);
	}
}

void renderScene()
{
	/**
	*	First render pass (render scene into screenQuadBuffer)
	**/

	// Set per-frame uniforms
	setPerFrameUniforms(standardShader.get(), orbitCam, dirLight, pointLights, spotLights, skybox);

	// Switch to screnQuadBuffer
	//hdrBuffer.use();

	// Render scene
	//render();
	snow->render(orbitCam.getViewMatrix(), orbitCam.getProjMatrix());
	skybox.render(orbitCam.getViewMatrix(), orbitCam.getProjMatrix()); // render skybox always last!

	// Switch back to default buffer
	//hdrBuffer.unuse();

	/**
	*	Second render pass (render buffer to quad)
	**/

	//hdrBuffer.renderScreenQuad(postProcessShader, _hdr, _exposure);
}


void setPerFrameUniforms(Shader* shader, Camera& camera, DirectionalLight dirLight, std::vector<std::shared_ptr<PointLight>> pointLights, std::vector<std::shared_ptr<SpotLight>> spotLights, Skybox& skybox)
{
	shader->use();

	//**********//
	//	Skybox  //
	//**********//

	skybox.bindTextures(5);
	shader->setUniform("skybox", 5);


	//**********//
	//	Camera  //
	//**********//

	shader->setUniform("viewProjMatrix", camera.getProjMatrix() * camera.getViewMatrix());
	shader->setUniform("camera_world", camera.getPosition());


	//*******************//
	//	Geometry Shader	 //
	//*******************//

	shader->setUniform("time", (float)glfwGetTime());
	shader->setUniform("enableGeometryShader", false);


	//***********//
	//	Lights	 //
	//***********//

	shader->setUniform("param.illuminated", true);

	shader->setUniform("dirL.color", dirLight.color);
	shader->setUniform("dirL.direction", dirLight.direction);

	shader->setUniform("NR_POINT_LIGHTS", pointLights.size());
	for (int i = 0; i < pointLights.size(); i++)
	{
		shader->setUniform("pointL[" + std::to_string(i) + "].color", pointLights.at(i)->color);
		shader->setUniform("pointL[" + std::to_string(i) + "].position", pointLights.at(i)->position);
		shader->setUniform("pointL[" + std::to_string(i) + "].attenuation", pointLights.at(i)->attenuation);
	}

	shader->setUniform("NR_SPOT_LIGHTS", spotLights.size());
	for (int i = 0; i < spotLights.size(); i++)
	{
		shader->setUniform("spotL[" + std::to_string(i) + "].color", spotLights.at(i)->color);
		shader->setUniform("spotL[" + std::to_string(i) + "].position", spotLights.at(i)->position);
		shader->setUniform("spotL[" + std::to_string(i) + "].attenuation", spotLights.at(i)->attenuation);
		shader->setUniform("spotL[" + std::to_string(i) + "].direction", spotLights.at(i)->direction);
		shader->setUniform("spotL[" + std::to_string(i) + "].innerAngle", spotLights.at(i)->innerAngle);
		shader->setUniform("spotL[" + std::to_string(i) + "].outerAngle", spotLights.at(i)->outerAngle);
	}
}