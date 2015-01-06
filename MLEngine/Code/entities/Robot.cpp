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

	////Create our components
	//StaticMesh* robotBody = new StaticMesh("robotBody");
	//BasicBone* robotLeftLeg = new BasicBone("robotLeftLeg");
	//BasicBone* robotRightLeg = new BasicBone("robotRightLeg");
	//BasicBone* robotLeftArm = new BasicBone("robotLeftArm");
	//BasicBone* robotRightArm = new BasicBone("robotRightArm");

	////Load in OBJ files
	//OBJModel* robotBodyObj = mauveresource::ResourceManager::GetResource<OBJModel>("data\\models\\robbody.obj");
	//OBJModel* robotLeftLegObj = mauveresource::ResourceManager::GetResource<OBJModel>("data\\models\\roblleg.obj");
	//OBJModel* robotRightLegObj = mauveresource::ResourceManager::GetResource<OBJModel>("data\\models\\robrleg.obj");
	//OBJModel* robotLeftArmObj = mauveresource::ResourceManager::GetResource<OBJModel>("data\\models\\roblarm.obj");
	//OBJModel* robotRightArmObj = mauveresource::ResourceManager::GetResource<OBJModel>("data\\models\\robrarm.obj");

	////Load Textures
	//ImageTexture* robotBodyTex = mauveresource::ResourceManager::GetResource<ImageTexture>("data\\images\\rainbow.png");
	//ImageTexture* robotLeftArmTex = mauveresource::ResourceManager::GetResource<ImageTexture>("data\\images\\rainbow.png");
	//ImageTexture* robotRightArmTex = mauveresource::ResourceManager::GetResource<ImageTexture>("data\\images\\rainbow.png");
	//ImageTexture* robotLeftLegTex = mauveresource::ResourceManager::GetResource<ImageTexture>("data\\images\\rainbow.png");
	//ImageTexture* robotRightLegTex = mauveresource::ResourceManager::GetResource<ImageTexture>("data\\images\\rainbow.png");

	////Upload everything into the entity

	////Body
	//robotBody->UploadVertices(robotBodyObj->GetVertices());
	//robotBody->UploadIndices(robotBodyObj->GetIndicies());
	//robotBody->UploadNormals(robotBodyObj->GetNormals());
	//robotBody->UploadUVs(robotBodyObj->GetUVs());
	//robotBody->UploadTexture(robotBodyTex->GetBitmap());

	////Left Leg
	//robotLeftLeg->UploadVertices(robotLeftLegObj->GetVertices());
	//robotLeftLeg->UploadIndices(robotLeftLegObj->GetIndicies());
	//robotLeftLeg->UploadNormals(robotLeftLegObj->GetNormals());
	//robotLeftLeg->UploadUVs(robotLeftLegObj->GetUVs());
	//robotLeftLeg->UploadTexture(robotLeftLegTex->GetBitmap());

	////Right Leg
	//robotRightLeg->UploadVertices(robotRightLegObj->GetVertices());
	//robotRightLeg->UploadIndices(robotRightLegObj->GetIndicies());
	//robotRightLeg->UploadNormals(robotRightLegObj->GetNormals());
	//robotRightLeg->UploadUVs(robotRightLegObj->GetUVs());
	//robotRightLeg->UploadTexture(robotRightLegTex->GetBitmap());

	////Left Arm
	//robotLeftArm->UploadVertices(robotLeftArmObj->GetVertices());
	//robotLeftArm->UploadIndices(robotLeftArmObj->GetIndicies());
	//robotLeftArm->UploadNormals(robotLeftArmObj->GetNormals());
	//robotLeftArm->UploadUVs(robotLeftArmObj->GetUVs());
	//robotLeftArm->UploadTexture(robotLeftArmTex->GetBitmap());

	////Right Arm
	//robotRightArm->UploadVertices(robotRightArmObj->GetVertices());
	//robotRightArm->UploadIndices(robotRightArmObj->GetIndicies());
	//robotRightArm->UploadNormals(robotRightArmObj->GetNormals());
	//robotRightArm->UploadUVs(robotRightArmObj->GetUVs());
	//robotRightArm->UploadTexture(robotRightArmTex->GetBitmap());

	////Add components to the entity
	//Components->AddComponent("staticmesh", robotBody);
	//Components->AddComponent("basicbone", robotLeftLeg);
	//Components->AddComponent("basicbone", robotRightLeg);
	//Components->AddComponent("basicbone", robotLeftArm);
	//Components->AddComponent("basicbone", robotRightArm);

}

bool Robot::Update(float dt)
{
	Components->UpdateAllComponents(dt);

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
	//Determine which direction we're going in
	if (messagePos.z != 0.0f)
	{
		//Going right
		moveSpeed.z = 1.0f;
		Transform->SetPosition(Transform->GetPosition() + moveSpeed * messagePos.z);
		if(messagePos.z > 0.0f)
		{
			Transform->SetRotation(glm::vec3(0.0f,-180.0f,0.0f));
		}
		else
		{
			Transform->SetRotation(glm::vec3(0.0f,0.0f,0.0f));
		}

	}
	else if (messagePos.x != 0.0f)
	{
		moveSpeed.x = -1.0f;
		if(messagePos.x > 0.0f)
		{
			Transform->SetRotation(glm::vec3(0.0f,90.0f,0.0f));
		}
		else
		{
			Transform->SetRotation(glm::vec3(0.0f,-90.0f,0.0f));
		}
		Transform->SetPosition(Transform->GetPosition() + moveSpeed * messagePos.x);
	}
	isAnimating = true;
	mauvemessage::PositionMessage msg_cameraMove("robotPositionMove", Transform->GetPosition());
	mauvemessage::MessageManager::SendListnerMessage(&msg_cameraMove, "robotPositionMove");
}

void Robot::Destroy()
{
	IEntity::Destroy();
}

