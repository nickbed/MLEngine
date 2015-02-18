#include "UI\GUI.h"

LoadState GUI::loadfile = DONTOPEN;
GUI::GUI() :width(0),height(0),j(0),objects(new Object[200]), modelType()
{
	rManager = ResourceManager::getInstance();

}

GUI::~GUI()
{
	delete objects;
	objects = NULL;
	TwTerminate();
}

void TW_CALL GUI::Save(void *clientData)
{ 
	GUI *ui = static_cast<GUI *>(clientData);
	ui->saveData();
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
			updateLayout();



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
			TwRefreshBar(bar);
			updateLayout();
			
		}


	}

}

void GUI::saveData()
{ 

	OPENFILENAME ofn={0};
	char szFileName[MAX_PATH]={0};
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.Flags=OFN_EXPLORER;
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

void GUI::updateLayout()
{
	TwDefine("GLOBAL help='Press N to change camera.\nHold alt to move the camera.'");
	TwAddButton(bar, "Saving", Save, this , " label='Save Scene' ");
	TwAddButton(bar, "OpenFiles", OpenFile, NULL , " label='Open File' ");

	for(unsigned int i=0; i<m_scene->GetGameObjects().size(); ++i)  // Add 'maxLights' variables of type lightType; 
	{                               // unused lights variables (over NumLights) will hidden by Scene::Update( )
		objects[i].pos.x = m_scene->GetGameObjects().at(i)->getTransformComp()->getTranslate().x;
		objects[i].pos.y = m_scene->GetGameObjects().at(i)->getTransformComp()->getTranslate().y;
		objects[i].pos.z = m_scene->GetGameObjects().at(i)->getTransformComp()->getTranslate().z;
		objects[i].rot = m_scene->GetGameObjects().at(i)->getTransformComp()->getRotate();
		objects[i].scale.x = m_scene->GetGameObjects().at(i)->getTransformComp()->getScale().x;
		objects[i].scale.y = m_scene->GetGameObjects().at(i)->getTransformComp()->getScale().y;
		objects[i].scale.z = m_scene->GetGameObjects().at(i)->getTransformComp()->getScale().z;

		

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
	TwStructMember objectMembers[] = // array used to describe tweakable variables of the Light structure
	{
		{ "Translate ",    TW_TYPE_FLOAT, offsetof(Object, pos),    " help='Translates the object in X.' step=0.1" },   
		{ "Translate Y",     TW_TYPE_FLOAT, offsetof(Object, pos.y),     " help='Translates the object in Y.' step=0.1" }, 
		{ "Translate Z",    TW_TYPE_FLOAT,   offsetof(Object, pos.z),    " help='Translates the object in Z.' step=0.1" },
		{ "Rotate ", TW_TYPE_QUAT4F,        offsetof(Object, rot), " help='Rotate .' showval=true " },
		{ "Scale X", TW_TYPE_FLOAT,        offsetof(Object, scale.x), " help='Scale in the X.' step=0.01" },  
		{ "Scale Y",     TW_TYPE_FLOAT,   offsetof(Object, scale.y),    " help='Scale in the Y.' step=0.01" },
		{ "Scale Z",     TW_TYPE_FLOAT,   offsetof(Object, scale.z),    " help='Scale in the Z.' step=0.01" } 
	};

	modelType = TwDefineStruct("Object", objectMembers, 7, sizeof(Object), NULL, NULL);  // create a new TwType associated to the struct defined by the lightMembers array
	updateLayout();


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
			m_scene->GetGameObjects().at(i)->getTransformComp()->setTranslate(objects[i].pos);
			m_scene->GetGameObjects().at(i)->getTransformComp()->setRotate(objects[i].rot);
			m_scene->GetGameObjects().at(i)->getTransformComp()->setScale(objects[i].scale) ;
			if(m_scene->GetGameObjects().at(i)->getEntityType() == "light")
			{
				m_scene->getLights().at(j++).position = glm::vec3(objects[i].pos.x,objects[i].pos.y,objects[i].pos.z);
			}

		}

	}
}
void GUI::draw() {



	TwDraw();


}
