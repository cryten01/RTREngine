#include "Utils.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "Skybox.h"
#include "Light.h"

#include "SceneObject.h"
#include "Transform.h"
#include "SceneComponent.h"



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


/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */
static void APIENTRY DebugCallbackDefault(GLenum source, GLenum textype, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
static std::string FormatDebugOutput(GLenum source, GLenum textype, GLuint id, GLenum severity, const char* msg);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void setPerFrameUniforms(Shader* shader, Camera& camera, DirectionalLight dirLight, std::vector<PointLight>& pointLights, std::vector<SpotLight>& spotLights);


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

	// Load shaders here (location starts at solution folder)
	std::shared_ptr<Shader> defaultShader = std::make_shared<Shader>("../assets/shader/color.vert", "../assets/shader/color.frag");
	std::shared_ptr<Shader> skyboxShader = std::make_shared<Shader>("../assets/shader/skybox.vert", "../assets/shader/skybox.frag");

	// Create textures here
	Texture leatherTexture("../assets/textures/leather.jpg", TEX_DIFFUSE);
	Texture minionTexture("../assets/textures/minion.jpg", TEX_DIFFUSE);

	// Create materials here
	std::shared_ptr<Material> blueMaterial = std::make_shared<Material>(defaultShader, glm::vec3(0.2f, 0.4f, 0.8f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	std::shared_ptr<Material> leatherMaterial = std::make_shared<TextureMaterial>(defaultShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, leatherTexture);
	std::shared_ptr<Material> minionMaterial = std::make_shared<TextureMaterial>(defaultShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, minionTexture);

	// Create geometry here
	std::shared_ptr<Mesh> sphere1Mesh = std::make_shared<Mesh>(
		Mesh::createSphereGeometry(24, 24, 0.7f), 
		blueMaterial
	);

	std::shared_ptr<Mesh> sphere2Mesh = std::make_shared<Mesh>(
		Mesh::createSphereGeometry(24, 24, 0.7f),
		blueMaterial
	);

	std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>(
		Mesh::createCubeGeometry(1.5f, 1.5f, 1.5f),
		blueMaterial
	);

	std::shared_ptr<Mesh> cylinderMesh = std::make_shared<Mesh>(
		Mesh::createCylinderGeometry(24.0f, 1.5f, 0.7f),
		blueMaterial
	);

	// Create models here (object files must be in separate directory)
	Model demoModel("../assets/models/nanosuit/nanosuit.obj", defaultShader);


	// Create scene objects here
	std::shared_ptr <SceneObject> nanoMan = std::make_shared<SceneObject>(defaultShader, glm::mat4(1));
	std::shared_ptr <SceneObject> sphere1 = std::make_shared<SceneObject>(defaultShader, glm::mat4(1));
	std::shared_ptr <SceneObject> sphere2 = std::make_shared<SceneObject>(defaultShader, glm::mat4(1));
	std::shared_ptr <SceneObject> cube = std::make_shared<SceneObject>(defaultShader, glm::mat4(1));
	std::shared_ptr <SceneObject> cylinder = std::make_shared<SceneObject>(defaultShader, glm::mat4(1));

	// Add Meshes here
	sphere1->setMesh(sphere1Mesh);
	sphere2->setMesh(sphere2Mesh);
	cube->setMesh(cubeMesh);
	cylinder->setMesh(cylinderMesh);

	// Change Position here
	cube->getTransform()->setPosition(glm::vec3(0.0f, 1.0, 0.0));
	cylinder->getTransform()->setPosition(glm::vec3(0.0f, 2.0, 0.0));
	sphere1->getTransform()->setPosition(glm::vec3(0.0f, 2.0, 0.0));
	sphere2->getTransform()->setPosition(glm::vec3(1.0f, 2.0, 0.0));


	cube->addChild(cylinder);
	cylinder->addChild(sphere1);


	// Create directional light here
	DirectionalLight dirLight(glm::vec3(1.0f), glm::vec3(0, -1, 0));

	// Create point lights here
	std::vector<PointLight> pointLights;
	pointLights.push_back(PointLight(
		glm::vec3(1.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f), 
		glm::vec3(1.0f, 0.4f, 0.1f)
	));
	pointLights.push_back(PointLight(
		glm::vec3(0.6f), 
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(1.0f,  0.4f, 0.1f)
	));

	// Create spot lights here
	std::vector<SpotLight> spotLights;
	spotLights.push_back(SpotLight(
		glm::vec3(1.0f), 
		glm::vec3(1.0f, 0.0f, 4.0f), 
		glm::vec3(1.0f, 0.4f, 0.1f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::cos(glm::radians(10.5f)),
		glm::cos(glm::radians(12.5f))
	));

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


	// For debugging only purposes only!
	float test = 0;

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		// Clear buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

			std::cout << fps << std::endl;
		}

		// Update sphere (Rotation for debugging purposes only!)
		test += 10.0f * deltaTime;
		cube->getTransform()->setRotation(glm::vec3(test, 0, 0));
		cylinder->getTransform()->setRotation(glm::vec3(test, 0, 0));
		sphere1->getTransform()->setRotation(glm::vec3(0, test, 0));
		cube->updateAll();

		// Update camera
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		orbitCam.update(int(mouse_x), int(mouse_y), _zoom, _dragging, _strafing);

		// Set per-frame uniforms
		setPerFrameUniforms(defaultShader.get(), orbitCam, dirLight, pointLights, spotLights);
	
		// Render here
		cube->renderAll();
		//cylinder->render();
		//sphere1->render();
		//sphere2->render();
		demoModel.render();
		skybox.render(skyboxShader, orbitCam.getViewMatrix(), orbitCam.getProjMatrix()); // render always last!

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


void setPerFrameUniforms(Shader* shader, Camera& camera, DirectionalLight dirLight, std::vector<PointLight>& pointLights, std::vector<SpotLight>& spotLights)
{
	shader->use();
	shader->setUniform("viewProjMatrix", camera.getProjMatrix() * camera.getViewMatrix());
	shader->setUniform("camera_world", camera.getPosition());

	shader->setUniform("dirL.color", dirLight.color);
	shader->setUniform("dirL.direction", dirLight.direction);

	shader->setUniform("NR_POINT_LIGHTS", pointLights.size());
	for (int i = 0; i < pointLights.size(); i++)
	{
		shader->setUniform("pointL[" + std::to_string(i) + "].color", pointLights[i].color);
		shader->setUniform("pointL[" + std::to_string(i) + "].position", pointLights[i].position);
		shader->setUniform("pointL[" + std::to_string(i) + "].attenuation", pointLights[i].attenuation);
	}

	shader->setUniform("NR_SPOT_LIGHTS", spotLights.size());
	for (int i = 0; i < spotLights.size(); i++)
	{
		shader->setUniform("spotL[" + std::to_string(i) + "].color", spotLights[i].color);
		shader->setUniform("spotL[" + std::to_string(i) + "].position", spotLights[i].position);
		shader->setUniform("spotL[" + std::to_string(i) + "].attenuation", spotLights[i].attenuation);
		shader->setUniform("spotL[" + std::to_string(i) + "].direction", spotLights[i].direction);
		shader->setUniform("spotL[" + std::to_string(i) + "].innerAngle", spotLights[i].innerAngle);
		shader->setUniform("spotL[" + std::to_string(i) + "].outerAngle", spotLights[i].outerAngle);
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