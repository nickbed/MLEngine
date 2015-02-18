#include "UI\GUI.h"

LoadState GUI::loadfile = DONTOPEN;
GUI::GUI() :width(0)
	,height(0)
	,j(0)
	,objects(new Object[200])
	, modelType()
	,bar()
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
	loadfile = OPEN;

}

void GUI::openFile(std::vector<Scene*>& scene, int& activeScene)
{

	OPENFILENAME ofn={0};
	char szFileName[MAX_PATH]={0};
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.Flags=OFN_EXPLORER|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR;
	ofn.lpstrFilter = "Obj Files\0*.OBJ\0Scene Files\0*.scn\0";
	ofn.lpstrFile=szFileName;
	ofn.nMaxFile=MAX_PATH;
	if(GetOpenFileName(&ofn))
	{
		std::string filename = ofn.lpstrFile;

		std::string newstring = filename.substr(filename.find_last_of("."));


		if(newstring == ".obj")
		{
			ModelLoader mLoader;
			GameObject* g = new GameObject();
			Model* m = new Model();
			filename = filename.substr(filename.find("data"));
			std::string name = filename.substr(filename.find_last_of("/\\")+1);
			mLoader.loadFromfile(filename);
			m->normals = mLoader.getNormals();
			m->verts = mLoader.getVerts();
			m->textureCoords = mLoader.getTextureCoords();
			rManager->addToModel(std::pair<std::string, Model*>(filename,m));
			g->setEntityType("generalentity");
			g->getTransformComp()->setTranslate(glm::vec3(0,0,0));
			g->getTransformComp()->setScale(glm::vec3(1.0, 1.0, 1.0));
			g->getTransformComp()->setRotate(glm::quat(0.0, 0.0, 0.0,0.0));

			g->setName(name);
			g->addToComponentID(name);
			g->addToComponentTYPE("mesh");
			g->addToComponentModelFiles(filename);
			g->addToComponentTextureFiles("data\\images\\default.png");
			g->getRenderComp()->init(m,rManager->getTexture().at("data\\images\\default.png"));
			g->getRenderComp()->update();
			m_scene->addGameObject(g);
			//TwRemoveAllVars(bar);
			updateObjects();



		}
		else if(newstring == ".scn")
		{
			rManager->clearAll();
			Scene* newScene = new Scene();
			//std::string name = filename.substr(filename.find();

			activeScene++;
			scene.push_back(newScene);
			scene.at(activeScene)->InitScene(filename);
			m_scene = scene[activeScene];
			updateObjects();
			//TwRefreshBar(bar);

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
void TW_CALL GUI::DeleteLightBar(void *clientData)
{
	GUI *ui = static_cast<GUI *>(clientData);
	ui->deleteLightBar();
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
void GUI::deleteLightBar()
{
	TwDeleteBar(lightbar);
}
void GUI::addToLights()
{
	m_scene->loadAndAddLightPlane(light);
	m_scene->addLight(light);
	TwDeleteBar(lightbar);
}
void TW_CALL GUI::CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
{
	destinationClientString = sourceLibraryString;
}

void GUI::createLight()
{
	lightbar = TwNewBar("New Light");
	static int i = 0;
	light.diffuse = glm::vec3(255,255,255);
	TwCopyStdStringToClientFunc(CopyStdStringToClient);
	TwAddVarRW(lightbar, "Name", TW_TYPE_STDSTRING, &light.name, "");
	TwAddVarRW(lightbar, "Colour", TW_TYPE_COLOR3F, &light.diffuse, "");
	light.position = glm::vec3(0,0,0);
	light.specular = glm::vec3(0,0,0);
	light.ambient = glm::vec3(0.2,0.2,0.2);
	light.linear = 0.014f;
	light.quadratic = 0.0007f;
	light.constant = 1.0f;
	TwAddButton(lightbar,"Create",AddtoLights,this,"");
	TwAddButton(lightbar,"Quit",DeleteLightBar,this,"");
	i++;
}
void GUI::updateObjects()
{
	///TODO CREATE LIGHTS
	for(unsigned int i=0; i<m_scene->GetGameObjects().size(); ++i)  // Add 'maxLights' variables of type lightType; 
	{                               // unused lights variables (over NumLights) will hidden by Scene::Update( )
		objects[i].useRotBall = false;
		objects[i].pos = m_scene->GetGameObjects().at(i)->getTransformComp()->getTranslate();
		objects[i].rot = m_scene->GetGameObjects().at(i)->getTransformComp()->getRotate();
		objects[i].scale = m_scene->GetGameObjects().at(i)->getTransformComp()->getScale();
		objects[i].eulers = glm::degrees(glm::eulerAngles(m_scene->GetGameObjects().at(i)->getTransformComp()->getRotate()));


		//_snprintf(objects[i].type, sizeof(objects[i].type), "%s", m_scene->GetGameObjects().at(i)->getEntityType().c_str());//Copy data into objects.
		_snprintf(objects[i].Name, sizeof(objects[i].Name), "%d", i+1); //Create a unique name;


		std::string grouping = "group="+m_scene->GetGameObjects().at(i)->getEntityType();
		TwAddVarRW(bar, objects[i].Name, modelType, &objects[i], grouping.c_str());//Creates Type Grouping.
		TwSetParam(bar, objects[i].Name, "label", TW_PARAM_CSTRING, 1, m_scene->GetGameObjects().at(i)->getName().c_str()); // Set label


		std::string entityType = "Editor/"+m_scene->GetGameObjects().at(i)->getName()+" group="+m_scene->GetGameObjects().at(i)->getEntityType()+" opened='false'";//Creates the string for grouping.
		TwDefine(entityType.c_str());


		std::string fold = "Editor/"+m_scene->GetGameObjects().at(i)->getEntityType()+" opened='false'";
		TwDefine(fold.c_str());

	}
}


bool GUI::setup(int w, int h, Scene* nScene ) {
	width = w;
	height = h;
	m_scene = nScene;

	TwInit(TW_OPENGL_CORE, NULL );
	bar = TwNewBar("Editor");
	TwDefine(" Editor color='192 255 192' text=dark ");
	TwStructMember objectMembers[] = 
	{
		{ "Translate X",    TW_TYPE_FLOAT, offsetof(Object, pos.x),    " help='Translates the object in X.' step=0.1" },   
		{ "Translate Y",     TW_TYPE_FLOAT, offsetof(Object, pos.y),     " help='Translates the object in Y.' step=0.1" }, 
		{ "Translate Z",    TW_TYPE_FLOAT,   offsetof(Object, pos.z),    " help='Translates the object in Z.' step=0.1" },
		{ "Switch Rotation",    TW_TYPE_BOOLCPP,   offsetof(Object, useRotBall),    "false='Euler' true='RotBall' help='Toggles the use of the rotation ball'" },
		{ "Rotate ", TW_TYPE_QUAT4F,        offsetof(Object, rot), "readonly=false "  },
		{ "Rotate X", TW_TYPE_FLOAT,        offsetof(Object, eulers.x), "readonly=false precision=2" },
		{ "Rotate Y", TW_TYPE_FLOAT,        offsetof(Object, eulers.y), "readonly=false precision=2" },
		{ "Rotate Z", TW_TYPE_FLOAT,        offsetof(Object, eulers.z), "readonly=false precision=2" },
		{ "Scale X", TW_TYPE_FLOAT,        offsetof(Object, scale.x), " help='Scale in the X.' step=0.01" },  
		{ "Scale Y",     TW_TYPE_FLOAT,   offsetof(Object, scale.y),    " help='Scale in the Y.' step=0.01" },
		{ "Scale Z",     TW_TYPE_FLOAT,   offsetof(Object, scale.z),    " help='Scale in the Z.' step=0.01" } 
	};
	TwStructMember lightMembers[] = 
	{
		{ "Translate X",    TW_TYPE_FLOAT, offsetof(Light, position.x),    " help='Translates the object in X.' step=0.1" }, 
		{ "Translate X",    TW_TYPE_FLOAT, offsetof(Light, position.y),    " help='Translates the object in X.' step=0.1" },
		{ "Translate X",    TW_TYPE_FLOAT, offsetof(Light, position.z),    " help='Translates the object in X.' step=0.1" },
		{ "Colour",    TW_TYPE_COLOR3F, offsetof(Light, diffuse),    " help='Change the colour of the light.'" },
	};

	modelType = TwDefineStruct("Object", objectMembers, 11, sizeof(Object), NULL, NULL);
	lightType = TwDefineStruct("Light", lightMembers,4,sizeof(Light),NULL,NULL);
	updateObjects();
	TwDefine("GLOBAL help='Press N to change camera.\nHold alt to move the camera.'");
	TwAddButton(bar, "Saving", Save, this , " label='Save Scene' ");
	TwAddButton(bar, "OpenFiles", OpenFile, NULL , " label='Open File' ");
	TwAddButton(bar, "createLights", CreateLight, this , " label='Create Light' ");
	TwWindowSize(width, height);
	return true;
}

void GUI::onResize(int w, int h) {
	width = w;
	height = h;
	TwWindowSize(w, h);
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
void GUI::checkOpenFile(std::vector<Scene*>& scene, int& activeScene)
{
	if(loadfile == OPEN)
	{
		openFile(scene, activeScene);
		loadfile = DONTOPEN;
	}
}
void GUI::update(Scene* nscene)
{

	m_scene=nscene;
	if(m_scene)
	{
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

	}
}
void GUI::draw() {



	TwDraw();


}
