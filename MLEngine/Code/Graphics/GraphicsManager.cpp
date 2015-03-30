#include "GraphicsManager.h"
#include "..\Resources\GPUResourceManager.h"

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



	//Vertices
	glEnableVertexAttribArray(0);  // Vertex position
	glEnableVertexAttribArray(1);  // Vertex colour
	glEnableVertexAttribArray(2);  // Vertex colour
	glEnableVertexAttribArray(3);  // Vertex colour

	//Bind to the VAO
	glBindVertexArray(componentToRender->GetModelVAOID());

	//Bind the texture
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, componentToRender->GetTextureID());

	UploadShaderDataForDraw(modelTransform);
	
	//Draw them
	glDrawElements(GL_TRIANGLES, componentToRender->GetModelIndicesCount(), GL_UNSIGNED_INT, 0);



	//Reset the state
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3); 



}

template<>
void GraphicsManager::RenderComponents<BasicBone>(BasicBone* componentToRender, TransformComponent* modelTransform, TransformComponent boneTransform)
{
	UploadBoneShaderDataForDraw(*modelTransform, boneTransform);

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

template<>
void GraphicsManager::RenderComponents<BoundingBoxO>(BoundingBoxO* componentToRender, TransformComponent* modelTransform)
{
	glUseProgram(0);
	
	glm::mat4 viewprojmatrix = currentCamera->GetViewMatrix();

	glm::vec3 ext = componentToRender->GetExtent();
 
	glm::vec4 pos[8] =
	{
		glm::vec4(ext.x,ext.y,ext.z,1.0),    //0
		glm::vec4(ext.x,ext.y,-ext.z,1.0),   //1
		glm::vec4(-ext.x,ext.y,-ext.z,1.0),  //2
		glm::vec4(-ext.x,ext.y,ext.z,1.0),   //3
		glm::vec4(ext.x,-ext.y,ext.z,1.0),   //4
		glm::vec4(ext.x,-ext.y,-ext.z,1.0),  //5
		glm::vec4(-ext.x,-ext.y,-ext.z,1.0), //6
		glm::vec4(-ext.x,-ext.y,ext.z,1.0)   //7
	};

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(currentCamera->GetCameraFov(),(float)1024 / (float)768, 0.1f, 10000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(currentCamera->GetCameraPosition().x, currentCamera->GetCameraPosition().y, currentCamera->GetCameraPosition().z,currentCamera->GetCameraCenterPosition().x,currentCamera->GetCameraCenterPosition().y, currentCamera->GetCameraCenterPosition().z,currentCamera->GetUpVector().x, currentCamera->GetUpVector().y, currentCamera->GetUpVector().z);   
	glTranslatef(modelTransform->GetPosition().x, modelTransform->GetPosition().y, modelTransform->GetPosition().z);
	glTranslatef(componentToRender->GetCenter().x, componentToRender->GetCenter().y, componentToRender->GetCenter().z);
	glRotatef(modelTransform->GetRotation().x, 1.0, 0.0, 0.0);
	glRotatef(modelTransform->GetRotation().y, 0.0, 1.0, 0.0);
	glRotatef(modelTransform->GetRotation().z, 0.0, 0.0, 1.0);

	glLineWidth(1.0f);
	if(componentToRender->IsCollided()==false)
	{
		glColor3f(0.0f,1.0f,0.0f);
	}
	else
	{
		glColor3f(1.0f,0.0f,0.0f);
	}
	glDepthRange(0.1f,1000.f);
	glBegin(GL_LINES);
	glVertex3f(pos[0].x,pos[0].y,pos[0].z);
	glVertex3f(pos[1].x,pos[1].y,pos[1].z);
	glVertex3f(pos[1].x,pos[1].y,pos[1].z);
	glVertex3f(pos[2].x,pos[2].y,pos[2].z);
	glVertex3f(pos[2].x,pos[2].y,pos[2].z);
	glVertex3f(pos[3].x,pos[3].y,pos[3].z);
	glVertex3f(pos[3].x,pos[3].y,pos[3].z);
	glVertex3f(pos[0].x,pos[0].y,pos[0].z);

	glVertex3f(pos[0].x,pos[0].y,pos[0].z);
	glVertex3f(pos[4].x,pos[4].y,pos[4].z);
	glVertex3f(pos[1].x,pos[1].y,pos[1].z);
	glVertex3f(pos[5].x,pos[5].y,pos[5].z);
	glVertex3f(pos[2].x,pos[2].y,pos[2].z);
	glVertex3f(pos[6].x,pos[6].y,pos[6].z);
	glVertex3f(pos[3].x,pos[3].y,pos[3].z);
	glVertex3f(pos[7].x,pos[7].y,pos[7].z);

	glVertex3f(pos[4].x,pos[4].y,pos[4].z);
	glVertex3f(pos[5].x,pos[5].y,pos[5].z);
	glVertex3f(pos[5].x,pos[5].y,pos[5].z);
	glVertex3f(pos[6].x,pos[6].y,pos[6].z);
	glVertex3f(pos[6].x,pos[6].y,pos[6].z);
	glVertex3f(pos[7].x,pos[7].y,pos[7].z);
	glVertex3f(pos[7].x,pos[7].y,pos[7].z);
	glVertex3f(pos[4].x,pos[4].y,pos[4].z);

	glEnd();
}

template<>
void GraphicsManager::RenderComponents<BoundingCapsule>(BoundingCapsule* componentToRender, TransformComponent* modelTransform)
{
	glUseProgram(0);
	
	glm::mat4 viewprojmatrix = currentCamera->GetViewMatrix();

	float ext = componentToRender->GetExtent();
	float rad = componentToRender->GetRadius();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(currentCamera->GetCameraFov(),(float)1024 / (float)768, 0.1f, 10000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(currentCamera->GetCameraPosition().x, currentCamera->GetCameraPosition().y, currentCamera->GetCameraPosition().z,currentCamera->GetCameraCenterPosition().x,currentCamera->GetCameraCenterPosition().y, currentCamera->GetCameraCenterPosition().z,currentCamera->GetUpVector().x, currentCamera->GetUpVector().y, currentCamera->GetUpVector().z);   
	glTranslatef(modelTransform->GetPosition().x, modelTransform->GetPosition().y, modelTransform->GetPosition().z);
	glTranslatef(componentToRender->GetCenter().x, componentToRender->GetCenter().y, componentToRender->GetCenter().z);
	glRotatef(modelTransform->GetRotation().x, 1.0, 0.0, 0.0);
	glRotatef(modelTransform->GetRotation().y, 0.0, 1.0, 0.0);
	glRotatef(modelTransform->GetRotation().z, 0.0, 0.0, 1.0);

	glTranslatef(0.f,-ext,0.f);
	glRotatef(-90.f,1.0,0.0,0.0);

	glLineWidth(1.0f);
	if(componentToRender->IsCollided()==false)
	{
		glColor3f(0.0f,1.0f,0.0f);
	}
	else
	{
		glColor3f(1.0f,0.0f,0.0f);
	}
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_POLYGON_SMOOTH);
	GLUquadricObj *obj = gluNewQuadric();
	gluCylinder(obj,rad,rad,ext*2.f,16,2);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_LINES);
	glVertex3f(0.f,0.f,-rad);
	glVertex3f(0.f,0.f,ext*2+rad);
	glEnd();
}

void GraphicsManager::RenderText(std::string text, int x, int y, int size, IEntity* entity)
{
	if (windowHasClosed) return;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.098f, 0.0f, 0.8f, 1.0f); //Background colour, should be replaced by skybox if there
	glClearDepth(1.0f);
	if(entity != nullptr) DrawAndUpdateWindow(entity, 0.0f, false);
	textRenderer->Draw2DText(text, x, y, size);
	glfwSwapBuffers(currentWindow);
	//Hack

}

void GraphicsManager::RenderText(std::string text, int x, int y, int size)
{
	if (windowHasClosed) return;
	textRenderer->Draw2DText(text, x, y, size);
}

GraphicsManager::GraphicsManager()
{
	currentWindow = nullptr;
	windowShouldBeClosed = false;
	textRenderer = new TextRender();
	currentParticleManager = new ParticleManager();
	windowHasClosed = false;
	delayTime = 0.0f;
	
}

GraphicsManager::~GraphicsManager()
{
	delete textRenderer;
	delete currentParticleManager;
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
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); //Needed to get the bounding boxes to work
	glfwWindowHint(GL_DOUBLEBUFFER, GL_TRUE);

	

	//Initialize clear color
	glClearColor(0.0f, 0.098f, 0.0f, 1.0f);

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

	success &= textRenderer->InitTextRender(xSize, ySize);
	skyboxShader = mauvefileresource::ResourceManager::GetResource<Shader>("data\\shaders\\skybox");
	skyboxShader->UseShader();
	currentSkyBox = new SkyBox();
	currentSkyBox->LoadFile("data\\images\\checkered_top.jpg", 0);
	currentSkyBox->LoadFile("data\\images\\checkered_left.jpg", 1);
	currentSkyBox->LoadFile("data\\images\\checkered_front.jpg", 2);
	currentSkyBox->LoadFile("data\\images\\checkered_right.jpg", 3);
	currentSkyBox->LoadFile("data\\images\\checkered_back.jpg", 4);
	currentSkyBox->LoadFile("data\\images\\checkered_top.jpg", 5);
	currentSkyBox->InitCubeMap();
	currentSkyBox->InitSkybox();

	//Particles


	if(!success) return false;
	return true;
}

void GraphicsManager::SetWindowTitle(const char* windowTitle)
{
	if (currentWindow == nullptr) return;
	glfwSetWindowTitle(currentWindow, windowTitle);
}

bool GraphicsManager::DrawAndUpdateWindow(IEntity* *entities, int numEntities, float dt, bool poll)
{
	delayTime += dt;
	if (delayTime > 0.5f)
	{
		//currentParticles->SetPosition(currentCamera->GetCameraPosition());
		delayTime = 0.0f;
	}
	if (windowHasClosed) return false;
	
	//Close and cleanup
	if (windowShouldBeClosed)
	{
		return false;
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Less or equal depth appears to work best.
	glDepthFunc(GL_LEQUAL);

	//

	glfwSwapBuffers(currentWindow);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_FALSE);
	RenderSkybox();
	glDepthMask(GL_TRUE);
	//Transparency

	//glClearColor(0.098f, 0.0f, 0.8f, 1.0f); //Background colour, should be replaced by skybox if there
	
	glClearDepth(1.0f);
	
	//currentParticles->Draw(currentCamera->GetViewProjMatrix());
	//currentParticles->Update(0.0f, glm::mat4(0.0f));

	currentShader->UseShader();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	bool result = true;
	for (int i = 0; i < numEntities; ++i)
	{
		result &= DrawAndUpdateWindow(*entities++, dt, poll);
	}

	currentParticleManager->UpdateManager(currentCamera->GetViewProjMatrix(), currentCamera->GetCameraPosition());
	currentParticleManager->UpdateAndRenderParticleSystems(0.0f);
	
	return result;
}

void GraphicsManager::PollWindow()
{
	
	glfwPollEvents();
	//Hook into the esc here to close the window - probably temp
	if (glfwGetKey(currentWindow, GLFW_KEY_ESCAPE))
	{
		windowShouldBeClosed = true;
	}
	//Close and cleanup
	if (windowShouldBeClosed)
	{
		//Stop memory leaks
		mauvegpuresource::GPUResourceManager::UnloadAllResources();
		mauvefileresource::ResourceManager::UnloadAllResources();
		mauvemessage::MessageManager::ClearAllListners();
		glfwDestroyWindow(currentWindow);
		delete textRenderer;
		windowHasClosed = true;
	}

}

bool GraphicsManager::DrawAndUpdateWindow(IEntity* entity, float dt, bool poll)
{
	DrawEntity(entity);
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
	std::vector<IComponent*> boneComponents = ent->Components->GetComponentsOfType("basicbone");

	for (auto& x : arrayComponents)
	{
		RenderComponents<StaticMeshNoIndices>((StaticMeshNoIndices*)x, ent->Transform);
	}

	for (auto& y : meshComponents)
	{
		RenderComponents<StaticMesh>((StaticMesh*)y, ent->Transform);
	}
	for (auto& z : boneComponents)
	{
		BasicBone* gotBone = (BasicBone*)z;
		RenderComponents<BasicBone>(gotBone, ent->Transform, gotBone->BoneTransform);
	}
}

void GraphicsManager::DrawDebug(IEntity* ent)
{
	std::vector<IComponent*> obbComponents = ent->Components->GetComponentsOfType("boundingboxo");
	std::vector<IComponent*> capsuleComponents = ent->Components->GetComponentsOfType("boundingcapsule");

	for (auto& w : obbComponents)
	{
		BoundingBoxO* gotBox = (BoundingBoxO*)w;
		RenderComponents<BoundingBoxO>(gotBox, ent->Transform);
	}
	for (auto& w : capsuleComponents)
	{
		BoundingCapsule* gotCap = (BoundingCapsule*)w;
		RenderComponents<BoundingCapsule>(gotCap, ent->Transform);
	}
}

void GraphicsManager::RenderSkybox()
{
	skyboxShader->UseShader();
	skyboxShader->SendUniformMat4("viewprojmatrix", currentCamera->GetViewProjSkyboxMatrix());
	currentSkyBox->BindTexture();
	currentSkyBox->RenderSkybox();
	currentShader->UseShader();
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
	currentShader->SendUniformVec3("lightposition", currentLights[0].lightPosition);

	//Reflectivity
	currentShader->SendUniformVec3("Kd", currentLights[0].surfaceReflectivity);

	//Light intensity
	currentShader->SendUniformVec3("Ld", currentLights[0].lightIntensity);

	//TEMP
	currentShader->SendUniformVec3("Sd", glm::vec3(0.2, 0.2, 0.2));

	currentShader->SendUniform1f("Sp", 35.0f);

	//Camera position
	currentShader->SendUniformVec3("cameraPos", currentCamera->GetCameraPosition());
	return true;
}

//Hack
bool GraphicsManager::UploadBoneShaderDataForDraw(TransformComponent modelTransform, TransformComponent boneTransform)
{
	currentShader->UseShader();

	//Set up model matrix
	glm::mat4 modelMat(1.0f);
	modelMat *= glm::translate(glm::mat4(1.0f), modelTransform.GetPosition());
	modelMat *= glm::rotate(glm::mat4(1.0f), modelTransform.GetRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat *= glm::rotate(glm::mat4(1.0f), modelTransform.GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat *= glm::rotate(glm::mat4(1.0f), modelTransform.GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat *= glm::scale(glm::mat4(1.0f), modelTransform.GetScale());

	glm::mat4 boneMat(1.0f);
	//boneMat *= glm::translate(glm::mat4(1.0f), boneTransform.GetPosition());
	//Translate to rotate position
	boneMat *= glm::translate(glm::mat4(1.0f), -boneTransform.GetPosition());
	boneMat *= glm::rotate(glm::mat4(1.0f), boneTransform.GetRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
	boneMat *= glm::rotate(glm::mat4(1.0f), boneTransform.GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
	boneMat *= glm::rotate(glm::mat4(1.0f), boneTransform.GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
	boneMat *= glm::translate(glm::mat4(1.0f), boneTransform.GetPosition());
	//boneMat *= glm::scale(glm::mat4(1.0f), boneTransform.GetScale());

	//TEMP - put this in the shader?
	modelMat *= boneMat;

	glm::mat4 modelViewCalc = currentCamera->GetViewMatrix() * modelMat;

	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMat)));


	//Upload matrix to the shader
	currentShader->SendUniformMat4("modelmatrix", modelMat);
	currentShader->SendUniformMat4("viewprojmatrix", currentCamera->GetViewProjMatrix());
	currentShader->SendUniformMat4("viewmatrix", currentCamera->GetViewMatrix());
	currentShader->SendUniformMat3("normalmatrix", normalMatrix);

	//Lighting stuff - TODO make some kind of material out of this
	currentShader->SendUniformVec3("lightposition", currentLights[0].lightPosition);

	//Reflectivity
	currentShader->SendUniformVec3("Kd", currentLights[0].surfaceReflectivity);

	//Light intensity
	currentShader->SendUniformVec3("Ld", currentLights[0].lightIntensity);
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

//SceneLight* GraphicsManager::GetCurrentSceneLight()
//{
//	return currentSceneLight;
//}

//TODO - maybe more than one shader?
Shader* GraphicsManager::GetCurrentShader()
{
	return currentShader;
}

void GraphicsManager::SetCurrentCamera(CameraEntity* cam)
{
	currentCamera = cam;
}

//void GraphicsManager::SetCurrentSceneLight(SceneLight* light)
//{
//	currentSceneLight = light;
//}

void GraphicsManager::SetActiveSceneLights(int numLights, SceneLight *lights)
{
	for (int i = 0; i < numLights; ++i)
	{
		currentLights[i] = *lights++;
	}
}

void GraphicsManager::SetCurrentShader(Shader* shader)
{
	currentShader = shader;
}

void GraphicsManager::SetCurrentSkybox(SkyBox* skybox)
{
	currentSkyBox = skybox;
}
