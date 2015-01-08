#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include "..\Interfaces\IComponent.h"

typedef std::unordered_multimap<std::string, IComponent*>::iterator componentMapIterator;
typedef std::unordered_multimap<std::string, IComponent*> componentMapType;

//A class for storing components. Can be used within an entity or outside it.
class ComponentManager
{
public:
	ComponentManager();
	ComponentManager(std::unique_ptr<componentMapType> components);
	~ComponentManager();

	bool AddComponent(std::string componentType, IComponent* componentToAdd);
	bool RemoveComponent(std::string componentType, std::string componentID);

	std::vector<IComponent*> GetComponentsOfType(std::string componentType);

	const std::unique_ptr<componentMapType>& GetComponentList();

	void UpdateAllComponents(float dt);
	void InitAllComponents();
	void DestroyAllComponents();

private:
	std::unique_ptr<componentMapType> componentList;
};

#endif