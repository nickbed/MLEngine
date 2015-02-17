#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Loaders\ShaderLoader.h"
#include "Loaders\ModelLoader.h"
#include "Loaders\TextureLoader.h"
#include <unordered_map>

struct Model
{
	std::vector<GLfloat> verts, normals, textureCoords;
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
	//enum FileType { OBJ, TEX, SHA, UNKN};


	static bool instanceFlag;
	static ResourceManager *m_rManager;
	ResourceManager();
	//FileType fileType;
	/*ShaderLoader* m_sLoader;
	ModelLoader* m_mLoader;
	TextureLoader* m_tLoader;*/
	std::unordered_map<std::string, Shader*> m_shaders;
	std::unordered_map<std::string, Model*> m_model;
	std::unordered_map<std::string, Texture*> m_textures;
public:
	~ResourceManager();
	static ResourceManager* getInstance();
	//bool LoadMaster(const std::string& filename);
	void clearAll();
	void addToShader(const std::pair<std::string, Shader*>& pair);
	void addToTexture(const std::pair<std::string, Texture*>& pair);
	void addToModel(const std::pair<std::string, Model*>&  pair);

	std::unordered_map<std::string, Shader*> getShaders(){return m_shaders;};
	std::unordered_map<std::string, Model*> getModel(){return m_model;};
	std::unordered_map<std::string, Texture*> getTexture() {return m_textures;};
};
#endif

