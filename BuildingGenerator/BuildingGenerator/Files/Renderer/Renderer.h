#ifndef RENDER_H
#define RENDER_H

#include "..\Commons\Commons.h"
#include "..\Source.h"
<<<<<<< HEAD
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <GLFW\glfw3.h>
#include <algorithm>
#include <fstream>
#include <glm.hpp>
#include <matrix_transform.hpp> //translate, rotate, scale, perspective 
#include <type_ptr.hpp> //value_ptr
#include <iostream>
=======
#include<GL\glew.h>
#include<GL\freeglut.h>
#include<iostream>
>>>>>>> 0bccdc6449e680e5c82b40141274fcec35ca960f
#include <vector>
#include <string>

namespace rend
{

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

	class Renderer
	{
	private:
<<<<<<< HEAD
		// --- Variables ----------------------------------------
		GLFWwindow* window;
		std::pair<int, int> Resolution;
		bool IsLastStageRendered = false;
		vector<comn::Symbol*> RenderingSymbols;
		bool IsInitializing = true;

		// ------------------ MATRIX & NAVIGATION VARS -----------
		glm::mat4
			// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
			ProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f),
			// Camera matrix
			ViewMatrix = glm::lookAt(
			glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
			),
			// Model matrix : an identity matrix (model will be at the origin)
			ModelMatrix = glm::mat4(1.0f),
			// Our ModelViewProjection : multiplication of our 3 matrices
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Initial position : on +Z
		glm::vec3 position = glm::vec3(-20, 8, -10);
		// Initial horizontal angle : toward -Z
		float horizontalAngle = 3.14f / 3;
		// Initial vertical angle : none
		float verticalAngle = 0.0f;
		// Initial Field of View
		float initialFoV = 45.0f;
		float speed = 3.0f; // 3 units / second
		float mouseSpeed = 0.005f;

		// --- Methods --------------------------------------------
		void static _initRendering() {
			//Makes 3D drawing work when something is in front of something else
			glEnable(GL_DEPTH_TEST);
		}
		GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
		GLuint loadBMP_custom(const char * imagepath){
=======
		// --- Variables ----------------------
		bool GlewErrorOccoured;
>>>>>>> 0bccdc6449e680e5c82b40141274fcec35ca960f

			//printf("Reading image %s\n", imagepath);

			// Data read from the header of the BMP file
			unsigned char header[54];
			unsigned int dataPos;
			unsigned int imageSize;
			unsigned int width, height;
			// Actual RGB data
			unsigned char * data;

			// Open the file
			FILE * file = fopen(imagepath, "rb");
			if (!file)							    { printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0; }

			// Read the header, i.e. the 54 first bytes

			// If less than 54 bytes are read, problem
			if (fread(header, 1, 54, file) != 54){
				printf("Not a correct BMP file\n");
				return 0;
			}
			// A BMP files always begins with "BM"
			if (header[0] != 'B' || header[1] != 'M'){
				printf("Not a correct BMP file\n");
				return 0;
			}
			// Make sure this is a 24bpp file
			if (*(int*)&(header[0x1E]) != 0)         { printf("Not a correct BMP file\n");    return 0; }
			if (*(int*)&(header[0x1C]) != 24)         { printf("Not a correct BMP file\n");    return 0; }

			// Read the information about the image
			dataPos = *(int*)&(header[0x0A]);
			imageSize = *(int*)&(header[0x22]);
			width = *(int*)&(header[0x12]);
			height = *(int*)&(header[0x16]);

			// Some BMP files are misformatted, guess missing information
			if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
			if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

			// Create a buffer
			data = new unsigned char[imageSize];

			// Read the actual data from the file into the buffer
			fread(data, 1, imageSize, file);

			// Everything is in memory now, the file wan be closed
			fclose(file);

			// Create one OpenGL texture
			GLuint textureID;
			glGenTextures(1, &textureID);

			// "Bind" the newly created texture : all future texture functions will modify this texture
			glBindTexture(GL_TEXTURE_2D, textureID);

			// Give the image to OpenGL
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

			// OpenGL has now copied the data. Free our own version
			delete[] data;

			// Poor filtering, or ...
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

			// ... nice trilinear filtering.
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);

			// Return the ID of the texture we just created
			return textureID;
		}
		GLuint loadDDS(const char * imagepath){

			unsigned char header[124];

			FILE *fp;

			/* try to open the file */
			fp = fopen(imagepath, "rb");
			if (fp == NULL){
				printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
				return 0;
			}

			/* verify the type of file */
			char filecode[4];
			fread(filecode, 1, 4, fp);
			if (strncmp(filecode, "DDS ", 4) != 0) {
				fclose(fp);
				return 0;
			}

			/* get the surface desc */
			fread(&header, 124, 1, fp);

			unsigned int height = *(unsigned int*)&(header[8]);
			unsigned int width = *(unsigned int*)&(header[12]);
			unsigned int linearSize = *(unsigned int*)&(header[16]);
			unsigned int mipMapCount = *(unsigned int*)&(header[24]);
			unsigned int fourCC = *(unsigned int*)&(header[80]);


			unsigned char * buffer;
			unsigned int bufsize;
			/* how big is it going to be including all mipmaps? */
			bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
			buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
			fread(buffer, 1, bufsize, fp);
			/* close the file pointer */
			fclose(fp);

			unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
			unsigned int format;
			switch (fourCC)
			{
			case FOURCC_DXT1:
				format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				break;
			case FOURCC_DXT3:
				format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				break;
			case FOURCC_DXT5:
				format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				break;
			default:
				free(buffer);
				return 0;
			}

			// Create one OpenGL texture
			GLuint textureID;
			glGenTextures(1, &textureID);

			// "Bind" the newly created texture : all future texture functions will modify this texture
			glBindTexture(GL_TEXTURE_2D, textureID);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
			unsigned int offset = 0;

			/* load the mipmaps */
			for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
			{
				unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
				glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
					0, size, buffer + offset);

				offset += size;
				width /= 2;
				height /= 2;

				// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
				if (width < 1) width = 1;
				if (height < 1) height = 1;

			}

			free(buffer);

			return textureID;
		}
<<<<<<< HEAD
		void static PosAndScaleToVerteces(comn::Symbol* obj, std::vector<GLfloat>& verteces);
		void static PosAndScaleToVerteces(comn::Vector3 pos, comn::Vector3 scale, std::vector<GLfloat>& verteces, bool x, bool y, bool z);
		void ComputeMatricesFromInputs();			

=======
		//Initializes 3D rendering
		void static _initRendering() {
			//Makes 3D drawing work when something is in front of something else
			glEnable(GL_DEPTH_TEST);
		}
>>>>>>> 0bccdc6449e680e5c82b40141274fcec35ca960f
	public:
		Renderer(int argc, char** argv);
		~Renderer();
<<<<<<< HEAD
		bool Render(comn::Symbol* tree);
=======
		void Render(comn::Node tree);
		
>>>>>>> 0bccdc6449e680e5c82b40141274fcec35ca960f
	};
}

#endif