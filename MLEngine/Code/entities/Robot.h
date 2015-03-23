#ifndef Robot_H
#define Robot_H
#include "..\Interfaces\IEntity.h"
#include "..\Components\StaticMesh.h"
#include "..\Components\BasicBone.h"
#include "..\resources\OBJModel.h"
#include "..\resources\ImageTexture.h"
#include "..\resources\ResourceManager.h"
#include "..\Messages\MessageManager.h"

class Robot : public IEntity
{
public:
	DEFAULTENTITYCONSTRUCTOR(Robot);
	virtual ~Robot();
	
	virtual void Init();
	virtual bool Update(float dt);
	virtual void Destroy();

	void msg_SetMovePosition(mauvemessage::BaseMessage* msg);
	void msg_SetHeadPosition(mauvemessage::BaseMessage* msg);

private:
	float currentRotationAngle0;
	float currentRotationAngle1;

	bool rotationDirection0;
	bool rotationDirection1;

	bool isAnimating;
};

#endif