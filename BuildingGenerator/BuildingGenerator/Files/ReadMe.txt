-----------------------------------------------
--- Project Structure -------------------------
-----------------------------------------------
1. Commons   -> includes all *.h / *.cpp files using the namespace <"comn"> that have common functionality or data types neccessary for all the other files.
2. Parsing   -> includes all *.h / *.cpp files using the namespace <"pars"> that handle the parsing part of the project.
3. Modeling  -> includes all *.h / *.cpp files using the namespace <"modl"> that handle the modeling part of the project.
4. Renderer  -> includes all *.h / *.cpp files using the namespace <"rend"> that handle the rendering part of the project.

Source.cpp  -> includes the main function that uses classes from those namspaces to start the project
Source.h    -> constains a #define USE_TEST_DATA that is set if we want to use hardcoded samples from Parers -> Modeling -> Rendering


-----------------------------------------------
--- Directory Structure -----------------------
-----------------------------------------------
Project Directory\Files\
Folders:
	@ Commons
	@ Modeler
	@ Parser
	@ Renderer
	@ Resources
Files:
	@ Source.cpp/.h -> includes the main function
	@ Rules.txt & ReadMe.txt


-----------------------------------------------
--- Project Settings in Visual Studio ---------
-----------------------------------------------
Enter the Properties of the Project BuildingGenerator

1. C/C++  -> General -> Additional Include Directories
	@ add ..\project\libs\glew-1.12.0\include
	@ add ..\project\libs\freeglut\include
	@ add ..\project\libs\glfw-3.1.bin.WIN32\include
	@ add ..\project\libs\glm\

1. Linker -> General -> Additional Library Directories
	@ add ..\project\libs\glew-1.12.0\lib\Release\Win32
	@ add ..\project\libs\freeglut\lib
	@ add ..\project\libs\glfw-3.1.bin.WIN32\lib-vc2013

3. Linker -> Input -> Additional Dependencies
	@ add glew32.lib
	@ add freeglut.lib
	@ add glfw3.lib