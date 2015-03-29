#include "Script.h"

#include "../Interfaces/IEntity.h"

lua_State* ScriptComponent::luaVM = nullptr;
std::map<std::string, funcWrapper> ScriptComponent::scriptFuncList;

ScriptComponent::ScriptComponent(std::string id) : IComponent(id), ownFuncs(luaVM)
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
		ownFuncs = scriptFuncList[envName].funcs->ref();
		try {
			ownFuncs.start(owner);
		}
		catch (luabridge::LuaException const& e) {
			std::cout << "Lua Exception: " << e.what() << std::endl;
		}
		return;
	}
	if (luaL_dofile(luaVM, filename.c_str()) == 0) {
		luabridge::LuaRef table = luabridge::getGlobal(luaVM, expectedNamespace.c_str());
		funcWrapper wrapper;
		wrapper.funcs = new LuaScriptFuncs(luaVM);
		wrapper.funcs->id = envName;
		if (table.isTable()) {
			wrapper.funcs->luaDataTable = table;
			if (table["Update"].isFunction()) {
				wrapper.funcs->update = table["Update"];
			}

			if (table["Collision"].isFunction()) {
				wrapper.funcs->collision = table["Collision"];
			}

			if (table["Start"].isFunction()) {
				wrapper.funcs->start = table["Start"];
				try {
					wrapper.funcs->start(owner);
				}
				catch (luabridge::LuaException const& e) {
					std::cout << "Lua Exception: " << e.what() << std::endl;
				}
			}
			scriptFuncList[envName] = wrapper;
			ownFuncs = scriptFuncList[envName].funcs->ref();
			
			DEBUGWRITEINFO("Successfully loaded Lua Script:", filename);
		}
	}
	else {
		mauveassert::Assert::WriteDebug("Lua: Unable to find/interpret file: " + filename, mauveassert::ENUM_severity::SEV_WARNING);
	}
}

void ScriptComponent::Update(float dt)
{
	if (ownFuncs.update) {
		try{
			ownFuncs.update(owner, dt);
		}
		catch (luabridge::LuaException const& e) {
			mauveassert::Assert::WriteDebug(std::string("Lua Exception: ") + e.what(), mauveassert::ENUM_severity::SEV_WARNING);
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
	if(m.VolumeA->GetOwnerId()==owner->id||m.VolumeB->GetOwnerId()==owner->id)
	{

		if (ownFuncs.collision) {
			try{
				ownFuncs.collision(owner, m);
			}
			catch (luabridge::LuaException const& e) {
				mauveassert::Assert::WriteDebug(std::string("Lua Exception: ") + e.what(), mauveassert::ENUM_severity::SEV_WARNING);
			}
		}
	}
}