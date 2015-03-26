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
		.addData("name", &IEntity::id)
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

		.deriveClass<BoundingVolume, IComponent>("boundingvolume")
		.addStaticFunction("New", BoundingVolumesHelper::_newBase)
		.endClass()

		.deriveClass<BoundingSphere, BoundingVolume>("boundingsphere")
		.addStaticFunction("New", BoundingVolumesHelper::_newSphere)
		.endClass()

		.deriveClass<BoundingCapsule, BoundingVolume>("boundingcapsule")
		.addStaticFunction("New", BoundingVolumesHelper::_newCapsule)
		.endClass()

		.deriveClass<BoundingBox, BoundingVolume>("boundingbox")
		.addStaticFunction("New", BoundingVolumesHelper::_newBox)
		.endClass()

		.deriveClass<BoundingBoxO, BoundingVolume>("boundingboxo")
		.addStaticFunction("New", BoundingVolumesHelper::_newBoxO)
		.endClass()


		//Scenes
		.beginClass<SceneManager>("scene")
		.addStaticFunction("NewObject", SceneManager::AddEntity)
		.addStaticFunction("RemoveObject", SceneManager::DestroyEntity)
		.endClass()


		//Libs
		.beginClass<glm::vec3>("Vector3")
		.addConstructor<void(*)(float, float, float)>()
		.addProperty("x", &Vec3Helper::get<0>, &Vec3Helper::set<0>)
		.addProperty("y", &Vec3Helper::get<1>, &Vec3Helper::set<1>)
		.addProperty("z", &Vec3Helper::get<2>, &Vec3Helper::set<2>)
		.endClass()


		//Messages
		.beginClass<CollisionManifold>("collisionmanifold")
		.addData("Collision", &CollisionManifold::Collision)
		.addData("Axis", &CollisionManifold::Axis)
		.addData("AxisBox", &CollisionManifold::AxisBox)
		.addData("Penetration", &CollisionManifold::Penetration)
		.addData("Sign", &CollisionManifold::Sign)
		.addData("VolumeA", &CollisionManifold::VolumeA)
		.addData("VolumeB", &CollisionManifold::VolumeB)
		.addData("Top",&CollisionManifold::Top)
		.endClass();
}