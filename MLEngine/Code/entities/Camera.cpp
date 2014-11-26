#include "Camera.h"

CameraEntity::~CameraEntity()
{
}

void CameraEntity::Init()
{
	SetPosition(glm::vec3(0.0f, 1.0f, -5.0f));
	SetLookPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	cameraMatrix = glm::mat4(1.0);
	cameraFOV = 50.0f;
	viewMatrix = glm::perspective(cameraFOV, (float)1024 / (float)768, 0.1f, 10000.0f);
	pitch = 0.0f;
	yaw = 0.0f;
	upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	rightVector = glm::vec3(1.0f, 0.0f, 0.0f);
	cameraDirection = glm::vec3(0.0f, 0.0f, 1.0f);
}

bool CameraEntity::Update(float dt)
{
	IEntity::Update(dt);
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

void CameraEntity::SetPitch(float ptch)
{
	pitch = ptch;
}

void CameraEntity::SetYaw(float yw)
{
	yaw = yw;
}

void CameraEntity::msg_SetMovePosition(mauvemessage::BaseMessage* msg)
{
	mauvemessage::PositionMessage* posMsg = static_cast<mauvemessage::PositionMessage*>(msg);
	glm::vec3 messagePos = (glm::vec3)*posMsg;
	//Determine which direction we're going in
	if (messagePos.z != 0.0f)
	{
		//Going right
		SetPosition(GetCameraPosition() + cameraDirection * messagePos.z);
		SetLookPosition(cameraLookPosition + cameraDirection * messagePos.z);

	}
	else if (messagePos.x != 0.0f)
	{
		glm::vec3 tempRightVector = rightVector * messagePos.x;
		//tempRightVector *= messagePos.z;

		SetPosition(GetCameraPosition() + tempRightVector);
		SetLookPosition(cameraLookPosition + tempRightVector);
		//Going forward
	}
	RegenerateCameraMatrix();
}

void CameraEntity::msg_SetLookPosition(mauvemessage::BaseMessage* msg)
{
	mauvemessage::PositionMessage* posMsg = static_cast<mauvemessage::PositionMessage*>(msg);
	glm::vec3 messagePos = (glm::vec3)*posMsg;
	//Rubbish data filtering
	if(messagePos.x > 5.0f || messagePos.x < -5.0f || messagePos.y > 5.0f || messagePos.y < -5.0f) return;
	yaw += messagePos.x;
	pitch += messagePos.y;
	if (pitch > glm::radians(85.0f))
	{
		pitch = glm::radians(85.0f);
	}
	if (pitch < glm::radians(-85.0f))
	{
		pitch = glm::radians(-85.0f);
	}
	if(yaw > glm::radians(360.0f) || yaw < glm::radians(-360.0f)) yaw = 0.0f;
	//glm::vec3 lookDirection(glm::cos(pitch) * glm::sin(yaw), glm::sin(pitch), glm::cos(pitch) * glm::cos(yaw));
	glm::vec3 lookDirection(glm::cos(pitch) * glm::sin(yaw), glm::sin(pitch), glm::cos(pitch) * glm::cos(yaw));
	cameraDirection = glm::normalize(lookDirection);
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

const float CameraEntity::GetPitch()
{
	return pitch;
}

const float CameraEntity::GetYaw()
{
	return yaw;
}

void CameraEntity::RegenerateCameraMatrix()
{

	rightVector = glm::vec3(glm::sin(yaw - glm::pi<float>() / 2.0f), 0, glm::cos(yaw - glm::pi<float>() / 2.0f));
	//upVector = glm::cross(rightVector, cameraDirection);
	cameraMatrix = glm::lookAt(Transform->GetPosition(), Transform->GetPosition() + cameraDirection, upVector);
	viewMatrix = glm::perspective(cameraFOV, (float)1024/ (float)768, 0.1f, 1000.0f);
	viewProjMatrix = viewMatrix * cameraMatrix;
}