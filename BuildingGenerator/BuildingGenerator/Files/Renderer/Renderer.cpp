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
		speed = 8.0f; // 3 units / second
		mouseSpeed = 0.005f;

		Textures["no_texture"] = 0;
	}
	Renderer::~Renderer(){}
	void Renderer::PosAndScaleToVerteces(comn::Vector3 pos, comn::Vector3 scale, std::vector<GLfloat>& verteces, bool x, bool y, bool z, float index)
	{
		GLfloat g_vertex_buffer_data[3*3*6] = {
			pos.x, pos.y, pos.z,
			0.0f, 0.0f, index,
			pos.x + scale.x*(x&y || x&z), pos.y + scale.y*(y&z), pos.z,
			1.0f * (x&y || x&z), 1.0f * (y&z), index,
			pos.x + scale.x*x, pos.y + scale.y*y, pos.z + scale.z*z,
			1.0f, 1.0f, index,

			pos.x, pos.y, pos.z,
			0.0f, 0.0f, index,
			pos.x, pos.y + scale.y*(x&y), pos.z + scale.z*(x&z || y&z),
			1.0f*(x&z || y&z), 1.0f*(x&y), index,
			pos.x + scale.x*x, pos.y + scale.y*y, pos.z + scale.z*z,
			1.0f, 1.0f, index,
		};		

		for (size_t i = 0; i < 3 * 3 * 6; i++)
			verteces.push_back(g_vertex_buffer_data[i]);
	}
	void Renderer::PosAndScaleToVerteces(comn::Symbol* obj, std::vector<GLfloat>& verteces, std::vector<GLuint>& texturesID)
	{
		std::string Texture = obj->getTexture();
		float index = 0.0f;
		if (Texture == "window")
			index = 1.0f;
		else if (Texture == "door")
			index = 2.0f;
		else if (Texture == "wall")
			index = 3.0f;

		texturesID.push_back((int)index);

		if (obj->getScale().x > 0 && obj->getScale().y > 0 && obj->getScale().z > 0)
		{
			comn::Vector3 pos = obj->getPosition();

			PosAndScaleToVerteces(obj->getPosition(), obj->getScale(), verteces, true, true, false, index);
			PosAndScaleToVerteces(obj->getPosition(), obj->getScale(), verteces, true, false, true, index);
			PosAndScaleToVerteces(obj->getPosition(), obj->getScale(), verteces, false, true, true, index);

			comn::Vector3 posX(obj->getPosition()), posY(obj->getPosition()), posZ(obj->getPosition());
			posX.x += obj->getScale().x;
			posY.y += obj->getScale().y;
			posZ.z += obj->getScale().z;
			PosAndScaleToVerteces(posX, obj->getScale(), verteces, false, true, true, index);
			PosAndScaleToVerteces(posY, obj->getScale(), verteces, true, false, true, index);
			PosAndScaleToVerteces(posZ, obj->getScale(), verteces, true, true, false, index);
		}
		else
		{
			PosAndScaleToVerteces(obj->getPosition(), obj->getScale(), verteces, 
				obj->getScale().x > 0, obj->getScale().y > 0, obj->getScale().z > 0, index);
		}
	}
	void Renderer::ParseTreeForTextures(comn::Symbol* tree)
	{
		Textures["window"] = 1;
		Textures["door"] = 2;
		Textures["wall"] = 3;
		/*std::vector<comn::Symbol*> queue;
		queue.push_back(tree);

		while (queue.size()){
			comn::Symbol* cur_item = queue[0];
			queue.erase(queue.begin());
			map<std::string, int>::iterator it = Textures.find(cur_item->getTexture());
			if (it == Textures.end())
				Textures[cur_item->getTexture()] == Textures.size();

			for (int i = 0; i < cur_item->Children.size(); i++){
				comn::Symbol *child = cur_item->Children[i];
				queue.push_back(child);
			}
		}*/
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
	void Renderer::ActivateTextures(int index)
	{
		switch (index)
		{
		case 0:glActiveTexture(GL_TEXTURE0);break;
		case 1:glActiveTexture(GL_TEXTURE1); break;
		case 2:glActiveTexture(GL_TEXTURE2); break;
		case 3:glActiveTexture(GL_TEXTURE3); break;
		case 4:glActiveTexture(GL_TEXTURE4); break;
		case 5:glActiveTexture(GL_TEXTURE5); break;
		case 6:glActiveTexture(GL_TEXTURE6); break;
		case 7:glActiveTexture(GL_TEXTURE7); break;
		case 8:glActiveTexture(GL_TEXTURE8); break;
		case 9:glActiveTexture(GL_TEXTURE9); break;
		default:glActiveTexture(GL_TEXTURE0); break;
		}
	}
	void Renderer::SetTextures(std::map<int, GLuint>& TextureBMP, std::map<int, GLuint>& TextureID)
	{		
		for (int i = 0; i < TextureID.size(); i++)
		{
			// Bind our texture in Texture Unit i
			ActivateTextures(i);
			glBindTexture(GL_TEXTURE_2D, TextureBMP[i]);
			// Set our "TextureSampler" sampler to user Texture Unit i
			glUniform1i(TextureID[i], i);
		}		
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
			return false;
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
			return false;
		}
		glfwMakeContextCurrent(window);

		// --------------INITIALIZE GLEW ------------------------------------------------------
		glewExperimental = true; // Needed for core profile
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			return false;
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
		// Parse all elements to find all the neccessary textures
		ParseTreeForTextures(tree);
		// Create and compile our GLSL program from the shaders
		GLuint programID = LoadShaders("../BuildingGenerator/Files/Renderer/VertexShader.vertexshader",
			"../BuildingGenerator/Files/Renderer/FragmentShader.fragmentshader");
			
	
		vector<GLfloat> g_vertex_buffer_data;
		vector<GLuint> g_vertex_texture_buffer_data;
		vector<comn::Symbol*>queue;
		queue.push_back(tree);
		double lastTime = glfwGetTime();
		std::map<int, GLuint> TextureBMP;
		std::map<int, GLuint> TextureID;
		int index = -1;

		do
		{
			// ------------- SOME INITIALIZATIONS (TEXTURE, MATRIX, VERTEX)----------------------
			GLuint VertexArrayID;
			glGenVertexArrays(1, &VertexArrayID);
			glBindVertexArray(VertexArrayID);

			// Get a handle for our "MVP" uniform
			GLuint MatrixID = glGetUniformLocation(programID, "MVP");

			if (IsInitializing || glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
			{
				index++;
				// Load the texture using any two methods
				std::map<std::string, int>::iterator iter;
				for (iter = Textures.begin(); iter != Textures.end(); ++iter)
				{
					// WARNING! improper filePath, texture naming will cause errors !!!
					std::string TextureMix = (index % 2 == 0 ? "1" : "2");
					std::string FilePath = "../BuildingGenerator/Files/Resources/TextureMix" + TextureMix + "/" + iter->first + ".bmp";
					TextureBMP[iter->second] = loadBMP_custom(FilePath.c_str());

					std::string TextureSmapler = "TextureSampler" + std::to_string(static_cast<long long>(iter->second));// std::to_string(iter->second);
					TextureID[iter->second] = glGetUniformLocation(programID, TextureSmapler.c_str());

					//GLuint Texture = loadBMP_custom("../BuildingGenerator/Files/Resources/no_texture.bmp");
					// Get a handle for our "myTextureSampler" uniform
					//GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
				}
			}

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
				PosAndScaleToVerteces(RenderingSymbols[i], g_vertex_buffer_data, g_vertex_texture_buffer_data);

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
				SetTextures(TextureBMP, TextureID);

				// --------------------------------------------------------------------------------
				// 1rst attribute buffer : vertices
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
				glVertexAttribPointer(
					0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
					3,                  // size
					GL_FLOAT,           // type
					GL_FALSE,           // normalized?
					sizeof(float) * 6,  // stride
					(void*)0            // array buffer offset
					);

				// 2nd attribute buffer : UVs
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
				glVertexAttribPointer(
					1,                                // attribute.
					3,                                // size : U+V + textureIndex => 3
					GL_FLOAT,                         // type
					GL_FALSE,                         // normalized?
					sizeof(float) * 6,                // stride
					(char*)(sizeof(float) * 3)        // array buffer offset
					);

				// --------------DRAW TRIANGLES ---------------------------------------------------
				glDrawArrays(GL_TRIANGLES, 0, g_vertex_buffer_data.size() / 6);

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
			for (size_t i = 0; i < TextureID.size(); i++)
			{
				glDeleteTextures(1, &TextureID[i]);
			}			
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