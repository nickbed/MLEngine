#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <unordered_map>
#include <memory>
#include <vector>
#include "..\Interfaces\IComponent.h"

typedef std::unordered_multimap<const char*, IComponent*>::iterator componentMapIterator;
typedef std::unordered_multimap<const char*, IComponent*> componentMapType;

//A class for storing components. Can be used within an entity or outside it.
class ComponentManager
{
public:
	ComponentManager();
	ComponentManager(std::unique_ptr<componentMapType> components);

	bool AddComponent(const char* componentType, IComponent* componentToAdd);
	bool RemoveComponent(const char* componentType, const char* componentID);

	std::vector<IComponent*> GetComponentsOfType(const char* componentType);

	const std::unique_ptr<componentMapType>& GetComponentList();

	void UpdateAllComponents(float dt);
	void InitAllComponents();
	void DestroyAllComponents();

private:
	std::unique_ptr<componentMapType> componentList;
};

#endif