#include "GameObjects\GameObject.h"

GameObject::GameObject()
{
	m_renderer = NULL;
	m_transform = NULL;
	m_anim = NULL;
	m_shader = NULL;
	m_rManager = ResourceManager::getInstance();
	m_renderer = new RenderComponent();
	m_transform = new TransformComponent();
	m_shader = new ShaderComponent();
}
GameObject::GameObject(std::string nName)
{
	m_rManager = ResourceManager::getInstance();
	m_name = nName;
	m_renderer = NULL;
	m_transform = NULL;
	m_shader = NULL;
	m_rManager = ResourceManager::getInstance();
	m_renderer = new RenderComponent();
	m_transform = new TransformComponent();
	m_shader = new ShaderComponent();
}
GameObject::~GameObject()
{
	delete m_renderer;
	delete m_transform;
	delete m_shader;
	delete m_anim;
	m_anim = NULL;
	m_renderer = NULL;
	m_transform = NULL;
	m_shader = NULL;
}
void GameObject::setEntityType(const std::string& entityType)
{
	m_type = entityType;
}
void GameObject::setName(const std::string& nName)
{
	m_name = nName;
}
void GameObject::init(const std::string& nShaderName)
{
	
	gl::UseProgram(m_rManager->getShaders().at(nShaderName)->programhandle);
}
void GameObject::update(bool keys[])
{
	
		if(m_anim && keys[GLFW_KEY_W] || m_anim && keys[GLFW_KEY_A] || m_anim && keys[GLFW_KEY_S]
		|| m_anim && keys[GLFW_KEY_D])
		{
			m_anim->Prepare();
			m_transform->Rotate(0.0f,0.0f,glm::radians(m_anim->getAngle()));
		}
		m_transform->Update();
}

void GameObject::render()
{
	
	m_shader->Render();
	m_renderer->update();
}
bool GameObject::addComponent(std::string nName)
{
	if(nName == "animComp")
	{
		m_compType = ANIM;
	}
	else m_compType = UNKN;
	switch(m_compType)
	{
	case ANIM:
		m_anim = new AnimationComponent();
		return true;
		break;
	case UNKN:
		m_anim = NULL;
		return false;
		break;
	}
}


