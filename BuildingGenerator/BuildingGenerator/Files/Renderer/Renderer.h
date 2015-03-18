#ifndef RENDER_H
#define RENDER_H

#include "..\Commons\Commons.h"
#include "..\Source.h"
#include<GL\glew.h>
#include<GL\freeglut.h>
#include<GLFW\glfw3.h>
#include<iostream>
#include <vector>
#include <string>

namespace rend
{
	class Renderer
	{
	private:
		// --- Variables ----------------------
		GLFWwindow* window;
		bool isInitialized;

		// --- Methods ------------------------

		// any time the window is resized the function gets called. It's set up to 
		// the function "glutReshapeFunction" in main.
		void static _changeViewport(int w, int h)
		{
			glViewport(0, 0, w, h);

			glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

			//Set the camera perspective
			glLoadIdentity(); //Reset the camera
			gluPerspective(45.0,                  //The camera angle
				(double)w / (double)h, //The width-to-height ratio
				1.0,                   //The near z clipping coordinate
				200.0);                //The far z clipping coordinate
		}
		// here is the function that gets called each time the window needs to redraw.
		// it is the "paint" method for our program, and is set up for the "glutDisplauFunction" in main
		void static _draw()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
			glLoadIdentity(); //Reset the drawing perspective

			glBegin(GL_QUADS); //Begin quadrilateral coordinates

			//Trapezoid
			glVertex3f(-0.7f, -1.5f, -5.0f);
			glVertex3f(0.7f, -1.5f, -5.0f);
			glVertex3f(0.4f, -0.5f, -5.0f);
			glVertex3f(-0.4f, -0.5f, -5.0f);

			glEnd(); //End quadrilateral coordinates

			glBegin(GL_TRIANGLES); //Begin triangle coordinates

			//Pentagon
			glVertex3f(0.5f, 0.5f, -5.0f);
			glVertex3f(1.5f, 0.5f, -5.0f);
			glVertex3f(0.5f, 1.0f, -5.0f);

			glVertex3f(0.5f, 1.0f, -5.0f);
			glVertex3f(1.5f, 0.5f, -5.0f);
			glVertex3f(1.5f, 1.0f, -5.0f);

			glVertex3f(0.5f, 1.0f, -5.0f);
			glVertex3f(1.5f, 1.0f, -5.0f);
			glVertex3f(1.0f, 1.5f, -5.0f);

			//Triangle
			glVertex3f(-0.5f, 0.5f, -5.0f);
			glVertex3f(-1.0f, 1.5f, -5.0f);
			glVertex3f(-1.5f, 0.5f, -5.0f);

			glEnd(); //End triangle coordinates

			glutSwapBuffers();
		}
		// Called when a key is pressed
		// The key that was pressed & The current mouse coordinates
		void static _handleKeypress(unsigned char key, int x, int y)
		{
			switch (key) {
			case 27: //Escape key
				exit(0); //Exit the program
			}
		}
		//Initializes 3D rendering
		void static _initRendering() {
			//Makes 3D drawing work when something is in front of something else
			glEnable(GL_DEPTH_TEST);
		}
		void Update();
		bool Initialize();
	public:
		Renderer();
		~Renderer();
		bool IsInitialized() { return isInitialized; }
		void Render(comn::Node tree);
		
	};
}

#endif