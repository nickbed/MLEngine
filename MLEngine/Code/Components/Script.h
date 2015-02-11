#ifndef ScriptComponent_H
#define ScriptComponent_H
#include "../Interfaces/IComponent.h"

#include <cahlua/CahLua.hpp>
#include <iostream>

//For transforming/positioning stuff
class ScriptComponent : public IComponent
{
public:
	//Constructor for the component
	ScriptComponent(std::string id);
	ScriptComponent(std::string id, std::string filename);

	//Getters/Setters
	void CallFunc(std::string funcName);


	virtual void Init();
	void Load(std::string filename = "");

	//Update method
	virtual void Update(float dt);

	//Gets called to bin us
	virtual void Destroy();

	//Destructor
	virtual ~ScriptComponent();


private:
	CahLua::Script script;
	bool loaded;
};



#endif