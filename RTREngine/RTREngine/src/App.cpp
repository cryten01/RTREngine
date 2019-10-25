#include "Utils.h"

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;


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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // window size fixed

	// Create a GLFWwindow object is used for GLFW's functions
	window = glfwCreateWindow(WIDTH, HEIGHT, "RTR Engine", NULL, NULL);
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
	glViewport(0, 0, WIDTH, HEIGHT);


	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// Render here
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// For debugging purposes only
		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f( 0.0f,  0.5f);
		glVertex2f( 0.5f, -0.5f);
		glEnd();

		// Swap front and back buffers
		glfwSwapBuffers(window);

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