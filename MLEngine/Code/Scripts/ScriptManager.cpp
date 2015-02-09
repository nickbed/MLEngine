#include "ScriptManager.h"

ScriptManager::ScriptManager()
{

}

ScriptManager::~ScriptManager()
{
	CahLua::close();
}

bool ScriptManager::Init()
{
	bool success = CahLua::open();



	return success;
}