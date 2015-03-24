#include "Script.h"

#include "../Interfaces/IEntity.h"

lua_State* ScriptComponent::luaVM = nullptr;
std::map<std::string, LuaScriptFuncs> ScriptComponent::scriptFuncList;

ScriptComponent::ScriptComponent(std::string id) : IComponent(id)
{
	this->error = -1;
	owner = nullptr;
}

ScriptComponent::~ScriptComponent()
{
	this->owner = nullptr;
}

void ScriptComponent::Init()
{
}

void ScriptComponent::Load(std::string filename, std::string expectedNamespace)
{
	envName = expectedNamespace;
	if (scriptFuncList.find(envName) != scriptFuncList.end())
	{
		ownFuncs = scriptFuncList[envName].ref();
		try {
			(*ownFuncs.startFunc)(owner);
		}
		catch (luabridge::LuaException const& e) {
			std::cout << "Lua Exception: " << e.what() << std::endl;
		}
		return;
	}
	if (luaL_dofile(luaVM, filename.c_str()) == 0) {
		luabridge::LuaRef table = luabridge::getGlobal(luaVM, expectedNamespace.c_str());
		LuaScriptFuncs funcList;
		if (table.isTable()) {
			if (table["Update"].isFunction()) {
				funcList.updateFunc = std::make_shared<luabridge::LuaRef>(table["Update"]);
			} else {
				funcList.updateFunc.reset();
			}

			if (table["Collision"].isFunction()) {
				funcList.collisionFunc = std::make_shared<luabridge::LuaRef>(table["Collision"]);
			} else {
				funcList.collisionFunc.reset();
			}

			if (table["Start"].isFunction()) {
				funcList.startFunc = std::make_shared<luabridge::LuaRef>(table["Start"]);
				try {
					(*funcList.startFunc)(owner);
				}
				catch (luabridge::LuaException const& e) {
					std::cout << "Lua Exception: " << e.what() << std::endl;
				}
			} else {
				funcList.startFunc.reset();
			}
			scriptFuncList[envName] = funcList;
			ownFuncs = scriptFuncList[envName].ref();
			
			DEBUGWRITEINFO("Successfully loaded Lua Script:", filename)
		}
	}
	else {
		mauveassert::Assert::WriteDebug("Lua: Unable to find/interpret file: " + filename, mauveassert::ENUM_severity::SEV_WARNING);
	}
}

void ScriptComponent::Update(float dt)
{
	if (ownFuncs.updateFunc) {
		try{
			(*ownFuncs.updateFunc)(owner, dt);
		}
		catch (luabridge::LuaException const& e) {
			std::cout << "Lua Exception: " << e.what() << std::endl;
		}
	}
}

void ScriptComponent::Destroy()
{
	this->owner = nullptr;
	ownFuncs.unref();
}

void ScriptComponent::setVM(lua_State* L)
{
	luaVM = L;
}


void ScriptComponent::msg_Collision(mauvemessage::BaseMessage* msg)
{
	mauvemessage::CollisionMessage* c = static_cast<mauvemessage::CollisionMessage*>(msg);
	CollisionManifold m = (CollisionManifold)*c;
	if (ownFuncs.collisionFunc) {
		try{
			(*ownFuncs.collisionFunc)(owner, m);
		}
		catch (luabridge::LuaException const& e) {
			std::cout << "Lua Exception: " << e.what() << std::endl;
		}
	}

}