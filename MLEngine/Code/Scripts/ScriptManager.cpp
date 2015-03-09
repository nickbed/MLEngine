#include "ScriptManager.h"


#include "ClassHelpers.h"



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
		//Interfaces
		.beginClass<IComponent>("component")
		.endClass()

		.beginClass<IEntity>("gameobject")
		.addConstructor<void(*)()>()
		.addData("transform", &IEntity::Transform)
		.addProperty("transform", &IEntity::getTransform, &IEntity::setTransform)
		.addData("components", &IEntity::Components)
		//.addProperty("components", &IEntityHelper::getComponentManager, &IEntityHelper::setComponentManager)
		.endClass()


		//Components
		.deriveClass<BasicKeyMovement, IComponent>("key")
		.addStaticFunction("Pressed", BasicKeyMovement::getAscii)
		.endClass()

		.beginClass<ComponentManager>("components")
		.addFunction("Add", &ComponentManager::AddComponent)
		.endClass()

		.deriveClass<StaticMesh, IComponent>("staticmesh")
		.addConstructor<void(*)(std::string)>()
		.addStaticFunction("New", StaticMeshHelper::_new)
		.addProperty("Texture", &StaticMeshHelper::getTexture, &StaticMeshHelper::setTexture)
		.addProperty("Model", &StaticMeshHelper::getModel, &StaticMeshHelper::setModel)
		.endClass()

		.beginClass<TransformComponent>("transform")
		.addProperty("position", &TransformComponent::GetPosition, &TransformComponent::SetPosition)
		.addProperty("rotation", &TransformComponent::GetRotation, &TransformComponent::SetRotation)
		.addProperty("scale", &TransformComponent::GetScale, &TransformComponent::SetScale)
		.endClass()


		//Scenes
		.beginClass<SceneManager>("scene")
		.addStaticFunction("NewObject", SceneManager::AddEntity)
		.endClass()


		//Libs
		.beginClass<glm::vec3>("Vector3")
		.addConstructor<void(*)(float, float, float)>()
		.addProperty("x", &Vec3Helper::get<0>, &Vec3Helper::set<0>)
		.addProperty("y", &Vec3Helper::get<1>, &Vec3Helper::set<1>)
		.addProperty("z", &Vec3Helper::get<2>, &Vec3Helper::set<2>)
		.endClass();


}