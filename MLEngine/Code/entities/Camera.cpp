#include "Camera.h"

CameraEntity::~CameraEntity()
{
}

void CameraEntity::Init()
{
	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	SetLookPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	cameraMatrix = glm::mat4(1.0);
	cameraFOV = 70.0f;
	viewMatrix = glm::perspective(cameraFOV, (float)640 / (float)480, 0.1f, 10000.0f);
}

bool CameraEntity::Update(float dt)
{
	return true;
}

void CameraEntity::Destroy()
{
	return;
}

void CameraEntity::SetCameraMatrix(glm::mat4 matrix)
{
	cameraMatrix = matrix;
	RegenerateCameraMatrix();
}

void CameraEntity::SetViewMatrix(glm::mat4 matrix)
{
	viewMatrix = matrix;
	RegenerateCameraMatrix();
}


void CameraEntity::SetPosition(glm::vec3 pos)
{
	Transform->SetPosition(pos);
	RegenerateCameraMatrix();
}

void CameraEntity::SetFov(float fov)
{
	cameraFOV = fov;
	RegenerateCameraMatrix();
	
}

void CameraEntity::SetLookPosition(glm::vec3 pos)
{
	cameraLookPosition = pos;
	RegenerateCameraMatrix();
}

const glm::mat4 CameraEntity::GetCameraMatrix()
{
	return cameraMatrix;
}

const glm::vec3 CameraEntity::GetCameraPosition()
{
	return Transform->GetPosition();
}

const float CameraEntity::GetCameraFov()
{
	return cameraFOV;
}

const glm::vec3 CameraEntity::GetLookPosition()
{
	return cameraLookPosition;
}

const glm::mat4 CameraEntity::GetViewMatrix()
{
	return viewMatrix;
}

const glm::mat4 CameraEntity::GetViewProjMatrix()
{
	return viewProjMatrix;
}

void CameraEntity::RegenerateCameraMatrix()
{
	cameraMatrix = glm::lookAt(Transform->GetPosition(), cameraLookPosition, glm::vec3(0.0f, 1.0f, 0.0f));
	viewMatrix = glm::perspective(cameraFOV, (float)640 / (float)480, 0.1f, 10000.0f);
	viewProjMatrix = viewMatrix * cameraMatrix;
}