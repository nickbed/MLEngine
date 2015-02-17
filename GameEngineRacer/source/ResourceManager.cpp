#include "ResourceManager.h"


bool ResourceManager::instanceFlag = false;
ResourceManager * ResourceManager::m_rManager = NULL;
ResourceManager::ResourceManager():
	m_model(), 
	m_shaders(),
	m_textures() 
{
	/*m_tLoader = new TextureLoader();
	m_sLoader = new ShaderLoader();
	m_mLoader = new ModelLoader();*/
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
   /*delete m_tLoader;
	delete m_rManager;
	delete m_sLoader;
	delete m_mLoader;*/
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
	/*m_mLoader = NULL;
	m_sLoader = NULL;
	m_rManager = NULL;
	m_tLoader = NULL;*/
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


/*bool ResourceManager::LoadMaster(const std::string& filename)//Will load a file containing all resources.
{
	std::ifstream input;
	std::stringstream ss;
	std::string line, nameObject, texName, shader;
	std::string flipped;
	std::size_t found;
	std::pair<std::string,Model*> modelPair;
	std::pair<std::string,Shader*> shaderPair;
	std::pair<std::string,Texture*> texturePair;
	int i=0;
	input.open(filename);
	if(!input.is_open())
	{
		std::cout << "file not found" << std::endl;
		return false;
	}
	while(getline(input, line))
	{
		fileType = UNKN;
		found = line.find("obj");
		if (found!= std::string::npos)
		{
			//cout<<line<<endl;
			fileType = OBJ;
		}
		found = line.find("tex");
		if (found!= std::string::npos)
		{
			fileType = TEX;
		}
		found = line.find("sha");
		if (found!= std::string::npos)
		{
			fileType = SHA;
		}
		Model* m = new Model();
		Shader* s = new Shader();
		ss.str("");
		ss.clear();
		ss.str(line);
		ss.ignore(3);
		switch(fileType){
		case OBJ:
			ss >> nameObject;
			m_mLoader->loadFromfile(nameObject);

			m->normals = m_mLoader->getNormals();
			m->textureCoords = m_mLoader->getTextureCoords();
			m->verts = m_mLoader->getVerts();
			if(m_mLoader->getName().find_last_of("/"))
			{
				int cutOff = m_mLoader->getName().find_last_of("/");
				m_mLoader->setName( m_mLoader->getName().substr(cutOff+1));
			}
			modelPair.first  = "data\\models\\"+m_mLoader->getName();
			modelPair.second= m;
			m_model.insert(modelPair);

			break;
		case TEX:
			ss >> texName >> flipped;
			m_tLoader->LoadTexture(texName);
			if(flipped != "NOFLIP")
			{
				m_tLoader->FlipImage();
			}
			if(m_tLoader->getName().find_last_of("/"))
			{
				int cutOff = m_tLoader->getName().find_last_of("/");
				m_tLoader->setName( m_tLoader->getName().substr(cutOff+1));
			}
			texturePair.first = "data\\images\\"+m_tLoader->getName();
			texturePair.second = m_tLoader->getTexture();
			m_textures.insert(texturePair);
			flipped.clear();

			break;
		case SHA:
			ss >> shader;
			m_sLoader->LoadShader(shader,shader);
			s->fragShader = m_sLoader->getFrag();
			s->vertShader = m_sLoader->getVert();
			s->programhandle = m_sLoader->getProgramHandle();
			shaderPair.first = "data\\shaders\\"+m_sLoader->getName();
			shaderPair.second = s;
			m_shaders.insert(shaderPair);

			break;
		case UNKN:
			break;


		}




	}
	input.close();
	return true;
}*/

void ResourceManager::clearAll()
{
	m_shaders.clear();
	m_model.clear();
	m_textures.clear();
}