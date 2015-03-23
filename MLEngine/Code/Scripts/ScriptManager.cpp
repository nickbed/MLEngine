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


	//Perform out actual bindings to Lua here
	luabridge::getGlobalNamespace(luaVM)

		.beginClass<IEntity>("gameobject")
		.addConstructor<void(*)(void)>()
		.addData("transform", &IEntity::Transform)
		.addProperty("transform", &IEntity::getTransform, &IEntity::setTransform)

		.endClass()

		.beginClass<TransformComponent>("transform")
		.addProperty("position", &TransformComponent::GetPosition, &TransformComponent::SetPosition)
		.addProperty("rotation", &TransformComponent::GetRotation, &TransformComponent::SetRotation)
		.addProperty("scale", &TransformComponent::GetScale, &TransformComponent::SetScale)
		.endClass()



		.beginClass<glm::vec3>("Vector3")
		.addConstructor<void(*)(float, float, float)>()
		.addProperty("x", &Vec3Helper::get<0>, &Vec3Helper::set<0>)
		.addProperty("y", &Vec3Helper::get<1>, &Vec3Helper::set<1>)
		.addProperty("z", &Vec3Helper::get<2>, &Vec3Helper::set<2>)

		.endClass()

		.beginClass<BasicKeyMovement>("key")
		.addStaticFunction("Pressed", BasicKeyMovement::getAscii)
		.endClass();

}