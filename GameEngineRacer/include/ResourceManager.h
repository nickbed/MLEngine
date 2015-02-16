#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Loaders\ShaderLoader.h"
#include "Loaders\ModelLoader.h"
#include "Loaders\AnimationLoader.h"
#include "Loaders\TextureLoader.h"
#include <unordered_map>

struct Model
	{
		std::vector<GLfloat> verts, normals, textureCoords;
	};
class ResourceManager
{
private:
	enum FileType { OBJ, TEX, SHA, UNKN};
	
	struct Shader
	{
		GLuint fragShader;
		GLuint vertShader;
		GLuint programhandle;
	};
	static bool instanceFlag;
	static ResourceManager *m_rManager;
	ResourceManager();
	FileType fileType;
	ShaderLoader* m_sLoader;
	ModelLoader* m_mLoader;
	AnimationLoader* m_aLoader;
	TextureLoader* m_tLoader;
	std::unordered_map<std::string, Shader*> m_shaders;
	std::unordered_map<std::string, Model*> m_model;
	std::unordered_map<std::string, Texture*> m_textures;
public:
	~ResourceManager();
	static ResourceManager* getInstance();
	bool LoadMaster(const std::string& filename);
	void clearAll();
	std::unordered_map<std::string, Shader*> getShaders(){return m_shaders;};
	std::unordered_map<std::string, Model*> getModel(){return m_model;};
	std::unordered_map<std::string, Texture*> getTexture() {return m_textures;};
};
#endif

