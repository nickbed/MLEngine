#include "Robot.h"

Robot::~Robot()
{
}

void Robot::Init()
{
	IEntity::Init();

	currentRotationAngle0 = 0;
	currentRotationAngle1 = 0;

	rotationDirection0 = true;
	rotationDirection1 = false;

	isAnimating = false;
}

bool Robot::Update(float dt)
{
	Components->UpdateAllComponents(dt);
	camdir.y = 0.0f;
	camdir.x = sinf(glm::radians(Transform->GetRotation().y )) * (-1.0f * 0.33f);
	camdir.z = cosf(glm::radians(Transform->GetRotation().y )) * (-1.0f * 0.33f);
	followPos = Transform->GetPosition();
	followPos.x -= camdir.x * 10.0f; 
	followPos.z -= camdir.z * 10.0f; 
	followPos.y +=0.8f;
	mauvemessage::PositionMessage msg_cameraMove("robotPositionMove", followPos);
	mauvemessage::MessageManager::SendListnerMessage(&msg_cameraMove, "robotPositionMove");
	
	Script->Update(dt);

	if(!isAnimating) return true;
	isAnimating = false;
	const float rotAmount = 255.0f;
	const float maxRotate = 20.0f;
	const float minRotate = -20.0f;

	if(rotationDirection0)
	{
		//Forwards
		currentRotationAngle0 += rotAmount * dt;
	}
	else
	{
		currentRotationAngle0 -= rotAmount * dt;
	}
	if(currentRotationAngle0 > maxRotate)
	{
		rotationDirection0 = !rotationDirection0;
		currentRotationAngle0 = maxRotate;
	}
	if(currentRotationAngle0 < minRotate)
	{
		rotationDirection0 = !rotationDirection0;
		currentRotationAngle0 = minRotate;
	}

	if(rotationDirection1)
	{
		//Forwards
		currentRotationAngle1 += rotAmount * dt;
	}
	else
	{
		currentRotationAngle1 -= rotAmount * dt;
	}
	if(currentRotationAngle1 > maxRotate)
	{
		rotationDirection1 = !rotationDirection1;
		currentRotationAngle1 = maxRotate;
	}
	if(currentRotationAngle1 < minRotate)
	{
		rotationDirection1 = !rotationDirection1;
		currentRotationAngle1 = minRotate;
	}

	std::vector<IComponent*> gotBones = Components->GetComponentsOfType("basicbone");

	for(std::vector<IComponent*>::iterator it = gotBones.begin(); it != gotBones.end(); ++it)
	{
		//Cast to bone
		BasicBone* gotBone = (BasicBone*)*it;
		if(gotBone->id == "robotLeftLeg")
		{
			gotBone->BoneTransform.SetRotation(glm::vec3(currentRotationAngle0, 0, 0));
		}
		else if(gotBone->id == "robotRightLeg")
		{
			gotBone->BoneTransform.SetRotation(glm::vec3(currentRotationAngle1, 0, 0));
		}
		else if(gotBone->id == "robotRightArm")
		{
			gotBone->BoneTransform.SetRotation(glm::vec3(currentRotationAngle0 /2.0f, 0, 0));
		}
		else if(gotBone->id == "robotLeftArm")
		{
			gotBone->BoneTransform.SetRotation(glm::vec3(currentRotationAngle1 /2.0f, 0, 0));
		}
	}

	return true;
}

void Robot::msg_SetMovePosition(mauvemessage::BaseMessage* msg)
{
	mauvemessage::PositionMessage* posMsg = static_cast<mauvemessage::PositionMessage*>(msg);
	glm::vec3 messagePos = (glm::vec3)*posMsg;
	glm::vec3 moveSpeed(0.0f,0.0f,0.0f);
	glm::vec3 messageToSend;

	
	//Determine which direction we're going in
	if (messagePos.z != 0.0f)
	{
		//Going right
		moveSpeed.z = -1.0f;
		glm::vec3 moveVector = Transform->GetPosition();
		moveVector.y = 0.0f; //Don't want to take y into account
		moveVector.x = sinf(glm::radians(Transform->GetRotation().y)) * (moveSpeed.z * messagePos.z);
		moveVector.z = cosf(glm::radians(Transform->GetRotation().y)) * (moveSpeed.z * messagePos.z);
		
		Transform->SetPosition(Transform->GetPosition() + moveVector);

	}
	else if (messagePos.x != 0.0f)
	{
		moveSpeed.x = 1.0f;
		glm::vec3 moveVector = Transform->GetPosition();
		moveVector.y = 0.0f; //Don't want to take y into account
		moveVector.x = sinf(glm::radians(Transform->GetRotation().y + 90.0f)) * (moveSpeed.x * messagePos.x);
		moveVector.z = cosf(glm::radians(Transform->GetRotation().y + 90.0f)) * (moveSpeed.x * messagePos.x);
		
		Transform->SetPosition(Transform->GetPosition() + moveVector);
	}
	if (messagePos.z != 0.0f)

	
		
	isAnimating = true;
	
	

	
}

void Robot::msg_SetHeadPosition(mauvemessage::BaseMessage* msg)
{
	mauvemessage::PositionMessage* posMsg = static_cast<mauvemessage::PositionMessage*>(msg);
	glm::vec3 messagePos = (glm::vec3)*posMsg;
	if(messagePos.x > 5.0f || messagePos.x < -5.0f || messagePos.x == 0.0f) return;

	glm::vec3 setPos = Transform->GetRotation();
	setPos.y += glm::degrees(messagePos.x);
	Transform->SetRotation(setPos);
	setPos = Transform->GetRotation();
	setPos.y = 0;
	if(Transform->GetRotation().y > 360.0f || Transform->GetRotation().y < -360.0f) Transform->SetRotation(setPos);
}

void Robot::Destroy()
{
	IEntity::Destroy();
}

