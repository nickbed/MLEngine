#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Loaders\ShaderLoader.h"
#include "Loaders\ModelLoader.h"
#include "Loaders\TextureLoader.h"
#include <unordered_map>

struct Model
{
	std::vector<GLfloat> verts, normals, textureCoords;
	glm::vec3 min, max;
};
struct Shader
{
	GLuint fragShader;
	GLuint vertShader;
	GLuint programhandle;
};
class ResourceManager
{
private:

	static bool instanceFlag;
	static ResourceManager *m_rManager;
	ResourceManager();
	
	std::unordered_map<std::string, Shader*> m_shaders;
	std::unordered_map<std::string, Model*> m_model;
	std::unordered_map<std::string, Texture*> m_textures;

public:
	~ResourceManager();
	static ResourceManager* getInstance();
	bool loadDefaults();
	void clearAll();
	void addToShader(const std::pair<std::string, Shader*>& pair);
	void addToTexture(const std::pair<std::string, Texture*>& pair);
	void addToModel(const std::pair<std::string, Model*>&  pair);

	std::unordered_map<std::string, Shader*> getShaders(){return m_shaders;};
	std::unordered_map<std::string, Model*> getModel(){return m_model;};
	std::unordered_map<std::string, Texture*> getTexture() {return m_textures;};
	std::unordered_map<std::string, Model*>const &  getModel_const()const {return m_model;};
	std::unordered_map<std::string, Shader*>const &  getShaders_const()const {return m_shaders;};
	std::unordered_map<std::string, Texture*>const &  getTextures_const()const {return m_textures;};
};
#endif

