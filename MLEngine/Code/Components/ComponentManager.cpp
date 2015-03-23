#include "ComponentManager.h"
#include "..\Assert\Assert.h"



ComponentManager::ComponentManager()
{
	mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_FATAL, "Cannot call default constructor on component manager");
}

ComponentManager::ComponentManager(std::unique_ptr<componentMapType> components)
{
	//Move the pointer to be contained in this class
	componentList = std::move(components);
}

ComponentManager::~ComponentManager()
{
	DestroyAllComponents();
	componentList.release();
}

bool ComponentManager::AddComponent(std::string componentType, IComponent* componentToAdd)
{
	bool result = mauveassert::Assert::AssertTrue("Component type is null", componentType != "", mauveassert::ENUM_severity::SEV_ERROR);
	if(!result) return false;

	//Insert our component
	componentList->insert(std::pair<std::string, IComponent*>(componentType, componentToAdd));
	return true;
}

bool ComponentManager::RemoveComponent(std::string componentType, std::string componentID)
{
	bool result = mauveassert::Assert::AssertTrue("Component type is null", componentType != "", mauveassert::ENUM_severity::SEV_ERROR);
	result &= mauveassert::Assert::AssertTrue("Component ID is null", componentID != "", mauveassert::ENUM_severity::SEV_ERROR);
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

std::vector<IComponent*> ComponentManager::GetComponentsOfType(std::string componentType)
{
	bool result = mauveassert::Assert::AssertTrue("Component type is null", componentType != "", mauveassert::ENUM_severity::SEV_ERROR);
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

void ComponentManager::UpdateAllComponents(float dt)
{
	componentMapIterator startIt;
	for (startIt = componentList->begin(); startIt != componentList->end(); ++startIt)
	{
		(*startIt).second->Update(dt);
	}
}

void ComponentManager::InitAllComponents()
{
	componentMapIterator startIt;
	for (startIt = componentList->begin(); startIt != componentList->end(); ++startIt)
	{
		(*startIt).second->Init();
	}
}

void ComponentManager::DestroyAllComponents()
{
	//Call destroy on the component, and delete them all. Left with an empty unique ptr, that can delete itsself if needed.
	componentMapIterator startIt;
	for (startIt = componentList->begin(); startIt != componentList->end(); ++startIt)
	{
		if((*startIt).second == nullptr) break;
		(*startIt).second->Destroy();
		delete (*startIt).second;
	}
	componentList->clear();
	
}