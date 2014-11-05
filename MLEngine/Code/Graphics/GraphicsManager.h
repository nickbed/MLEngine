#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include "include\gl_core_4_3.hpp"
#include "GLFW\glfw3.h"
#include <map>

class GraphicsManager
{
public:
	GraphicsManager();
	~GraphicsManager();
	//TODO - Add another constructor here

	//Init OpenGL Windowing engine
	bool Init();

	//Create window to render stuff in and make it current
	bool CreateGraphicsWindow(const int xSize, const int ySize, const char* windowTitle);

	//Draw window, poll events and swap buffers. Returns false if window should be closing
	bool DrawAndUpdateWindow();

	bool LoadShader(const char* shaderName, const char* shaderPath);
	const int GetShaderID(const char* shaderName);

	//Getters
	GLFWwindow* GetCurrentWindow();
	const int GetXSize();
	const int GetYSize();

	//Setters


private:

	//This class will store the current window
	GLFWwindow* currentWindow;

	//Width and height of window in pixels
	int xSize;
	int ySize;

	//Map to store shaders and their locations
	std::map<char*, int> shaderMap;
};


#endif