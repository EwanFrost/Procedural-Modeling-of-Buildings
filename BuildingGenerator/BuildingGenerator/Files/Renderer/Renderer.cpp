#include "Renderer.h"

namespace rend
{
	static bool MouseButtonDown = false;
	static double MouseLastXPos = 0.0f, MouseLastYPos = 0.0f;

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		// only start motion if the left button is pressed
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			MouseButtonDown = true;
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
			MouseButtonDown = false;

		if (!MouseButtonDown)
		{
			MouseLastXPos = 0.0f, MouseLastYPos = 0.0f;
		}
	}

	Renderer::Renderer()
	{
		Resolution.first = 1024;
		Resolution.second = 768;
		IsLastStageRendered = false;
		IsInitializing = true;

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		ProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f),
		// Camera matrix
		ViewMatrix = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		),
		// Model matrix : an identity matrix (model will be at the origin)
		ModelMatrix = glm::mat4(1.0f),
		// Our ModelViewProjection : multiplication of our 3 matrices
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Initial position : on +Z
		position = glm::vec3(-20, 8, -10);
		// Initial horizontal angle : toward -Z
		horizontalAngle = 3.14f / 3;
		// Initial vertical angle : none
		verticalAngle = 0.0f;
		// Initial Field of View
		initialFoV = 45.0f;
		speed = 3.0f; // 3 units / second
		mouseSpeed = 0.005f;
	}
	Renderer::~Renderer(){}
	void Renderer::PosAndScaleToVerteces(comn::Vector3 pos, comn::Vector3 scale, std::vector<GLfloat>& verteces, bool x, bool y, bool z)
	{
		std::vector<GLfloat> g_vertex_buffer_data = {
			pos.x, pos.y, pos.z,
			0.0f, 0.0f,
			pos.x + scale.x*(x&y || x&z), pos.y + scale.y*(y&z), pos.z,
			1.0f * (x&y || x&z), 1.0f * (y&z),
			pos.x + scale.x*x, pos.y + scale.y*y, pos.z + scale.z*z,
			1.0f, 1.0f,

			pos.x, pos.y, pos.z,
			0.0f, 0.0f,
			pos.x, pos.y + scale.y*(x&y), pos.z + scale.z*(x&z || y&z),
			1.0f*(x&y), 1.0f*(x&z || y&z),
			pos.x + scale.x*x, pos.y + scale.y*y, pos.z + scale.z*z,
			1.0f, 1.0f,
		};		

		for (size_t i = 0; i < g_vertex_buffer_data.size(); i++)
			verteces.push_back(g_vertex_buffer_data[i]);
	}
	void Renderer::PosAndScaleToVerteces(comn::Symbol* obj, std::vector<GLfloat>& verteces)
	{
		if (obj->getScale().x > 0 && obj->getScale().y > 0 && obj->getScale().z > 0)
		{
			comn::Vector3 pos = obj->getPosition();

			PosAndScaleToVerteces(obj->getPosition(), obj->getScale(), verteces, true, true, false);
			PosAndScaleToVerteces(obj->getPosition(), obj->getScale(), verteces, true, false, true);
			PosAndScaleToVerteces(obj->getPosition(), obj->getScale(), verteces, false, true, true);

			comn::Vector3 posX(obj->getPosition()), posY(obj->getPosition()), posZ(obj->getPosition());
			posX.x += obj->getScale().x;
			posY.y += obj->getScale().y;
			posZ.z += obj->getScale().z;
			PosAndScaleToVerteces(posX, obj->getScale(), verteces, false, true, true);
			PosAndScaleToVerteces(posY, obj->getScale(), verteces, true, false, true);
			PosAndScaleToVerteces(posZ, obj->getScale(), verteces, true, true, false);
		}
		else
		{
			PosAndScaleToVerteces(obj->getPosition(), obj->getScale(), verteces, 
				obj->getScale().x > 0, obj->getScale().y > 0, obj->getScale().z > 0);
		}
	}
	void Renderer::ComputeMatricesFromInputs(){

		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		// Get mouse position
		double xpos, ypos;

		if (MouseButtonDown)
		{
			glfwGetCursorPos(window, &xpos, &ypos);

			// Reset mouse position for next frame
			xpos = xpos > Resolution.first - 20 ? Resolution.first - 20 : xpos < 20 ? 20 : xpos;
			ypos = ypos > Resolution.second - 20 ? Resolution.second - 20 : ypos < 20 ? 20 : ypos;
			glfwSetCursorPos(window, xpos, ypos);

			if (MouseLastXPos == 0.0f && MouseLastYPos == 0.0f)
				MouseLastXPos = xpos, MouseLastYPos = ypos;

			// Compute new orientation
			horizontalAngle += mouseSpeed * float(MouseLastXPos - xpos);
			verticalAngle += mouseSpeed * float(MouseLastYPos - ypos);
			horizontalAngle = horizontalAngle >= 359.9f ? 0.0f : horizontalAngle;
			verticalAngle = verticalAngle >= 359.9f ? 0.0f : verticalAngle;

			MouseLastXPos = xpos, MouseLastYPos = ypos;
		}
		// Direction : Spherical coordinates to Cartesian coordinates conversion
		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
			);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
			);

		// Up vector
		glm::vec3 up = glm::cross(right, direction);
		

		// Move forward
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
			position += direction * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
			position -= direction * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
			position += right * deltaTime * speed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
			position -= right * deltaTime * speed;
		}

		float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		ViewMatrix = glm::lookAt(
			position,           // Camera is here
			position + direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
			);

		// For the next frame, the "last time" will be "now"
		lastTime = currentTime;

		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	}
	GLuint Renderer::LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
	{
		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the Vertex Shader code from the file
		std::string VertexShaderCode;
		std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
		if (VertexShaderStream.is_open())
		{
			std::string Line = "";
			while (getline(VertexShaderStream, Line))
				VertexShaderCode += "\n" + Line;
			VertexShaderStream.close();
		}

		// Read the Fragment Shader code from the file
		std::string FragmentShaderCode;
		std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
		if (FragmentShaderStream.is_open()){
			std::string Line = "";
			while (getline(FragmentShaderStream, Line))
				FragmentShaderCode += "\n" + Line;
			FragmentShaderStream.close();
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Compile Vertex Shader
		printf("Compiling shader : %s\n", vertex_file_path);
		char const * VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> VertexShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

		// Compile Fragment Shader
		printf("Compiling shader : %s\n", fragment_file_path);
		char const * FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

		// Link the program
		fprintf(stdout, "Linking program\n");
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		return ProgramID;
	}
	bool Renderer::Render(comn::Symbol* tree)
	{
		// --------------INITIALIZE GLFW ------------------------------------------------------
		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			return -1;
		}

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Open a window and create its OpenGL context
		window = glfwCreateWindow(Resolution.first, Resolution.second, "Building Generator", NULL, NULL);
		if (window == NULL){
			fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n");
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);

		// --------------INITIALIZE GLEW ------------------------------------------------------
		glewExperimental = true; // Needed for core profile
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			return -1;
		}

		fprintf(stderr, "--- USAGE ---\n\n");
		fprintf(stderr, "1. Navigation.\n");
		fprintf(stderr, "		Arrow keys.\n");
		fprintf(stderr, "		Mouse + LeftButtonPressed.\n");
		fprintf(stderr, "2. Key press actions:\n");
		fprintf(stderr, "		ESC   - close application.\n");
		fprintf(stderr, "		SPACE - next rendering stage.\n");
		fprintf(stderr, "		T     - swap textures.\n");

		// --------------INITIALIZE OTHER SETTINGS --------------------------------------------
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		// Dark blue background
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		// Create and compile our GLSL program from the shaders
		GLuint programID = LoadShaders("../BuildingGenerator/Files/Renderer/VertexShader.vertexshader",
			"../BuildingGenerator/Files/Renderer/FragmentShader.fragmentshader");			
	
		vector<GLfloat> g_vertex_buffer_data;
		vector<comn::Symbol*>queue;
		queue.push_back(tree);
		double lastTime = glfwGetTime();

		do
		{
			// ------------- SOME INITIALIZATIONS (TEXTURE, MATRIX, VERTEX)----------------------
			GLuint VertexArrayID;
			glGenVertexArrays(1, &VertexArrayID);
			glBindVertexArray(VertexArrayID);

			// Get a handle for our "MVP" uniform
			GLuint MatrixID = glGetUniformLocation(programID, "MVP");
			// Load the texture using any two methods
			GLuint Texture = loadBMP_custom("../BuildingGenerator/Files/Resources/no_texture.bmp");
			// Get a handle for our "myTextureSampler" uniform
			GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

			// --------------TREE PARSING ------------------------------------------------------
			if (IsInitializing || glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				IsInitializing = false;
				if (!IsLastStageRendered)
				{
					IsLastStageRendered = true;
					RenderingSymbols.clear();
					vector<comn::Symbol*> newqueue;


					for (size_t i = 0; i < queue.size(); i++)
					{
						RenderingSymbols.push_back(queue[i]);
						if (queue[i]->Children.size() > 0)
						{
							IsLastStageRendered = false;
							for (int j = 0; j < queue[i]->Children.size(); j++)
								newqueue.push_back(queue[i]->Children[j]);
						}
						else
							newqueue.push_back(queue[i]);
					}
					queue.clear();
					queue = newqueue;
				}				
			}

			// --------------CREATING THE ACTUAL VERTECES & TEXTURE MAPPING ---------------------
			g_vertex_buffer_data.clear();
			for (size_t i = 0; i < RenderingSymbols.size(); i++)
				PosAndScaleToVerteces(RenderingSymbols[i], g_vertex_buffer_data);

			GLuint vertexbuffer;
			glGenBuffers(1, &vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * g_vertex_buffer_data.size(), &g_vertex_buffer_data[0], GL_DYNAMIC_DRAW);

			// --------------RENDERING ----------------------------------------------------------
			do{
				// --------------------------------------------------------------------------------
				// Clear the screen
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				// Use our shader
				glUseProgram(programID);

				// --------------------------------------------------------------------------------
				// Compute the MVP matrix from keyboard and mouse input
				ComputeMatricesFromInputs();
				// Send our transformation to the currently bound shader, 
				// in the "MVP" uniform
				glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

				// --------------------------------------------------------------------------------
				// Bind our texture in Texture Unit 0
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, Texture);
				// Set our "myTextureSampler" sampler to user Texture Unit 0
				glUniform1i(TextureID, 0);

				// --------------------------------------------------------------------------------
				// 1rst attribute buffer : vertices
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
				glVertexAttribPointer(
					0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
					3,                  // size
					GL_FLOAT,           // type
					GL_FALSE,           // normalized?
					sizeof(float) * 5,  // stride
					(void*)0            // array buffer offset
					);

				// 2nd attribute buffer : UVs
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
				glVertexAttribPointer(
					1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
					2,                                // size : U+V => 2
					GL_FLOAT,                         // type
					GL_FALSE,                         // normalized?
					sizeof(float) * 5,                // stride
					(char*)(sizeof(float) * 3)        // array buffer offset
					);

				// --------------DRAW TRIANGLES ---------------------------------------------------
				glDrawArrays(GL_TRIANGLES, 0, g_vertex_buffer_data.size() / 5);

				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);

				// --------------SWAP BUFFERS -----------------------------------------------------
				glfwSwapBuffers(window);
				glfwPollEvents();

				double currentTime = glfwGetTime();
				if (float(currentTime - lastTime) > 0.2f)
				{
					lastTime = currentTime;

					if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !IsLastStageRendered)
						break;

					if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
						break;
				}

			} // Check if the ESC key was pressed or the window was closed
			while ((glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0) );

			// -------------- CLEANUP VBO & SHADER ------------------------------------------------
			glDeleteBuffers(1, &vertexbuffer);		
			glDeleteTextures(1, &TextureID);
			glDeleteVertexArrays(1, &VertexArrayID);
			
		} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0);

		// ---------------------------------------------------------------------
		glDeleteProgram(programID);
		// Close OpenGL window and terminate GLFW
		glfwTerminate();

		return 0;
	}
}