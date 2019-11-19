#include "Utils.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "Skybox.h"
#include "Light.h"
#include "FrameBuffer.h"

#include "SceneObject.h"
#include "SceneComponent.h"
#include "Transform.h"
#include "Testing.h"




/* --------------------------------------------- */
// Global variables
/* --------------------------------------------- */

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
const char* TITLE = "RTR Engine";

// View frustum dimensions
float fov = 60.0f, nearZ = 0.1f, farZ = 100.0f;

// Camera controls
static bool _wireframe = false;
static bool _culling = true;
static bool _dragging = false;
static bool _strafing = false;
static float _zoom = 6.0f;

// Post processing effects
static bool _hdr = false;
static float _exposure = 1.0f;



/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */
static void APIENTRY DebugCallbackDefault(GLenum source, GLenum textype, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
static std::string FormatDebugOutput(GLenum source, GLenum textype, GLuint id, GLenum severity, const char* msg);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void renderScene(std::vector<std::shared_ptr<SceneObject>> renderableObjects);
void setPerFrameUniforms(Shader* shader, Camera& camera, DirectionalLight dirLight, std::vector<std::shared_ptr<PointLight>> pointLights, std::vector<std::shared_ptr<SpotLight>> spotLights);


int main(int argc, char** argv)
{
	/* --------------------------------------------- */
	// Init framework
	/* --------------------------------------------- */

	GLFWwindow* window;

	// Initialize GLFW library
	if (!glfwInit()) {
		EXIT_WITH_ERROR("Failed to init GLFW")
	}

	// Set required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // window size fixed
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Create a GLFWwindow object is used for GLFW's functions
	window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		EXIT_WITH_ERROR("Failed to create window")
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);


	// GLEW init
	glewExperimental = true;	// If true GLEW uses a modern approach for retrieving function pointers and extensions

	// Initialize GLEW to setup OpenGL function pointers
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		EXIT_WITH_ERROR("Failed to init GLEW")
	}


	// Set GL defaults here
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set callbacks here
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

#if _DEBUG
	// Register your callback function.
	glDebugMessageCallback(DebugCallbackDefault, NULL);
	// Enable synchronous callback. This ensures that your callback function is called
	// right after an error has occurred. 
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */

	// Create framebuffers here
	FrameBuffer screenQuadBuffer(WIDTH, HEIGHT, DEFAULT);
	FrameBuffer hdrBuffer(WIDTH, HEIGHT, FLOAT);

	// Load shaders here (location starts at solution folder)
	std::shared_ptr<Shader> standardShader = std::make_shared<Shader>("../assets/shader/standard.vert", "../assets/shader/standard.frag", "../assets/shader/standard.geom");
	std::shared_ptr<Shader> skyboxShader = std::make_shared<Shader>("../assets/shader/skybox.vert", "../assets/shader/skybox.frag");
	std::shared_ptr<Shader> postProcessShader = std::make_shared<Shader>("../assets/shader/framebuffer.vert", "../assets/shader/framebuffer.frag");
	std::shared_ptr<Shader> geometryShader = std::make_shared<Shader>("../assets/shader/geometry.vert", "../assets/shader/geometry.frag", "../assets/shader/geometry.geom");

	// Load textures here
	Texture leatherTexture("../assets/textures/leather.jpg", TEX_DIFFUSE);
	Texture minionTexture("../assets/textures/minion.jpg", TEX_DIFFUSE);

	// Create materials here
	std::shared_ptr<Material> singleColorMaterial = std::make_shared<Material>(standardShader, glm::vec3(0.2f, 0.4f, 0.8f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	std::shared_ptr<Material> leatherMaterial = std::make_shared<TextureMaterial>(standardShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, leatherTexture);
	std::shared_ptr<Material> minionMaterial = std::make_shared<TextureMaterial>(standardShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, minionTexture);

	// Set initial material states here
	singleColorMaterial->setState(REFLECTIVE);
	minionMaterial->setState(TEXTURE);

	// Create geometry here
	std::shared_ptr<Mesh> sphere1Mesh = std::make_shared<Mesh>(
		Mesh::createSphereGeometry(24, 24, 0.7f),
		minionMaterial
	);

	std::shared_ptr<Mesh> sphere2Mesh = std::make_shared<Mesh>(
		Mesh::createSphereGeometry(24, 24, 0.7f),
		minionMaterial
	);

	std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>(
		Mesh::createCubeGeometry(1.0f, 1.0f, 2.5f),
		singleColorMaterial
	);

	std::shared_ptr<Mesh> cylinderMesh = std::make_shared<Mesh>(
		Mesh::createCylinderGeometry(24.0f, 1.0f, 3.0f),
		singleColorMaterial
	);

	// Create directional light here
	DirectionalLight dirLight(glm::vec3(1.0f), glm::vec3(0, -1, 0));

	// Create point lights here
	std::vector<std::shared_ptr<PointLight>> pointLights;

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

	// Create spot lights here
	std::vector<std::shared_ptr<SpotLight>> spotLights;
	spotLights.push_back(std::make_shared<SpotLight>(
		glm::vec3(1.0f),
		glm::vec3(0.0f, 12.0f, 4.0f),
		glm::vec3(1.0f, 0.4f, 0.1f),
		glm::vec3(0.0f, 0.0f,-1.0f),
		glm::cos(glm::radians(10.5f)),
		glm::cos(glm::radians(12.5f))
	));

	// Create scene objects here
	std::shared_ptr <SceneObject> sphere1 = std::make_shared<SceneObject>(standardShader, glm::mat4(1));
	std::shared_ptr <SceneObject> sphere2 = std::make_shared<SceneObject>(standardShader, glm::mat4(1));
	std::shared_ptr <SceneObject> cube = std::make_shared<SceneObject>(standardShader, glm::mat4(1));
	std::shared_ptr <SceneObject> cylinder = std::make_shared<SceneObject>(standardShader, glm::mat4(1));
	std::shared_ptr <SceneObject> nanoMan = std::make_shared<SceneObject>(standardShader, glm::mat4(1));

	// Push back scene objects that should be rendered here
	std::vector<std::shared_ptr<SceneObject>> renderableObjects;
	renderableObjects.push_back(cube);
	renderableObjects.push_back(nanoMan);

	// Add meshes here
	sphere1->addMesh(sphere1Mesh);
	sphere2->addMesh(sphere2Mesh);
	cube->addMesh(cubeMesh);
	cylinder->addMesh(cylinderMesh);

	// Add children here
	cylinder->addChild(sphere1);
	cylinder->addChild(sphere2);
	nanoMan->addChild(cylinder);

	// Add initial transformations here
	sphere1->getTransform()->setLocalPos(glm::vec3(-4.0f, 4.0, 0.0));
	sphere2->getTransform()->setLocalPos(glm::vec3( 4.0f, 4.0, 0.0));
	cylinder->getTransform()->setLocalPos(glm::vec3(0.0f, -0.5, 0.0));
	cube->getTransform()->setLocalPos(glm::vec3(0, 10, 6));

	// Add lights here
	cube->setLight(pointLights.at(0));
	
	// Create model loader here (object files must be in separate directory)
	Model modelLoader;

	// Load models here
	modelLoader.load(nanoMan, "../assets/models/nanosuit/nanosuit.obj");

	// Create skybox here
	const char* skyboxTextures[] = {
		"../assets/textures/skybox/right.jpg",
		"../assets/textures/skybox/left.jpg",
		"../assets/textures/skybox/top.jpg",
		"../assets/textures/skybox/bottom.jpg",
		"../assets/textures/skybox/back.jpg",
		"../assets/textures/skybox/front.jpg"
	};
	Skybox skybox(60.0f, skyboxTextures);

	// Initialize camera here
	Camera orbitCam(fov, WIDTH/HEIGHT, nearZ, farZ);

	// Render loop variables
	float currentTime = float(glfwGetTime());
	float lastTime = float(glfwGetTime());
	float deltaTime = 0.0f;
	float runTime = 0.0f;
	int frames = 0;
	int fps = 0;
	double mouse_x, mouse_y;


	/******************************
	* Define all testing variables here
	******************************/

	// Create testing class here
	Testing test;

	test.createGeometryTest();

	float range = 30;
	float threshold = 30;
	float step = 20;
	bool up = true;


	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		/******************************
		* Do all update functions here
		******************************/

		// Compute frame time
		deltaTime = currentTime;
		currentTime = float(glfwGetTime());
		deltaTime = currentTime - deltaTime;
		runTime += deltaTime;

		// FPS counter
		frames++;
		if (currentTime - lastTime >= 1.0) {
			fps = frames;
			frames = 0;
			lastTime += 1.0;

			//std::cout << fps << std::endl;
		}

		// Update test (For debugging purposes only!)
		if (up)	{
			range += step * deltaTime;

			if (range > threshold)
				up = false;
		}

		if (!up) {
			range -= step * deltaTime;

			if (range < -threshold)
				up = true;
		}

		// Update transformations here
		nanoMan->getTransform()->setLocalRot(glm::vec3(0, range * 2.0, 0));
		cube->getTransform()->setLocalPos(glm::vec3(0, 8 + range * 0.3, 6));

		// Update scene objects here
		cube->update();
		nanoMan->updateAll();

		// Update camera
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		orbitCam.update(int(mouse_x), int(mouse_y), _zoom, _dragging, _strafing);


		//********************************//
		// Do all render functions here
		//********************************//

		// Enable if default buffer is used only!
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// First render pass (render scene into screenQuadBuffer)

		// Set per-frame uniforms
		setPerFrameUniforms(standardShader.get(), orbitCam, dirLight, pointLights, spotLights);

		// Switch to screnQuadBuffer
		//hdrBuffer.use();

		// Render scene
		renderScene(renderableObjects);
		skybox.render(skyboxShader, orbitCam.getViewMatrix(), orbitCam.getProjMatrix()); // render skybox always last!
		//test.renderGeometry(geometryShader);
		
		// Switch back to default buffer
		//hdrBuffer.unuse();



		// Second render pass (render buffer to quad)
		//hdrBuffer.renderScreenQuad(postProcessShader, _hdr, _exposure);



		// Poll events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	/* --------------------------------------------- */
	// Destroy context and exit
	/* --------------------------------------------- */

	glfwTerminate();

	return EXIT_SUCCESS;
}


void renderScene(std::vector<std::shared_ptr<SceneObject>> drawableObjects)
{
	for (std::shared_ptr<SceneObject> obj : drawableObjects)
	{
		obj->renderAll();
	}
}


void setPerFrameUniforms(Shader* shader, Camera& camera, DirectionalLight dirLight, std::vector<std::shared_ptr<PointLight>> pointLights, std::vector<std::shared_ptr<SpotLight>> spotLights)
{
	shader->use();

	//***********//
	//	Camera
	//***********//

	shader->setUniform("viewProjMatrix", camera.getProjMatrix() * camera.getViewMatrix());
	shader->setUniform("camera_world", camera.getPosition());


	//*******************//
	//	Geometry Shader
	//*******************//

	shader->setUniform("time", (float) glfwGetTime()); 
	shader->setUniform("enableGeometryShader", true);


	//***********//
	//	Lights
	//***********//

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



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		_dragging = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		_dragging = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		_strafing = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		_strafing = false;
	}
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	_zoom -= float(yoffset) * 0.5f;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{	
	// F1 - Wireframe
	// F2 - Culling
	// Esc - Exit

	if (action != GLFW_RELEASE) return;

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, true);
		break;
	case GLFW_KEY_F1:
		_wireframe = !_wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);
		break;
	case GLFW_KEY_F2:
		_culling = !_culling;
		if (_culling) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
		break;
	case GLFW_KEY_F3:
		_hdr = !_hdr;
		break;
	}
}



static void APIENTRY DebugCallbackDefault(GLenum source, GLenum textype, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) {
	if (id == 131185 || id == 131218) return; // ignore performance warnings (buffer uses GPU memory, shader recompilation) from nvidia
	std::string error = FormatDebugOutput(source, textype, id, severity, message);
	std::cout << error << std::endl;
}

static std::string FormatDebugOutput(GLenum source, GLenum textype, GLuint id, GLenum severity, const char* msg) {
	std::stringstream stringStream;
	std::string sourceString;
	std::string typeString;
	std::string severityString;

	switch (source) {
	case GL_DEBUG_SOURCE_API: {
		sourceString = "API";
		break;
	}
	case GL_DEBUG_SOURCE_APPLICATION: {
		sourceString = "Application";
		break;
	}
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
		sourceString = "Window System";
		break;
	}
	case GL_DEBUG_SOURCE_SHADER_COMPILER: {
		sourceString = "Shader Compiler";
		break;
	}
	case GL_DEBUG_SOURCE_THIRD_PARTY: {
		sourceString = "Third Party";
		break;
	}
	case GL_DEBUG_SOURCE_OTHER: {
		sourceString = "Other";
		break;
	}
	default: {
		sourceString = "Unknown";
		break;
	}
	}

	switch (textype) {
	case GL_DEBUG_TYPE_ERROR: {
		typeString = "Error";
		break;
	}
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
		typeString = "Deprecated Behavior";
		break;
	}
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
		typeString = "Undefined Behavior";
		break;
	}
	case GL_DEBUG_TYPE_PORTABILITY_ARB: {
		typeString = "Portability";
		break;
	}
	case GL_DEBUG_TYPE_PERFORMANCE: {
		typeString = "Performance";
		break;
	}
	case GL_DEBUG_TYPE_OTHER: {
		typeString = "Other";
		break;
	}
	default: {
		typeString = "Unknown";
		break;
	}
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: {
		severityString = "High";
		break;
	}
	case GL_DEBUG_SEVERITY_MEDIUM: {
		severityString = "Medium";
		break;
	}
	case GL_DEBUG_SEVERITY_LOW: {
		severityString = "Low";
		break;
	}
	default: {
		severityString = "Unknown";
		break;
	}
	}

	stringStream << "OpenGL Error: " << msg;
	stringStream << " [Source = " << sourceString;
	stringStream << ", Type = " << typeString;
	stringStream << ", Severity = " << severityString;
	stringStream << ", ID = " << id << "]";

	return stringStream.str();
}