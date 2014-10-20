#ifndef ICOMPONENT_H
#define ICOMPONENT_H
#include "../Messages/BaseMessage.h"
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

	//When something needs to send a message to the component
	virtual void SendMessage(BaseMessage message) = 0;

	//Gets called to bin us
	virtual void Destroy() = 0;

	//Destructor
	virtual ~IComponent();

	//ID
	std::string id;
};


#endif