#ifndef DebugComponent_H
#define DebugComponent_H
#include "../Interfaces/IComponent.h"
#include "../Messages/BaseMessage.h"
#include "GLM\glm.hpp"
#include <iostream>

//For transforming/positioning stuff
class TransformComponent : public IComponent
{
public:
	//Constructor for the component
	TransformComponent(std::string id);
	TransformComponent(std::string id, glm::vec3 pos);
	TransformComponent(std::string id, glm::vec3 pos, glm::vec3 rot);
	TransformComponent(std::string id, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl);

	//Getters/Setters
	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();

	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rot);
	void SetScale(glm::vec3 scl);

	virtual void Init();

	//Update method
	virtual void Update(float dt);

	//Gets called to bin us
	virtual void Destroy();

	//Destructor
	virtual ~TransformComponent();

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};



#endif