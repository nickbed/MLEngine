#include "GraphicsManager.h"



GraphicsManager::GraphicsManager()
{
	currentWindow = nullptr;
}

GraphicsManager::~GraphicsManager()
{
	glfwTerminate();
}

bool GraphicsManager::Init(int GLVersionMajor, int GLVersionMinor)
{
	
	int result = glfwInit();
	if(result != GL_TRUE)
	{
		glfwTerminate();
		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_FATAL, "Failed to init GLFW");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLVersionMinor);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GL_DOUBLEBUFFER, GL_TRUE);

	

	//Initialize clear color
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Add a camera
	currentCamera = new CameraEntity();
	glm::vec3 newCameraPos = glm::vec3(0.0, 5.0, 50.0);
	currentCamera->SetPosition(glm::vec3(0.0f, 3.0f, 3.0f));

	char GLVersionString[10]; 
	sprintf_s(GLVersionString, "v%i.%i", GLVersionMajor, GLVersionMinor);
	DEBUGWRITEINFO("Inited Graphics Manager with GL Version:",GLVersionString);

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
	success &= mauveassert::Assert::AssertTrue("Window creation failed",(currentWindow != nullptr), mauveassert::ENUM_severity::SEV_FATAL);
	
	//Set up the context
	glfwMakeContextCurrent(currentWindow);
	glewExperimental = GL_TRUE;
	int glewresult = glewInit();

	//Use default frag and vert shader
	currentShader = mauveresource::ResourceManager::GetResource<Shader>("data\\shaders\\default");

	glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if(!success) return false;
	return true;
}

bool GraphicsManager::DrawAndUpdateWindow(std::vector<IEntity*> entities, float dt)
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LEQUAL);

	glDepthMask(GL_TRUE);

	//// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	currentCamera->Update(dt);
	glfwSwapBuffers(currentWindow);
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.79f, 0.96f, 0.99f, 0.0f);
	glClearDepth(1.0f);
	
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
	modelMat *= glm::rotate(glm::mat4(1.0f), modelTransform->GetRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat *= glm::rotate(glm::mat4(1.0f), modelTransform->GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat *= glm::rotate(glm::mat4(1.0f), modelTransform->GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat *= glm::scale(glm::mat4(1.0f), modelTransform->GetScale());
	//TODO - ROTATE MATRIX IMPLEMENTATION
	
	

	//Upload matrix to the shader
	currentShader->SendUniformMat4("modelmatrix", modelMat);
	currentShader->SendUniformMat4("viewprojmatrix", currentCamera->GetViewProjMatrix());
	currentShader->SendUniformMat4("viewmatrix", currentCamera->GetViewMatrix());

	//Lighting stuff - TODO put it in the threeDgraphics component
	currentShader->SendUniformVec3("lightposition", currentCamera->GetCameraPosition());

	//Reflectivity
	currentShader->SendUniformVec3("Kd", glm::vec3(0.9f, 1.0f, 0.9f));

	//Light intensity
	currentShader->SendUniformVec3("Ld", glm::vec3(1.0f, 1.0f, 1.0f));

	currentShader->UseShader();

	GLuint vertarray;
	GLuint colourarray;

	// Create and set-up the vertex array object
	//glGenVertexArrays(1, &vertarray);

	//TODO - get colours working properly

	
	//Vertices
		glEnableVertexAttribArray(0);  // Vertex position
	//glBindBuffer(GL_ARRAY_BUFFER, componentToRender->GetVerticesID());
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);



	
	//Colours
	//glBindBuffer(GL_ARRAY_BUFFER, componentToRender->GetColoursID());
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	glEnableVertexAttribArray(1);  // Vertex colour

	//Normals
	//glBindBuffer(GL_ARRAY_BUFFER, componentToRender->GetNormalsID());
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	glEnableVertexAttribArray(2);  // Vertex colour
	glBindVertexArray(componentToRender->GetVAO());

	glDrawArrays(GL_TRIANGLES, 0, componentToRender->GetVertices().size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);



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

