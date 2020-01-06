#pragma once
#include "Utils.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "ParticleSystem.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Model.h"
#include "Skybox.h"
#include "Camera.h"
#include "Light.h"
#include "Input.h"


namespace RTREngine
{
	// Put forward declarations here
	class Scene;
	class Skybox;

	/**
	*	Represents the Meyers singleton approach.
	*	Call variables with Resources::Instance().var
	*	Call functions with Resources::Instance().func()
	**/
	class Resources
	{
	private:
		Resources();
		~Resources();

	public:
		static Resources& Instance() {
			static Resources resources;
			return resources;
		};

		// Create framebuffers here
		std::shared_ptr<FrameBuffer> screenQuadBuffer;
		std::shared_ptr<FrameBuffer> hdrBuffer;

		// Load shaders here (location starts at solution folder)
		std::shared_ptr<Shader> standardShader;
		std::shared_ptr<Shader> skyboxShader;
		std::shared_ptr<Shader> postProcessShader;
		std::shared_ptr<Shader> geometryShader;
		std::shared_ptr<Shader> particleRenderShader;
		std::shared_ptr<Shader> particleComputeShader;

		// Create model loader here (object files must be in separate directory)
		std::shared_ptr<Model> modelLoader;


		// Methods
		void init();
	};


	class SceneResources
	{
	private:
		SceneResources();
		~SceneResources();

	public:
		static SceneResources& Instance() {
			static SceneResources sceneResources;
			return sceneResources;
		};

		// Load textures here
		std::shared_ptr<Texture> leatherTexture;
		std::shared_ptr<Texture> floorTexture;
		std::shared_ptr<Texture> snowflakeTexture;

		// Create materials here
		std::shared_ptr<Material> singleColorMaterial;
		std::shared_ptr<Material> iceMaterial;
		std::shared_ptr<Material> leatherMaterial;
		std::shared_ptr<Material> floorMaterial;
		std::shared_ptr<Material> snowflakeMaterial;

		// Create debug cam here
		std::shared_ptr<Camera> orbitCam;

		// Create skybox here
		std::shared_ptr<Skybox> skybox;

		// Create meshes here
		std::shared_ptr<Mesh> sphere1Mesh;
		std::shared_ptr<Mesh> sphere2Mesh;
		std::shared_ptr<Mesh> sphere3Mesh;
		std::shared_ptr<Mesh> cubeMesh;
		std::shared_ptr<Mesh> floorMesh;
		std::shared_ptr<Mesh> podiumMesh;

		// Create particle systems here
		std::shared_ptr<ParticleSystem> snow;

		// Create directional lights here
		std::shared_ptr<DirectionalLight> dirLight;

		// Create point lights here
		std::vector<std::shared_ptr<PointLight>> pointLights;

		// Create spot lights here
		std::vector<std::shared_ptr<SpotLight>> spotLights;



		// Methods
		void init(std::shared_ptr<Input> input);
		std::shared_ptr<Scene> loadTestScene(std::shared_ptr<Input> input);
	};
}


//		void initSceneObjects()
//		{
//			// Load models here
//			General::modelLoader.load(nanoMan, "../assets/models/nanosuit/nanosuit.obj");
//
//			//*********************//
//			//	Intial transforms  //
//			//*********************//
//
//			// Set initial transformations here
//			sphere1->getTransform()->setLocalPos(glm::vec3(-10.0f, 10.0, 0.0));
//			sphere2->getTransform()->setLocalPos(glm::vec3(3.0f, 4.0, 0.0));
//			sphere3->getTransform()->setLocalPos(glm::vec3(1.0f, 4.0, 0.0));
//			podium->getTransform()->setLocalPos(glm::vec3(0.0f, 1.0, 0.0));
//			cube->getTransform()->setLocalPos(glm::vec3(0, 10, 6));
//			nanoMan->getTransform()->setLocalPos(glm::vec3(0, 1, 0));
//
//			//****************************//
//			//	Scene object attachments  //
//			//****************************//
//
//			// Attach meshes here
//			sphere1->addComponent(sphere1Mesh);
//			sphere2->addComponent(sphere2Mesh);
//			sphere3->addComponent(sphere3Mesh);
//			cube->addComponent(cubeMesh);
//			podium->addComponent(podiumMesh);
//			floorObj->addComponent(floorMesh);
//
//			// Attach children here
//			floorObj->addChild(podium);
//			podium->addChild(nanoMan);
//			sphere1->addChild(sphere2);
//			sphere2->addChild(sphere3);
//		}
//	}
//}
//
//
//// Test variables (for debugging only!)
//float range = 40;
//float threshold = 30;
//float step = 20;
//bool up = true;
//
//// For debugging only
//void updateScene(float deltaTime)
//{
//	// Update controls
//	Controls::update();
//
//	// Update camera
//	TestScene::orbitCam.update(deltaTime);
//
//	// Update scene here
//	if (controls.getFreezeScene() == false)
//	{
//		// Update test (For debugging purposes only!)
//		range += step * deltaTime;
//
//		if (range > 360.0f)
//			range = 0.0f;
//
//		// Update transformations here
//		nanoMan->getTransform()->setLocalRot(glm::vec3(0, range * 2.0, 0));
//		cube->getTransform()->setLocalPos(glm::vec3(0, 8 + range * 0.3, 6));
//		sphere1->getTransform()->setLocalRot(glm::vec3(0, range * 2.0, 0));
//		sphere2->getTransform()->setLocalRot(glm::vec3(range * 2.0, 0, 0));
//
//		// Update scene objects here
//		cube->update();
//		sphere1->updateAll();
//		floorObj->updateAll();
//		snow->update(deltaTime);
//	}
//}
//
//void renderScene()
//{
//	//render();
//	snow->render(orbitCam.getViewMatrix(), orbitCam.getProjMatrix());
//	skybox.render(orbitCam.getViewMatrix(), orbitCam.getProjMatrix());
//}
//
//
//void setPerFrameUniforms(std::shared_ptr<Shader> shader, Camera& camera, DirectionalLight dirLight, std::vector<std::shared_ptr<PointLight>> pointLights, std::vector<std::shared_ptr<SpotLight>> spotLights, Skybox& skybox)
//{
//	shader->use();
//

//	//***********//
//	//	Lights	 //
//	//***********//
//

//
//	// Directional light
//	dirLight.setUniforms(shader);
//
//	// Point lights
//	shader->setUniform("NR_POINT_LIGHTS", pointLights.size());
//	for (int i = 0; i < pointLights.size(); i++)
//	{
//		pointLights.at(i)->setUniforms(shader, i);
//	}
//
//	// Spot lights
//	shader->setUniform("NR_SPOT_LIGHTS", spotLights.size());
//	for (int i = 0; i < spotLights.size(); i++)
//	{
//		spotLights.at(i)->setUniforms(shader, i);
//	}
//}