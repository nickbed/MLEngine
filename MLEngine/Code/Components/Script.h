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
#include <LuaBridge/RefCountedObject.h>

class IEntity;

struct LuaScriptFuncs {
	LuaScriptFuncs(lua_State* L) :
		start(L),
		collision(L),
		update(L),
		reset(L),
		luaDataTable(L)
	{
		refCount = 1;
	};
	luabridge::LuaRef luaDataTable;
	luabridge::LuaRef update;
	luabridge::LuaRef collision;
	luabridge::LuaRef reset;
	luabridge::LuaRef start;
	unsigned short refCount;
	std::string id;
	LuaScriptFuncs& ref()
	{
		refCount++;
		return *this;
	};
	void unref()
	{
		refCount--;
	};
};

struct funcWrapper{
	LuaScriptFuncs* funcs;

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
	static std::map<std::string, funcWrapper> scriptFuncList;

	LuaScriptFuncs ownFuncs;

	static lua_State* luaVM;
	std::string envName;




	int error;
};



#endif