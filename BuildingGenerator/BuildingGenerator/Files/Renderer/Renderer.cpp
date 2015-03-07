#include "Renderer.h"

namespace rend
{
	Renderer::Renderer(int argc, char** argv) {
		GlewErrorOccoured = false;

		// Initialize GLUT
		glutInit(&argc, argv);
		//set up some memory buffers for our display
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		//set the window size
		glutInitWindowSize(800, 600);
		// create the window size
		glutCreateWindow("Building Generator");
		// Bind the two functions above to respond when neccessary
		_initRendering();

		glutReshapeFunc(_changeViewport);
		glutDisplayFunc(_draw);
		glutKeyboardFunc(_handleKeypress);

		// Very important! this initializes the entry points in the OpenGL driver so we can
		// call all the functions in the API
		if (GLEW_OK != glewInit())
		{
			fprintf(stderr, "GLEW error");
			GlewErrorOccoured = true;
		}
	}
	Renderer::~Renderer(){};
	void Renderer::Render(comn::Node tree)
	{
		// uses the Tree to render the objects on the display

		glutMainLoop(); //Start the main loop.  glutMainLoop doesn't return.
	}
}