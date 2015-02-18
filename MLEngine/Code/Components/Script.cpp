#include "Script.h"

lua_State* ScriptComponent::luaVM = nullptr;
int ScriptComponent::uid = 0;

ScriptComponent::ScriptComponent(std::string id) : IComponent(id)
{
	this->error = -1;
	this->uuid = "env_" + std::to_string(uid);
	++uid;
}

ScriptComponent::~ScriptComponent()
{
}

void ScriptComponent::Init()
{
}

void ScriptComponent::Load(std::string filename)
{
	if (luaL_dofile(luaVM, filename.c_str()) == 0) {
		luabridge::LuaRef table = luabridge::getGlobal(luaVM, "Default");
		if (table.isTable()) {
			if (table["Update"].isFunction()) {
				updateFunc = std::make_shared<luabridge::LuaRef>(table["Update"]);
			}
			else {
				updateFunc.reset();
			}
			if (table["Start"].isFunction()) {
				table["Start"]();
			}
		}
	}
	else {
		mauveassert::Assert::WriteDebug("Lua: Unable to open file: " + filename, mauveassert::ENUM_severity::SEV_WARNING);
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
}

void ScriptComponent::setVM(lua_State* L)
{
	luaVM = L;
}