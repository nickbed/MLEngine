#include "UI\GUI.h"

GUIstate GUI::checkEnum = DONTOPEN;
GUI::GUI() :width(0)
	,height(0)
	,j(0)
	,objects(new Object[200])
	, modelType()
	,bar()
	,lights(new GUILight[10])
{
	rManager = ResourceManager::getInstance();
}

GUI::~GUI()
{
	TwDeleteAllBars();
	delete objects;
	objects = NULL;
	TwTerminate();
}



void TW_CALL GUI::OpenFile(void *clientData)
{
	checkEnum = OPEN;
}

void GUI::openFile(std::vector<Scene*>& scene, int& activeScene)
{

	OPENFILENAME ofn={0};
	char szFileName[MAX_PATH]={0};
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.Flags=OFN_EXPLORER|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR;
	ofn.lpstrFilter = "Obj Files (*obj)\0*.OBJ\0Scene Files (*.scn)\0*.scn\0";

	ofn.lpstrFile=szFileName;
	ofn.nMaxFile=MAX_PATH;
	if(GetOpenFileName(&ofn))
	{
		std::string filename = ofn.lpstrFile;

		std::string newstring = filename.substr(filename.find_last_of("."));


		if(newstring == ".obj")
		{
			/////TODO NEED TO CHECK IF MODEL IS ALREADY LOADED


			filename = filename.substr(filename.find("data"));
			std::string name = filename.substr(filename.find_last_of("/\\")+1);
			if (rManager->getModel_const().find(filename) == rManager->getModel_const().end() ) 
			{
				ModelLoader mLoader;
				Model* m = new Model();
				mLoader.loadFromfile(filename);
				m->normals = mLoader.getNormals();
				m->verts = mLoader.getVerts();
				m->textureCoords = mLoader.getTextureCoords();
				rManager->addToModel(std::pair<std::string, Model*>(filename,m));
			}
			GameObject* g = new GameObject();
			g->setEntityType("generalentity");
			g->getTransformComp()->setTranslate(glm::vec3(0,0,0));
			g->getTransformComp()->setScale(glm::vec3(1.0, 1.0, 1.0));
			g->getTransformComp()->setRotate(glm::quat(0.0, 0.0, 0.0,0.0));
			g->setName(name);
			g->addToComponentID(name);
			g->addToComponentTYPE("mesh");
			g->addToComponentModelFiles(filename);
			g->getRenderComp()->init(rManager->getModel().at(filename),rManager->getTexture().at("data\\images\\default.png"));
			g->getRenderComp()->update();
			m_scene->addGameObject(g);
			TwRemoveAllVars(bar);
			updateBar(m_scene->getSceneData().name);
			updateObjects();
			updateLights();
		}
		else if(newstring == ".scn")
		{

			std::string checkName = filename.substr(filename.find("data"));
			for(int i=0; i < scene.size(); ++i)
			{
				if(scene.at(i)->getFileName() == checkName)
				{
					activeScene = i;
					m_scene = scene[activeScene];
					TwRemoveAllVars(bar);
					updateBar(m_scene->getSceneData().name);
					updateObjects();
					updateLights();
					return;
				}

			}
			//rManager->clearAll();
			Scene* newScene = new Scene();
			//std::string name = filename.substr(filename.find();
			activeScene++;
			scene.push_back(newScene);
			scene.at(activeScene)->InitScene(checkName);
			m_scene = scene[activeScene];
			TwRemoveAllVars(bar);
			updateBar(m_scene->getSceneData().name);
			updateObjects();
			updateLights();

			TwRefreshBar(bar);
		}


	}

}

void GUI::saveData()
{ 

	OPENFILENAME ofn={0};
	char szFileName[MAX_PATH]={0};
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.Flags=OFN_EXPLORER|OFN_OVERWRITEPROMPT;
	ofn.lpstrFilter="Scene Files (*.scn)\0";
	ofn.lpstrFile=szFileName;
	ofn.nMaxFile=MAX_PATH;
	ofn.lpstrDefExt = "scn";

	if(GetSaveFileName(&ofn))
	{
		std::ofstream myfile;
		myfile.open (ofn.lpstrFile);
		myfile << writer.write( m_scene->createJson() );
		myfile.close();
	}
}

void TW_CALL GUI::Save(void *clientData)
{ 
	GUI *ui = static_cast<GUI *>(clientData);
	ui->saveData();
}

void TW_CALL GUI::DeleteBar(void *clientData)
{
	TwBar *t = static_cast<TwBar *>(clientData);
	TwDeleteBar(t);
}

void TW_CALL GUI::AddtoLights(void *clientData)
{
	GUI *ui = static_cast<GUI *>(clientData);
	ui->addToLights();
}
void TW_CALL GUI::CreateLight(void *clientData)
{
	GUI *ui = static_cast<GUI *>(clientData);
	ui->createLight();
}
void TW_CALL GUI::TextureCB(void *clientData)
{
	GameObject *g = static_cast<GameObject *>(clientData);
	g->getRenderComp()->loadTexture(g);
}

void GUI::addToLights()
{
	m_scene->loadAndAddLightPlane(light);
	m_scene->addLight(light);
	updateLights();
	TwDeleteBar(lightbar);
}
void TW_CALL GUI::CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
{
	destinationClientString = sourceLibraryString;
}

void GUI::createLight()
{
	lightbar = TwNewBar("New Light");
	TwDefine("'New Light' position='260 40' color='254 92 99' ");
	static int i = 0;
	light.diffuse = glm::vec3(1,1,1);
	TwCopyStdStringToClientFunc(CopyStdStringToClient);
	TwAddVarRW(lightbar, "Name", TW_TYPE_STDSTRING, &light.name, "");
	TwAddVarRW(lightbar, "Colour", TW_TYPE_COLOR3F, &light.diffuse, "");
	light.position = glm::vec3(0,2,0);
	light.specular = glm::vec3(0,0,0);
	light.ambient = glm::vec3(0.2,0.2,0.2);
	light.linear = 0.014f;
	light.quadratic = 0.0007f;
	light.constant = 1.0f;
	TwAddButton(lightbar,"Create",AddtoLights,this,"");
	TwAddButton(lightbar,"Quit",DeleteBar,this->lightbar,"");
	i++;
}

void GUI::updateBar(const std::string& name)
{
	std::string def =  " label='Scene Name: "+name+"' ";
	TwAddButton(bar, "Info", NULL, NULL, def.c_str());
	TwAddButton(bar, "NewScene", NewScene, this,"label='New Scene'");
	TwAddButton(bar, "Saving", Save, this , " label='Save Scene' ");
	TwAddButton(bar, "OpenFiles", OpenFile, NULL , " label='Open File' ");
	TwAddButton(bar, "createLights", CreateLight, this , " label='Create Light' ");
}
void GUI::updateObjects()
{


	for(unsigned int i=0; i<m_scene->GetGameObjects().size(); ++i) 
	{           



		objects[i].useRotBall = false;
		objects[i].pos = m_scene->GetGameObjects().at(i)->getTransformComp()->getTranslate();
		objects[i].rot = m_scene->GetGameObjects().at(i)->getTransformComp()->getRotate();
		objects[i].scale = m_scene->GetGameObjects().at(i)->getTransformComp()->getScale();
		objects[i].eulers = glm::degrees(glm::eulerAngles(m_scene->GetGameObjects().at(i)->getTransformComp()->getRotate()));

		_snprintf(objects[i].name, sizeof(objects[i].name), "%d", i+1); //Create a unique name;

		std::string grouping = "group="+m_scene->GetGameObjects().at(i)->getEntityType();
		TwAddVarRW(bar, objects[i].name, modelType, &objects[i], grouping.c_str());//Creates Type Grouping.
		TwSetParam(bar, objects[i].name, "label", TW_PARAM_CSTRING, 1, m_scene->GetGameObjects().at(i)->getName().c_str()); // Set label

		/////////LOAD TEXTURE BUTTON
		TwAddButton(bar,  "Texture" , TextureCB, m_scene->GetGameObjects().at(i),"" );
		std::string buttongroup = objects[i].name;
		std::string grouping2 = " Editor/Texture  group="+buttongroup;
		std::string grouping3 =	" Editor/"+buttongroup+" group="+m_scene->GetGameObjects().at(i)->getEntityType();
		TwDefine(grouping2.c_str());  
		TwDefine(grouping3.c_str());  
		////////////////////////////////


		std::string fold = "Editor/"+m_scene->GetGameObjects().at(i)->getEntityType()+" opened='false'";
		TwDefine(fold.c_str());

	}
}

void GUI::updateLights()
{
	for(unsigned int i=0; i<m_scene->getLights().size(); ++i)  // Add 'maxLights' variables of type lightType; 
	{                               // unused lights variables (over NumLights) will hidden by Scene::Update( )

		lights[i].diffuse = m_scene->getLights().at(i).diffuse;
		lights[i].position =  m_scene->getLights().at(i).position;

		std::string uniqueName = m_scene->getLights().at(i).name + std::to_string(i);
		std::string grouping = "group=Lights";
		TwAddVarRW(bar, uniqueName.c_str(), lightType, &lights[i], grouping.c_str());//Creates Type Grouping.
		TwSetParam(bar, uniqueName.c_str(), "label", TW_PARAM_CSTRING, 1, m_scene->getLights().at(i).name.c_str()); // Set label
		std::string fold = "Editor/Lights opened='false'";
		TwDefine(fold.c_str());

	}
}
bool GUI::setup(int w, int h, Scene* nScene ) {
	width = w;
	height = h;
	m_scene = nScene;

	TwInit(TW_OPENGL_CORE, NULL );
	bar = TwNewBar("Editor");
	TwDefine("'Editor' position='20 20' color='254 92 99'  ");
	TwStructMember objectMembers[] = 
	{
		{ "Translate X",    TW_TYPE_FLOAT, offsetof(Object, pos.x),    " help='Translates the object in X.' step=0.1 precision=2" },   
		{ "Translate Y",     TW_TYPE_FLOAT, offsetof(Object, pos.y),     " help='Translates the object in Y.' step=0.1 precision=2" }, 
		{ "Translate Z",    TW_TYPE_FLOAT,   offsetof(Object, pos.z),    " help='Translates the object in Z.' step=0.1 precision=2" },
		{ "Switch Rotation",    TW_TYPE_BOOLCPP,   offsetof(Object, useRotBall),    "false='Euler' true='RotBall' help='Toggles the use of the rotation ball'" },
		{ "Rotate ", TW_TYPE_QUAT4F,        offsetof(Object, rot), "readonly=false "  },
		{ "Rotate X", TW_TYPE_FLOAT,        offsetof(Object, eulers.x), "readonly=false precision=2" },
		{ "Rotate Y", TW_TYPE_FLOAT,        offsetof(Object, eulers.y), "readonly=false precision=2" },
		{ "Rotate Z", TW_TYPE_FLOAT,        offsetof(Object, eulers.z), "readonly=false precision=2" },
		{ "Scale X", TW_TYPE_FLOAT,        offsetof(Object, scale.x), " help='Scale in the X.' step=0.01" },  
		{ "Scale Y",     TW_TYPE_FLOAT,   offsetof(Object, scale.y),    " help='Scale in the Y.' step=0.01" },
		{ "Scale Z", TW_TYPE_FLOAT,   offsetof(Object, scale.z),    " help='Scale in the Z.' step=0.01" } 
	};
	TwStructMember lightMembers[] = 
	{
		{ "Translate X",    TW_TYPE_FLOAT, offsetof(GUILight, position.x),    " help='Translates the object in X.' step=0.1 precision=2" }, 
		{ "Translate Y",    TW_TYPE_FLOAT, offsetof(GUILight, position.y),    " help='Translates the object in Y.' step=0.1 precision=2" },
		{ "Translate Z",    TW_TYPE_FLOAT, offsetof(GUILight, position.z),    " help='Translates the object in Z.' step=0.1 precision=2" },
		{ "Colour",    TW_TYPE_COLOR3F, offsetof(GUILight, diffuse),    " help='Change the colour of the light.'" }
	};

	modelType = TwDefineStruct("Object", objectMembers, 11, sizeof(Object), NULL, NULL);
	lightType = TwDefineStruct("Light", lightMembers,4,sizeof(GUILight),NULL,NULL);
	TwAddButton(bar, "NewScene", NewScene, this,"label='New Scene'");
	TwAddButton(bar, "OpenFiles", OpenFile, NULL , " label='Open File' ");
	TwDefine("GLOBAL help='Press N to change camera.\nHold alt to move the camera.'");

	TwWindowSize(width, height);
	return true;
}
void GUI::createScene()
{
	sceneBar = TwNewBar("New Scene");
	TwDefine("'New Scene' position='260 40' color='254 92 99' ");
	n_scene = new Scene();

	//n_scene->setSceneData().menu = false;
	TwCopyStdStringToClientFunc(CopyStdStringToClient);
	TwAddVarRW(sceneBar, "Name", TW_TYPE_STDSTRING, &n_scene->setSceneData().name, "");
	TwAddVarRW(sceneBar, "Menu", TW_TYPE_BOOLCPP, &n_scene->setSceneData().menu, "help='Declares is this is a Menu Scene' true='YES' false='NO'");
	TwAddButton(sceneBar,"Create Scene", AddtoScene, this, "");
	TwAddButton(sceneBar,"Quit",DeleteBar,sceneBar,"");

}
void GUI::addToScene()
{
	m_scenes->push_back(n_scene);
	(*m_activeScene)++;
	m_scenes->at(*m_activeScene)->InitScene(n_scene->setSceneData().name);
	TwDeleteBar(sceneBar);
	TwRemoveAllVars(bar);
	updateBar(m_scenes->at(*m_activeScene)->getSceneData().name);
}
void TW_CALL GUI::AddtoScene(void *clientData)
{
	GUI *ui = static_cast<GUI *>(clientData);
	ui->addToScene();
}
void TW_CALL GUI::NewScene(void *clientData)
{
	checkEnum = NEWSCENE;
	GUI *ui = static_cast<GUI *>(clientData);
	ui->createScene();
}

void GUI::onMouseMoved(double x, double y) {
	TwMouseMotion((int)x, (int)y);
}

void GUI::onMouseClicked(int bt, int action) {
	TwMouseButtonID btn = (bt == 0) ? TW_MOUSE_LEFT : TW_MOUSE_RIGHT;
	TwMouseAction ma = (action == GLFW_PRESS) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;
	TwMouseButton(ma, btn);
}

void GUI::onKeyPressed(int key, int mod) {

	switch(key) {
	case GLFW_KEY_LEFT: key = TW_KEY_LEFT; break;
	case GLFW_KEY_RIGHT: key = TW_KEY_RIGHT; break;
	case GLFW_KEY_UP: key = TW_KEY_UP; break;
	case GLFW_KEY_DOWN: key = TW_KEY_DOWN; break;
	default: break;
	}

	int tw_mod = TW_KMOD_NONE;

	if(mod & GLFW_MOD_SHIFT) {
		tw_mod |= TW_KMOD_SHIFT;
	}

	if(mod & GLFW_MOD_CONTROL) {
		tw_mod |= TW_KMOD_CTRL;
	}

	if(mod & GLFW_MOD_ALT) {
		tw_mod |= TW_KMOD_ALT;
	}

	TwKeyPressed(key, TW_KMOD_NONE);
}
void GUI::checkEnums(std::vector<Scene*>& scene, int& activeScene)
{
	if(checkEnum == OPEN)
	{
		m_activeScene = &activeScene;
		openFile(scene, activeScene);
		checkEnum = DONTOPEN;
	}
	else if(checkEnum == NEWSCENE)
	{
		m_activeScene = &activeScene;
		m_scenes = &scene;
		checkEnum = DONTOPEN;
	}
}
void GUI::update(Scene* nscene)
{

	m_scene=nscene;
	int j =0;
	for(unsigned int i = 0; i < m_scene->GetGameObjects().size(); ++i)
	{   
		if(!objects[i].useRotBall)
		{
			objects[i].rot = glm::quat(glm::radians(objects[i].eulers));

		}
		else
		{
			objects[i].eulers = glm::degrees(glm::eulerAngles(objects[i].rot));

		}
		m_scene->GetGameObjects().at(i)->getTransformComp()->setTranslate(objects[i].pos);
		m_scene->GetGameObjects().at(i)->getTransformComp()->setRotate(objects[i].rot);
		m_scene->GetGameObjects().at(i)->getTransformComp()->setScale(objects[i].scale) ;
	}
	for(unsigned int i=0; i< m_scene->getLights().size(); ++i)
	{
		m_scene->getLights().at(i).position = lights[i].position;
		m_scene->getLights().at(i).diffuse = lights[i].diffuse;
		m_scene->getLightObjects().at(i)->getTransformComp()->setTranslate(lights[i].position);
	}


}
void GUI::draw() {

	TwDraw();
}
