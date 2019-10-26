#include "Utils.h"
#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "Geometry.h"

/* --------------------------------------------- */
// Global variables
/* --------------------------------------------- */

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
const char* TITLE = "RTR Engine";

// View frustum dimensions
float fov = 60.0f, nearZ = 0.1f, farZ = 100.0f;

// Camera controls
static bool _dragging = false;
static bool _strafing = false;
static float _zoom = 6.0f;


/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void setPerFrameUniforms(Shader* shader, Camera& camera);


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

	// GL defaults
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);


	// Set callbacks here
	glfwSetKeyCallback(window, key_callback);

	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */

	// Load shaders here
	std::shared_ptr<Shader> colorShader = std::make_shared<Shader>("RTREngine/assets/shader/color.vert", "RTREngine/assets/shader/color.frag");

	// Create materials here
	std::shared_ptr<Material> blueMaterial = std::make_shared<Material>(colorShader, glm::vec3(0.0f, 0.0f, 1.0f));

	// Create geometry here
	GeometryData data = Geometry::createSphereGeometry(16, 8, 0.35f);
	Geometry sphere = Geometry(glm::mat4(1.0f), data, blueMaterial);

	// Initialize camera here
	Camera camera(fov, WIDTH/HEIGHT, nearZ, farZ);

	// Render loop variables
	float currentTime = float(glfwGetTime());
	float deltaTime = 0.0f;
	float runTime = 0.0f;
	double mouse_x, mouse_y;

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

		// Update camera
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		camera.update(int(mouse_x), int(mouse_y), _zoom, _dragging, _strafing);

		// Set per-frame uniforms
		setPerFrameUniforms(colorShader.get(), camera);

		// Render here
		sphere.draw();

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


void setPerFrameUniforms(Shader* shader, Camera& camera)
{
	shader->use();
	shader->setUniform("viewProjMatrix", camera.getViewProjectionMatrix());
	shader->setUniform("camera_world", camera.getPosition());
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	// esc - Exit
	if (action != GLFW_RELEASE)
		return;

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, true);
		break;
	}
}