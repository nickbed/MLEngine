#include "Transform.h"

TransformComponent::TransformComponent(std::string id)
{
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
}

TransformComponent::TransformComponent(std::string id, glm::vec3 pos)
{
	position = pos;
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
}

TransformComponent::TransformComponent(std::string id, glm::vec3 pos, glm::vec3 rot)
{
	position = pos;
	rotation = rot;
	scale = glm::vec3(1.0f);
}

TransformComponent::TransformComponent(std::string id, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl)
{
	position = pos;
	rotation = rot;
	scale = scl;
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetPosition(glm::vec3 pos)
{
	position = pos;
}

void TransformComponent::SetRotation(glm::vec3 rot)
{
	rotation = rot;
}

void TransformComponent::SetScale(glm::vec3 scl)
{
	scale = scl;
}

glm::vec3 TransformComponent::GetPosition()
{
	return position;
}

glm::vec3 TransformComponent::GetRotation()
{
	return rotation;
}

glm::vec3 TransformComponent::GetScale()
{
	return scale;
}

void TransformComponent::Init()
{
}

void TransformComponent::Update(float dt)
{
}

void TransformComponent::Destroy()
{
}

void TransformComponent::msg_MoveToPosition(mauvemessage::BaseMessage* msg)
{
	mauvemessage::PositionMessage* posMsg = static_cast<mauvemessage::PositionMessage*>(msg);
	glm::vec3 messagePos = (glm::vec3)*posMsg;
	position = messagePos;
}


