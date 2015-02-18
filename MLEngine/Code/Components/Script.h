#ifndef ScriptComponent_H
#define ScriptComponent_H
#include "../Interfaces/IComponent.h"
#include "../Assert/Assert.h"

#include <iostream>
#include <memory>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include <LuaBridge/LuaBridge.h>

//For transforming/positioning stuff
class ScriptComponent : public IComponent
{
public:
	//Constructor for the component
	ScriptComponent(std::string id = "defaultScript");

	//Getters/Setters
	virtual void Init();
	void Load(std::string filename = "");

	//Update method
	virtual void Update(float dt);

	//Gets called to bin us
	virtual void Destroy();

	//Destructor
	virtual ~ScriptComponent();

	static void setVM(lua_State*);

private:
	//luabind::object luaDataTable;
	//luabind::object compiledScript;
	static lua_State* luaVM;
	static int uid;
	std::string uuid;

	std::shared_ptr<luabridge::LuaRef> updateFunc;

	int error;
};



#endif