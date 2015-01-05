#include "GraphicsManager.h"

bool GraphicsManager::windowShouldBeClosed = false;

//Templates need to be at the top -_-
template<>
void GraphicsManager::RenderComponents<StaticMeshNoIndices>(StaticMeshNoIndices* componentToRender, TransformComponent* modelTransform)
{

	UploadShaderDataForDraw(modelTransform);

	//Vertices
	glEnableVertexAttribArray(0);  // Vertex position
	glEnableVertexAttribArray(1);  // Vertex colour
	glEnableVertexAttribArray(2);  // Vertex colour

	//Bind to the VAO
	glBindVertexArray(componentToRender->GetVAO());

	//Draw them
	glDrawArrays(GL_TRIANGLES, 0, componentToRender->GetVertices().size());

	glBindVertexArray(0);


	//Reset the state
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);


}

template<>
void GraphicsManager::RenderComponents<StaticMesh>(StaticMesh* componentToRender, TransformComponent* modelTransform)
{

	UploadShaderDataForDraw(modelTransform);

	//Vertices
	glEnableVertexAttribArray(0);  // Vertex position
	glEnableVertexAttribArray(1);  // Vertex colour
	glEnableVertexAttribArray(2);  // Vertex colour
	glEnableVertexAttribArray(3);  // Vertex colour

	//Bind to the VAO
	glBindVertexArray(componentToRender->GetVAO());

	//Bind the texture
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, componentToRender->GetTextureID());

	//Draw them
	glDrawElements(GL_TRIANGLES, componentToRender->GetIndicesCount(), GL_UNSIGNED_INT, 0);



	//Reset the state
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3); 



}

GraphicsManager::GraphicsManager()
{
	currentWindow = nullptr;
	windowShouldBeClosed = false;
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
	glfwSetWindowCloseCallback(currentWindow, WindowCloseCallback);

	//Set up the context
	glfwMakeContextCurrent(currentWindow);
	glewExperimental = GL_TRUE;
	int glewresult = glewInit();

	//Make camera controls work nicely
	glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if(!success) return false;
	return true;
}

bool GraphicsManager::DrawAndUpdateWindow(std::vector<IEntity*> entities, float dt)
{
	//Hook into the esc here to close the window - probably temp
	if (glfwGetKey(currentWindow, GLFW_KEY_ESCAPE))
	{
		windowShouldBeClosed = true;
	}

	//Close and cleanup
	if (windowShouldBeClosed)
	{
		glfwDestroyWindow(currentWindow);
		return false;
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Less or equal depth appears to work best.
	glDepthFunc(GL_LEQUAL);

	glDepthMask(GL_TRUE);

	//// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glfwSwapBuffers(currentWindow);
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.79f, 0.96f, 0.99f, 0.0f); //Background colour, should be replaced by skybox if there
	glClearDepth(1.0f);
	
	//Draw stuff
	for (auto& x: entities)
	{
		DrawEntity(x);
	}

	return true;
}

void GraphicsManager::WindowCloseCallback(GLFWwindow* window)
{
	windowShouldBeClosed = true;
}

void GraphicsManager::DrawEntity(IEntity* ent)
{
	//Get all instances of graphics components
	std::vector<IComponent*> arrayComponents = ent->Components->GetComponentsOfType("testGraphics");
	std::vector<IComponent*> meshComponents = ent->Components->GetComponentsOfType("staticmesh");

	for (auto& y : arrayComponents)
	{
		RenderComponents<StaticMeshNoIndices>((StaticMeshNoIndices*)y, ent->Transform);
	}

	for (auto& y : meshComponents)
	{
		RenderComponents<StaticMesh>((StaticMesh*)y, ent->Transform);
	}
}

bool GraphicsManager::UploadShaderDataForDraw(TransformComponent* modelTransform)
{
	currentShader->UseShader();

	//Set up model matrix
	glm::mat4 modelMat(1.0f);
	modelMat *= glm::translate(glm::mat4(1.0f), modelTransform->GetPosition());
	modelMat *= glm::rotate(glm::mat4(1.0f), modelTransform->GetRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat *= glm::rotate(glm::mat4(1.0f), modelTransform->GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat *= glm::rotate(glm::mat4(1.0f), modelTransform->GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat *= glm::scale(glm::mat4(1.0f), modelTransform->GetScale());
	//TODO - ROTATE MATRIX IMPLEMENTATION
	glm::mat4 modelViewCalc = currentCamera->GetViewMatrix() * modelMat;

	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMat)));


	//Upload matrix to the shader
	currentShader->SendUniformMat4("modelmatrix", modelMat);
	currentShader->SendUniformMat4("viewprojmatrix", currentCamera->GetViewProjMatrix());
	currentShader->SendUniformMat4("viewmatrix", currentCamera->GetViewMatrix());
	currentShader->SendUniformMat3("normalmatrix", normalMatrix);

	//Lighting stuff - TODO make some kind of material out of this
	currentShader->SendUniformVec3("lightposition", currentSceneLight->lightPosition);

	//Reflectivity
	currentShader->SendUniformVec3("Kd", currentSceneLight->surfaceReflectivity);

	//Light intensity
	currentShader->SendUniformVec3("Ld", currentSceneLight->lightIntensity);
	return true;
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

SceneLight* GraphicsManager::GetCurrentSceneLight()
{
	return currentSceneLight;
}

//TODO - maybe more than one shader?
Shader* GraphicsManager::GetCurrentShader()
{
	return currentShader;
}

void GraphicsManager::SetCurrentCamera(CameraEntity* cam)
{
	currentCamera = cam;
}

void GraphicsManager::SetCurrentSceneLight(SceneLight* light)
{
	currentSceneLight = light;
}

void GraphicsManager::SetCurrentShader(Shader* shader)
{
	currentShader = shader;
}
