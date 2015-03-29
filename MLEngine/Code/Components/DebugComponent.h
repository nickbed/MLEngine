#ifndef DebugComponent_H
#define DebugComponent_H
#include "../Interfaces/IComponent.h"
#include "../Messages/BaseMessage.h"
#include <iostream>

//Debugging component that will cout a lot of stuff
class DebugComponent : public IComponent
{
public:
	//Constructor for the component
	DebugComponent(std::string id);

	void Init();

	//Update method
	void Update(double dt);

	//We'll print any messages we receive
	void SendMessage(mauvemessage::BaseMessage message);

	//Gets called to bin us
	void Destroy();

	//Destructor
	virtual ~DebugComponent();
};



#endif