#include "Game.h"

float Game::zoom;
bool Game::keys[1024]= {false};

Game::Game():activeScene(0)

{
	scene.push_back(new Scene());
	gui = new GUI();
	rManager = ResourceManager::getInstance();
}

Game::~Game()
{
	delete gui;
	gui = NULL;
	for(unsigned int i=0; i < scene.size(); ++i)
	{
		delete scene[i];
		scene[i] = NULL;
	}
}
void Game::Run()
{
	Initialise();
	glfwSetTime(0.0);
	double refresh_rate = 1.0/60.0;
	//! run the program as long as the window is open
	while (!glfwWindowShouldClose(window))
	{


		handleInput();
		if (glfwGetTime()  > refresh_rate)
		{
			glfwSetTime(0.0);
			Update();
			Render();
		}



	}
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
void Game::Initialise()
{

	width = 1024;
	height = 768;




	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	window = glfwCreateWindow(width, height, "Mauve Editor", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window,WindowSizeCB);
	glfwSetMouseButtonCallback(window,mouse_button_callback);
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW3);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW3);
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	printf("OpenGL version (%s): \n", glGetString(GL_VERSION));
	
	if(!rManager->loadDefaults())
	{
		exit(EXIT_FAILURE);
	}
	//scene[activeScene]->InitScene("data\\Scene\\demolevel.scn");
	scene[activeScene]->InitScene("");

	ui.initText2D();


	gui->setup(width,height, scene[activeScene]);


}

void Game::error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
void Game::Update()
{
	if(keys[GLFW_KEY_LEFT_ALT])
	{
		scene[activeScene]->GetCamera()->zoom(zoom);
		zoom = 0;
		glfwGetCursorPos(window,&cursorPositionX,&cursorPositionY);

		//See how much the cursor has moved
		float deltaX = (float)(lastCursorPositionX - cursorPositionX);
		float deltaY = (float)(lastCursorPositionY - cursorPositionY);

		//Using a different way (i.e. instead of callback) to check for LEFT mouse button
		if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) )
		{
			//std::cout << "Left button \n";
			//Rotate the camera. The 0.001f is a velocity mofifier to make the speed sensible
			scene[activeScene]->GetCamera()->rotate(deltaX*0.001f, deltaY*0.001f);

		}

		//Using a different way (i.e. instead of callback) to check for RIGHT mouse button
		if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) )
		{
			//std::cout << "Right button \n";
			//Rotate the camera. The 0.01f is a velocity mofifier to make the speed sensible
			scene[activeScene]->GetCamera()->pan(deltaX*0.01f, deltaY*0.01f);

		}
	}
	double lastX = x, lastY = y;
	
	glfwGetCursorPos(window,&x,&y);
	/*if(lastX != x && lastY != y)
	{
		std::cout << x << "  " << y << std::endl;
	}*/
	
	scene[activeScene]->Update(keys);
	gui->checkEnums(scene,activeScene);
	gui->update(scene[activeScene]);
	//Store the current cursor position
	lastCursorPositionX = cursorPositionX;
	lastCursorPositionY = cursorPositionY;


}
void Game::Render()
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glClearColor(0.0f,0.5f,0.7f,1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	scene[activeScene]->Render();





	ui.printText2D("EDITOR",20,20,35);

	gui->draw();

	glfwSwapBuffers(window);
	glfwPollEvents();

}

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)//key callback
{
	if(!TwEventKeyGLFW(key, action))
	{

	}
	glfwSetKeyCallback(window,key_callback);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	keys[key] = action;

}

void Game::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(!TwEventMouseButtonGLFW(button,action))
	{
		//std::cout << "hello/n";
	}
	glfwSetMouseButtonCallback(window,mouse_button_callback);
}

void Game::scroll_callback(GLFWwindow *window, double x, double y)
{
	zoom = (float)y*2.5f;
}
void Game::handleInput()
{

	if(keyPressedOnce(GLFW_KEY_N))
	{
		scene[activeScene]->nextCamera();
	}

}
bool Game::keyPressedOnce(int key)
{
	static bool pressed = false;
	if(keys[key] == GLFW_PRESS && !pressed)
	{
		pressed = true;
		return true;
	}
	else if(keys[key] == GLFW_RELEASE )
	{
		pressed = false;
		return false;	
	}
	else return false;
}

void Game::WindowSizeCB(GLFWwindow* window, int width, int height){
	// Set OpenGL viewport and camera
	glViewport(0, 0, width, height);
	// Send the new window size to AntTweakBar
	TwWindowSize(width, height);

}