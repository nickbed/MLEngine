#include "../Interfaces/ISystem.h"


void ISystem::Init()
{
}

void ISystem::Update(float dt)
{
	for (unsigned int i = 0; i < Components.size(); ++i)
	{
		Components.at(i)->Update(dt);
	}
}


ISystem::~ISystem()
{

}



IComponent& ISystem::GetComponent(std::string id)
{
	for (unsigned int i = 0; i < Components.size(); ++i)
	{
		if(Components.at(i)->id == id)
		{
			return *Components.at(i);
		};
	}
}

bool ISystem::HasComponent(std::string id)
{
	for (unsigned int i = 0; i < Components.size(); ++i)
	{
		if(Components.at(i)->id == id)
		{
			return true;
		};
	}
	return false;
}

void ISystem::AddComponent(std::unique_ptr<IComponent> component)
{
	Components.push_back(std::move(component));
}
