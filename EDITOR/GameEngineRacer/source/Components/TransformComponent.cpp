#include "Components\TransformComponent.h"

TransformComponent::TransformComponent():

	m_scale(0,0,0),
	m_rotate(0,0,0,0),
	m_translate(0,0,0)
{
}
void TransformComponent::Init()
{
	
}

void TransformComponent::setScale(const glm::vec3& scale)
{
	m_scale = scale;
}
void TransformComponent::setRotate(const glm::quat& rotate)
{
	m_rotate = rotate;
}
void TransformComponent::setTranslate(const glm::vec3& translate)
{
	m_translate = translate;
}

void TransformComponent::Update()
{
	
}
/*void TransformComponent::Rotate(float nx, float ny, float nz)
{
	m_rotate.x += nx;
	m_rotate.y += ny;
	m_rotate.z += nz;

}
void TransformComponent::Translate(float nx, float ny, float nz)
{
	m_translate.x += nx;
	m_translate.y += ny;
	m_translate.z += nz;
}
void TransformComponent::Scale(float nx, float ny, float nz)
{
	m_scale.x += nx;
	m_scale.y += ny;
	m_scale.z += nz;
}*/

void TransformComponent::Reset()
{
	m_scale = glm::vec3 (0.0f,0.0f,0.0f);
	m_rotate = glm::quat (0.0f,0.0f,0.0f, 0.0f);
	m_translate = glm::vec3 (0.0f,0.0f,0.0f);
}

