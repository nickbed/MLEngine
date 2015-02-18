#include "ScriptManager.h"

ScriptManager::ScriptManager()
{

}

ScriptManager::~ScriptManager()
{
}

bool ScriptManager::Init()
{
	luaVM = luaL_newstate();
	luaL_openlibs(luaVM);
	ScriptComponent::setVM(luaVM);
	bindElements();

	return true;
}

void ScriptManager::Close()
{
	lua_close(luaVM);
}

void ScriptManager::bindElements()
{
	/*luabind::module(luaVM)
	[
		luabind::class_<IEntity>("gameobject")
		.def(luabind::constructor<>())
		//.def("addFrame", &Animation::addFrame)
	];*/
}