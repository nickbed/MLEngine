#include "ComponentManager.h"
#include "..\Assert\Assert.h"



ComponentManager::ComponentManager()
{
	mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_FATAL, "Cannot call default constructor on component manager");
}

ComponentManager::ComponentManager(std::unique_ptr<componentMapType> components)
{
	//Move the pointer to be contained in this class
	components.swap(componentList);
}

bool ComponentManager::AddComponent(const char* componentType, IComponent* componentToAdd)
{
	bool result = mauveassert::Assert::AssertTrue("Component type is null", componentType != nullptr, mauveassert::ENUM_severity::SEV_ERROR);
	if(!result) return false;

	//Insert our component
	componentList->insert(std::pair<const char*, IComponent*>(componentType, componentToAdd));
	return true;
}

bool ComponentManager::RemoveComponent(const char* componentType, const char* componentID)
{
	bool result = mauveassert::Assert::AssertTrue("Component type is null", componentType != nullptr, mauveassert::ENUM_severity::SEV_ERROR);
	result &= mauveassert::Assert::AssertTrue("Component ID is null", componentID != nullptr, mauveassert::ENUM_severity::SEV_ERROR);
	if(!result) return false;

	//Get our list of components that match the type
	componentMapIterator startIt, endIt;
	std::pair<componentMapIterator, componentMapIterator> keyRange = componentList->equal_range(componentType);

	for(startIt = keyRange.first; startIt != keyRange.second; ++startIt)
	{
		if((*startIt).second->id == componentID)
		{
			componentList->erase(startIt);
			delete startIt->second;
			return true;
		}
	}
	return false;
}

std::vector<IComponent*> ComponentManager::GetComponentsOfType(const char* componentType)
{
	bool result = mauveassert::Assert::AssertTrue("Component type is null", componentType != nullptr, mauveassert::ENUM_severity::SEV_ERROR);
	std::vector<IComponent*> gotComponents;

	//Get our list of components that match the type
	componentMapIterator startIt, endIt;
	std::pair<componentMapIterator, componentMapIterator> keyRange = componentList->equal_range(componentType);

	for(startIt = keyRange.first; startIt != keyRange.second; ++startIt)
	{
		gotComponents.push_back((*startIt).second);
	}

	return gotComponents;
}

const std::unique_ptr<componentMapType>& ComponentManager::GetComponentList()
{
	return componentList;
}