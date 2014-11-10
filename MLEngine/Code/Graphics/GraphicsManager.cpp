#include "GraphicsManager.h"



GraphicsManager::GraphicsManager()
{
	currentWindow = nullptr;
}

GraphicsManager::~GraphicsManager()
{
	glfwTerminate();
}

bool GraphicsManager::Init()
{
	
	int result = glfwInit();
	if(result != GL_TRUE)
	{
		glfwTerminate();
		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_ERROR, "Failed to init GLFW");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GL_DOUBLEBUFFER, GL_TRUE);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	//// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);
	
	//Initialize clear color
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Add a camera
	currentCamera = new CameraEntity();
	glm::vec3 newCameraPos = glm::vec3(0.0, 0.0, 5.0);
	currentCamera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));

	return true;
}

bool GraphicsManager::CreateGraphicsWindow(const int xSize, const int ySize, const char* windowTitle)
{
	NULLPTRCHECK(windowTitle, "Null pointer on window title")
	bool success = true;
	success &= mauveassert::Assert::AssertTrue("Can't create a window when a window already exists!",(currentWindow == nullptr), mauveassert::ENUM_severity::SEV_ERROR);
	success &= mauveassert::Assert::AssertTrue("Invalid X window size specified",(xSize > 1), mauveassert::ENUM_severity::SEV_ERROR);
	success &= mauveassert::Assert::AssertTrue("Invalid Y window size specified",(ySize > 1), mauveassert::ENUM_severity::SEV_ERROR);
	success &= mauveassert::Assert::AssertTrue("Null window title specified",(windowTitle != nullptr), mauveassert::ENUM_severity::SEV_ERROR);
	if(!success) return false;



	
	//Create window
	currentWindow = glfwCreateWindow(xSize, ySize, windowTitle, NULL, NULL);
	success &= mauveassert::Assert::AssertTrue("Window creation failed",(currentWindow != nullptr), mauveassert::ENUM_severity::SEV_ERROR);
	
	//Set up the context
	glfwMakeContextCurrent(currentWindow);
	glewExperimental = GL_TRUE;
	int glewresult = glewInit();

	//Use default frag and vert shader
	currentShader = mauveresource::ResourceManager::GetResource<Shader>("data\\shaders\\default");

	//TODO - do something with this
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	if(!success) return false;
	return true;
}

bool GraphicsManager::DrawAndUpdateWindow(std::vector<IEntity*> entities, float dt)
{
	glfwSwapBuffers(currentWindow);
	glfwPollEvents();
	
	glClear(GL_COLOR_BUFFER_BIT);
	for (auto& x: entities)
	{
		DrawEntity(x);
	}

	return true;
}

void GraphicsManager::DrawEntity(IEntity* ent)
{
	//Get all instances of graphics components
	std::vector<IComponent*> graphicsComponents = ent->Components->GetComponentsOfType("testGraphics");
	for(auto& y: graphicsComponents)
	{
		RenderComponents((ThreeDGraphics*)y, ent->Transform);
	}
}

void GraphicsManager::RenderComponents(ThreeDGraphics* componentToRender, TransformComponent* modelTransform)
{
	//Set up model matrix
	glm::mat4 modelMat(1.0f);
	modelMat *= glm::translate(glm::mat4(1.0f), modelTransform->GetPosition());
	modelMat *= glm::scale(glm::mat4(1.0f), modelTransform->GetScale());
	//TODO - ROTATE MATRIX IMPLEMENTATION
	
	

	//Upload matrix to the shader
	currentShader->SendUniformMat4("modelmatrix", modelMat);
	currentShader->SendUniformMat4("viewprojmatrix", currentCamera->GetViewProjMatrix());

	currentShader->UseShader();

	GLuint vertarray;
	GLuint colourarray;

	// Create and set-up the vertex array object
	//glGenVertexArrays(1, &vertarray);

	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, componentToRender->GetVerticesID());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glDrawArrays(GL_TRIANGLES, 0, componentToRender->GetVertices().size());


	glEnableVertexAttribArray(1);  // Vertex colour

	glBindBuffer(GL_ARRAY_BUFFER, componentToRender->GetColoursID());
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);


	glDisableVertexAttribArray(0);
}

GLFWwindow* GraphicsManager::GetCurrentWindow()
{
	return currentWindow;
}

const int GraphicsManager::GetXSize()
{
	return xSize;
}

const int GraphicsManager::GetYSize()
{
	return ySize;
}

CameraEntity* GraphicsManager::GetCurrentCamera()
{
	return currentCamera;
}

void GraphicsManager::SetCurrentCamera(CameraEntity* cam)
{
	currentCamera = cam;
}

