#ifndef ScriptComponent_H
#define ScriptComponent_H
#include "../Interfaces/IComponent.h"
#include "../Assert/Assert.h"
#include "..\Messages\CollisionMessage.h"

#include <iostream>
#include <memory>
#include <map>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include <LuaBridge/LuaBridge.h>

class IEntity;

struct LuaScriptFuncs{
	//std::shared_ptr<luabridge::LuaRef> luaDataTable;
	std::shared_ptr<luabridge::LuaRef> updateFunc;
	std::shared_ptr<luabridge::LuaRef> collisionFunc;
	std::shared_ptr<luabridge::LuaRef> startFunc;
	unsigned short refCount;
	std::string id;

	LuaScriptFuncs(){
		refCount = 1;
	};

	LuaScriptFuncs& ref()
	{ 
		refCount++; 
		return *this; 
	};
	void unref()
	{ 
		refCount++; 
		if (refCount == 0)
		{
			startFunc.reset();
			collisionFunc.reset();
			updateFunc.reset();
		}
	};
};

//For transforming/positioning stuff
class ScriptComponent : public IComponent
{
public:
	//Constructor for the component
	ScriptComponent(std::string id = "defaultScript");

	//Getters/Setters
	virtual void Init();
	void Load(std::string filename = "", std::string expectedNamespace = "");

	//Update method
	virtual void Update(float dt);

	//Gets called to bin us
	virtual void Destroy();

	//Destructor
	virtual ~ScriptComponent();

	static void setVM(lua_State*);

	void msg_Collision(mauvemessage::BaseMessage* msg);

	IEntity* owner;


private:
	static std::map<std::string, LuaScriptFuncs> scriptFuncList;
	
	LuaScriptFuncs ownFuncs;

	static lua_State* luaVM;
	std::string envName;




	int error;
};



#endif