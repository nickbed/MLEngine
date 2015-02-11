#include "Script.h"

ScriptComponent::ScriptComponent(std::string id) : IComponent(id)
{
	loaded = false;
	script = CahLua::Script(false, "data/Scripts/default.lua");
	if (0 == script.getError())
	{
		loaded = true;
	}
}

ScriptComponent::ScriptComponent(std::string id, std::string filename) : IComponent(id)
{
	script = CahLua::Script(false, filename);
	if (0 == script.getError())
	{
		loaded = true;
	}
}


ScriptComponent::~ScriptComponent()
{
}

void ScriptComponent::CallFunc(std::string funcName)
{
	if (loaded)
	{
		script.callFunc(funcName);
	}
}

void ScriptComponent::Init()
{
	if (loaded)
	{
		script.execute();
	}
}

void ScriptComponent::Load(std::string filename)
{
	if (!loaded)
	{
		script = CahLua::Script(false, filename);
	}
	script.execute();
}

void ScriptComponent::Update(float dt)
{
	CallFunc("Update");
}

void ScriptComponent::Destroy()
{
}