#include "ResourceManager.h"


bool ResourceManager::instanceFlag = false;
ResourceManager * ResourceManager::m_rManager = NULL;
ResourceManager::ResourceManager():
	m_model(), 
	m_shaders(),
	m_textures() 
{

}
ResourceManager* ResourceManager::getInstance()
{
	if(!instanceFlag)
	{
		m_rManager = new ResourceManager();
		instanceFlag = true;
		return m_rManager;
	}
	//! Returns the player if there is already a player.
	else return m_rManager;
}

ResourceManager::~ResourceManager()
{
 
	for(auto it = m_shaders.begin(); it != m_shaders.end(); ++it)
	{
		delete (*it).second;
		(*it).second = NULL;
	}
	for(auto it = m_model.begin(); it != m_model.end(); ++it)
	{
		delete (*it).second;
		(*it).second = NULL;
	}
	for(auto it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		delete (*it).second;
		(*it).second = NULL;
	}
	
}

void  ResourceManager::addToModel(const std::pair<std::string, Model*>& pair)
{
	m_model.insert(pair);
}
void ResourceManager::addToShader(const std::pair<std::string, Shader*>& pair)
{
	m_shaders.insert(pair);
}
void ResourceManager::addToTexture(const std::pair<std::string, Texture*>& pair)
{
	m_textures.insert(pair);
}
void ResourceManager::loadDefaults()
{
	TextureLoader* t_loader = new TextureLoader();
	t_loader->LoadTexture("data\\images\\holstein1.png");
	std::pair<std::string, Texture*> texturePair;
	texturePair.first = t_loader->getName();
	texturePair.second = t_loader->getTexture();
	this->addToTexture(texturePair);
	//std::pair<std::string, Texture*> texturePair2;
	t_loader->LoadTexture("data\\images\\default.png");
	texturePair.first = t_loader->getName();
	texturePair.second = t_loader->getTexture();
	this->addToTexture(texturePair);
	t_loader->LoadTexture("data\\images\\light.jpg");
	texturePair.first = t_loader->getName();
	texturePair.second = t_loader->getTexture();
	this->addToTexture(texturePair);

	ShaderLoader m_sloader;
	m_sloader.LoadShader("data\\shaders\\basic3.vert","data\\shaders\\basic3.Frag");
	Shader* shader = new Shader();
	shader->fragShader = m_sloader.getFrag();
	shader->vertShader = m_sloader.getVert();
	shader->programhandle = m_sloader.getProgramHandle();
	std::pair<std::string, Shader*> shaderPair;
	shaderPair.first = m_sloader.getName();
	shaderPair.second = shader;
	this->addToShader(shaderPair);

	m_sloader.LoadShader("data\\shaders\\TextVertexShader.vert", "data\\shaders\\TextVertexShader.Frag");
	Shader* shader2 = new Shader();
	shader2->fragShader = m_sloader.getFrag();
	shader2->vertShader = m_sloader.getVert();
	shader2->programhandle = m_sloader.getProgramHandle();
	std::pair<std::string, Shader*> shaderPair2;
	shaderPair2.first = m_sloader.getName();
	shaderPair2.second = shader2;
	this->addToShader(shaderPair2);
}



void ResourceManager::clearAll()
{
	m_shaders.clear();
	m_model.clear();
	m_textures.clear();
}