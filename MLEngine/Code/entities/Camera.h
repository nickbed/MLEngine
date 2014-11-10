#ifndef CAMERAENTITY_H
#define CAMERAENTITY_H
#include "..\Interfaces\IEntity.h"
#include "GLM\glm.hpp"
#include "GLM\gtx\transform.hpp"

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

	const glm::mat4 GetCameraMatrix();
	const glm::vec3 GetCameraPosition();
	const float GetCameraFov();
	const glm::vec3 GetLookPosition();
	const glm::mat4 GetViewMatrix();
	const glm::mat4 GetViewProjMatrix();

private:
	void RegenerateCameraMatrix();

	glm::mat4 cameraMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 viewProjMatrix;
	glm::vec3 cameraLookPosition;
	float cameraFOV;

};

#endif