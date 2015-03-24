#ifndef ICOMPONENT_H
#define ICOMPONENT_H
#include "../Messages/BaseMessage.h"
#include "../Components/Transform.h"
#include <string>

//Interface (Abstract class) that defines how components interact with entities
class IComponent
{
public:
	//Constructor for the component
	IComponent(std::string id);

	virtual void Init() = 0;

	//Update method
	virtual void Update(float dt) = 0;

	//Gets called to bin us
	virtual void Destroy() = 0;

	//Destructor
	virtual ~IComponent();

	void SetParentTransform(TransformComponent* pTransform);

	TransformComponent* GetParentTransform();
	//ID
	std::string id;
	
private:
	TransformComponent* parentTransform;
};


#endif