#include "Script.h"

#include "../Interfaces/IEntity.h"

lua_State* ScriptComponent::luaVM = nullptr;
int ScriptComponent::uid = 0;

ScriptComponent::ScriptComponent(std::string id) : IComponent(id)
{
	this->error = -1;
	this->uuid = "env_" + std::to_string(uid);
	++uid;
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
	if (luaL_dofile(luaVM, filename.c_str()) == 0) {
		luabridge::LuaRef table = luabridge::getGlobal(luaVM, expectedNamespace.c_str());
		luaDataTable = std::make_shared<luabridge::LuaRef>(table);
		if (table.isTable()) {
			if (table["Update"].isFunction()) {
				updateFunc = std::make_shared<luabridge::LuaRef>(table["Update"]);
			}
			else {
				updateFunc.reset();
			}
			if (table["Start"].isFunction()) {
				startFunc = std::make_shared<luabridge::LuaRef>(table["Start"]);
				(*startFunc)(owner);
			}
			else {
				startFunc.reset();
			}
		}
	}
	else {
		mauveassert::Assert::WriteDebug("Lua: Unable to find/interpret file: " + filename, mauveassert::ENUM_severity::SEV_WARNING);
	}
}

void ScriptComponent::Update(float dt)
{
	if (updateFunc) {
		try{
			(*updateFunc)(dt);
		}
		catch (luabridge::LuaException const& e) {
			std::cout << "Lua Exception: " << e.what() << std::endl;
		}
	}
}

void ScriptComponent::Destroy()
{
	this->owner = nullptr;
}

void ScriptComponent::setVM(lua_State* L)
{
	luaVM = L;
}