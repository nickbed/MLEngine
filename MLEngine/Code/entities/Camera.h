#ifndef CAMERAENTITY_H
#define CAMERAENTITY_H
#include "..\Interfaces\IEntity.h"
#include "GLM\glm.hpp"
#include "GLM\gtx\transform.hpp"
#include "..\Messages\PositionMessage.h"
#include "..\Messages\MessageManager.h"

class CameraEntity : public IEntity
{
public:
	DEFAULTENTITYCONSTRUCTOR(CameraEntity);
	virtual ~CameraEntity();

	virtual void Init();
	virtual bool Update(float dt);
	virtual void Destroy();

	void SetCameraMatrix(glm::mat4 matrix);
	void SetViewMatrix(glm::mat4 matrix);
	void SetPosition(glm::vec3 pos);
	void SetFov(float fov);
	void SetLookPosition(glm::vec3 pos);
	void SetPitch(float pitch);
	void SetYaw(float yaw);

	//Message processors
	void msg_SetMovePosition(mauvemessage::BaseMessage* msg);
	void msg_SetLookPosition(mauvemessage::BaseMessage* msg);
	void msg_SetFollowPosition(mauvemessage::BaseMessage* msg);

	const glm::mat4 GetCameraMatrix();
	const glm::vec3 GetCameraPosition();
	const glm::vec3 GetCameraCenterPosition();
	const float GetCameraFov();
	const glm::vec3 GetUpVector();
	const glm::vec3 GetLookPosition();
	const glm::mat4 GetViewMatrix();
	const glm::mat4 GetViewProjMatrix();
	const glm::mat4 GetViewProjSkyboxMatrix();
	const float GetPitch();
	const float GetYaw();

	bool Listning;

private:
	void RegenerateCameraMatrix();

	glm::mat4 cameraMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 viewProjMatrix;
	glm::mat4 viewProjSkybox;
	glm::vec3 cameraLookPosition;
	glm::vec3 upVector;
	glm::vec3 rightVector;
	glm::vec3 cameraDirection;
	float pitch;
	float yaw;
	float cameraFOV;

};

#endif