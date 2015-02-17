#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "Components\TransformComponent.h"
#include "Components\RenderComponent.h"
#include "ResourceManager.h"
#include "Components\AnimationComponent.h"
#include "ResourceManager.h"


//class ResourceManager;
class GameObject
{
private:
	////////
	////Component Saving for Json//////
	std::vector<std::string> componentIDs;
	std::vector<std::string> componentTypes;
	std::vector<std::string> modelFiles;
	std::vector<std::string> textureFiles;
	///////////////////////////////////

	enum ComponentType {ANIM, UNKN};
	std::string m_name;
	std::string m_type;

	ResourceManager* m_rManager;
	ComponentType m_compType;

	AnimationComponent *m_anim;
	TransformComponent *m_transform;//Pointer to Transform component
	RenderComponent *m_renderer;//Creates the vbo and vao for this object.

public:
	GameObject();
	GameObject(std::string nName);
	~GameObject();
	virtual void init(const std::string& nShaderName);//Initialises the game objects
	virtual void update(bool keys[]);//Updates the Game objects
	virtual void render();

	void setName(const std::string& nName);
	const std::string& getName()const {return m_name;};

	bool addComponent(std::string nName);

	void addToComponentID(const std::string& ID){ componentIDs.push_back(ID);};
	void addToComponentTYPE(const std::string& type){componentTypes.push_back(type);};
	void addToComponentModelFiles(const std::string& modelFileName){modelFiles.push_back(modelFileName);};
	void addToComponentTextureFiles(const std::string& textureFileName){textureFiles.push_back(textureFileName);};

	const std::vector<std::string>& getComponentIDs()const{return componentIDs;};
	const std::vector<std::string>& getComponentTypes()const{return componentTypes;};
	const std::vector<std::string>& getComponentModelFiles()const{return modelFiles;};
	const std::vector<std::string>& getComponentTextureFiles()const{return textureFiles;};

	void setEntityType(const std::string& entityType);
	const std::string& getEntityType()const {return m_type;};

	TransformComponent* getTransformComp(){return m_transform;};
	RenderComponent* getRenderComp(){return m_renderer;};
	AnimationComponent* getAnimComp(){return m_anim;};
};
#endif