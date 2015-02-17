#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#define GLM_FORCE_RADIANS

#include "3rdParty\gl_core_4_3.hpp"
#include "glfw3.h"
#include "glm\glm.hpp"
#include "glm\gtc\constants.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"

class TransformComponent
{
private:
	
	glm::vec3 m_scale;
	glm::quat m_rotate;
	glm::vec3 m_translate;

	
public:
	TransformComponent();
	void Init();
	void Update();
	//void Rotate(float nx, float ny, float nz);
	//void Translate(float x, float y, float z);
	//void Scale(float nx, float ny, float nz);
	void Reset();


	const glm::vec3& getScale() {return m_scale;};
	const glm::quat& getRotate() {return m_rotate;};
	const glm::vec3& getTranslate() {return m_translate;};
	void setRotate(const glm::quat& rotate);
	void setTranslate(const glm::vec3& rotate);
	void setScale(const glm::vec3& scale);

};
#endif