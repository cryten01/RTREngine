#include "Utils.h"
#include "Shader.h"

/* --------------------------------------------- */
// Global variables
/* --------------------------------------------- */

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
const char* TITLE = "RTR Engine";


/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);



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
	glClearColor(0, 1, 0, 0);


	// Set callbacks here
	glfwSetKeyCallback(window, key_callback);

	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */

	// Load shaders here
	std::shared_ptr<Shader> colorShader = std::make_shared<Shader>("Assets/Shader/color.vert", "Assets/Shader/color.frag");

	// Create materials here
	// Create geometry here



	// Render loop variables
	float currentTime = float(glfwGetTime());
	float deltaTime = 0.0f;
	float runTime = 0.0f;

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


		// Set per-frame uniforms here
		// Render here

		// For debugging purposes only
		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f( 0.0f,  0.5f);
		glVertex2f( 0.5f, -0.5f);
		glEnd();

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