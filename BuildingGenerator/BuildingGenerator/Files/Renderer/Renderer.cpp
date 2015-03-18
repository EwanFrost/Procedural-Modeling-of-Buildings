#include "Renderer.h"

namespace rend
{
	Renderer::Renderer() 
	{		
		isInitialized = Initialize();
	}
	Renderer::~Renderer(){}
	void Renderer::Update()
	{
		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

		do{
			// Draw nothing, see you in tutorial 2 !

			// Swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();

		} // Check if the ESC key was pressed or the window was closed
		while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
	}
	bool Renderer::Initialize()
	{
		// Initialise GLFW
		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			return false;
		}

		glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

		// Open a window and create its OpenGL context 
		window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
		if (window == NULL){
			fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window); // Initialize GLEW 
		glewExperimental = true; // Needed in core profile 
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			return false;
		}
		return true;
	}
	void Renderer::Render(comn::Symbol* tree)
	{
		if (!isInitialized)
			return;
		
		// uses the Tree to render the objects on the display

		Update();
		//glutMainLoop(); //Start the main loop.  glutMainLoop doesn't return.
	}
}