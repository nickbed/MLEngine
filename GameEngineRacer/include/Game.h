#ifndef GAME_H
#define GAME_H

#include <glew.h>
#include <glfw3.h>
#include "glm\glm.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "Scene\Scene.h"
#include <AntTweakBar.h>
#include "UI\GUI.h"
#include "ResourceManager.h"
#include "UI\UI.h"
#include <vector>

//#define PI 3.14159265359
//inline void TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods){TwEventMouseButtonGLFW(button, action);}
inline void TwEventMousePosGLFW3(GLFWwindow* window, double xpos, double ypos){TwMouseMotion(int(xpos), int(ypos));}
//inline void TwEventMouseWheelGLFW3(GLFWwindow* window, double xoffset, double yoffset){TwEventMouseWheelGLFW(yoffset);}
//inline void TwEventKeyGLFW3(GLFWwindow* window, int key, int scancode, int action, int mods){TwEventKeyGLFW(key, action);}
inline void TwEventCharGLFW3(GLFWwindow* window, int codepoint){TwEventCharGLFW(codepoint, GLFW_PRESS);}

class Game
{
private:
	UI ui;
	ResourceManager* rManager;
	float x1,y1,z1,rot,speed, turnSpeed;
	std::vector<Scene*> scene;
	int activeScene;
	static bool keys[1024];
	static float zoom;
	double lastCursorPositionX, lastCursorPositionY, cursorPositionX, cursorPositionY;
	GUI* gui;
	GLFWwindow* window;
	int height,width;
	void Initialise();//!< Initialises the game.
	void Update();//!< Updates the game
	void Render();//!< Renders to the screen.
	static void error_callback(int error, const char* description);//error callback
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);//key callback
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods); //Mouse callback
	static void scroll_callback(GLFWwindow *window, double x, double y);
	static void WindowSizeCB(GLFWwindow* window, int width, int height);

	void handleInput();
	bool keyPressedOnce(int key);
	//void SetZoom(float);
public:
	Game();//!< Default constructor that calls initialise function.
	~Game();
	void Run();//!< Runs the game. Calls the Update and Render.
	
};

#endif
