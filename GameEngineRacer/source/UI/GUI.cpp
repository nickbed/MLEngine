#include "UI\GUI.h"

SaveState GUI::save = DONTSAVE;
FileState GUI::load = DONTOPEN;

GUI::GUI() :width(0),height(0),j(0),objects(new Object[200])
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
	save = SAVE;
}

void TW_CALL GUI::OpenFile(void *clientData)
{
	load = OPEN;
}

void GUI::openFile(Scene *nScene)
{

	if(load == OPEN)
	{
		scene = nScene;
		OPENFILENAME ofn={0};
		char szFileName[MAX_PATH]={0};
		ofn.lStructSize=sizeof(OPENFILENAME);
		ofn.Flags=OFN_EXPLORER|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
		ofn.lpstrFilter = "Obj Files\0*.OBJ\0Scene Files\0*.scn\0";
		ofn.lpstrFile=szFileName;
		ofn.nMaxFile=MAX_PATH;
		::GetOpenFileName(&ofn);

		std::string filename = ofn.lpstrFile;
		
		std::string newstring = filename.substr(filename.find_last_of("."));
		

		if(newstring == ".obj")
		{
			ModelLoader mLoader(filename);
			GameObject* g = new GameObject();
			Model* m = new Model();
			filename = filename.substr(filename.find_last_of("\\")+1);
			
			
			
			m->normals = mLoader.getNormals();
			m->verts = mLoader.getVerts();
			m->textureCoords = mLoader.getTextureCoords();
			g->addToComponentModelFiles(filename);
			g->getTransformComp()->Translate(0.0,0.0,0.0);
			g->getTransformComp()->Scale(1.0, 1.0, 1.0);
			g->getTransformComp()->Rotate(0.0, 0.0, 0.0);
			g->getRenderComp()->init(m,rManager->getTexture().at("data\\images\\default.png"),scene->getSceneData().sceneShader);
			g->setName(filename);
			scene->addGameObject(g);
			std::cout << "This is an OBJ file" << std::endl;
		}
		else if(newstring == ".scn")
		{
			std::cout << "This is a Scene file" << std::endl;
		}
			
		load = DONTOPEN;
	}

}

void GUI::saveData(Scene* nscene)
{ 
	if(save == SAVE)
	{

		OPENFILENAME ofn={0};
		char szFileName[MAX_PATH]={0};
		ofn.lStructSize=sizeof(OPENFILENAME);
		ofn.Flags=OFN_EXPLORER;
		ofn.lpstrFilter="Scene Files (*.scn)\0";
		ofn.lpstrFile=szFileName;
		ofn.nMaxFile=MAX_PATH;
		ofn.lpstrDefExt = "scn";

		GetSaveFileName(&ofn);
		
		scene = nscene;
		std::ofstream myfile;
		myfile.open (ofn.lpstrFile);
		myfile << writer.write( scene->createJson() );
		myfile.close();
		save = DONTSAVE;
	}
}



bool GUI::setup(int w, int h, Scene* nScene ) {
	width = w;
	height = h;
	scene = nScene;

	TwInit(TW_OPENGL, NULL);
	bar = TwNewBar("GameEngine");

	 TwStructMember objectMembers[] = // array used to describe tweakable variables of the Light structure
    {
        { "Translate X",    TW_TYPE_FLOAT, offsetof(Object, x),    " help='Translates the object in X.' " },   // Light::Active is a C++ boolean value
        { "Translate Y",     TW_TYPE_FLOAT, offsetof(Object, y),     " help='Translates the object in Y.' " },        // Light::Color is represented by 4 floats, but alpha channel should be ignored
        { "Translate Z",    TW_TYPE_FLOAT,   offsetof(Object, z),    " help='Translates the object in Z.' " },
        { "Rotate X", TW_TYPE_FLOAT,        offsetof(Object, xR), " help='Change the animation mode.' " },  // use the enum 'modeType' created before to tweak the Light::Animation variable
        { "Rotate Y",     TW_TYPE_FLOAT,   offsetof(Object, yR),    "  help='Light moving speed.' " }, // Light::Speed is made read-only
		{ "Rotate Z",     TW_TYPE_FLOAT,   offsetof(Object, zR),    "  help='Light moving speed.' " } // Light::Speed is made read-only
    };

	TwType modelType = TwDefineStruct("Object", objectMembers, 6, sizeof(Object), NULL, NULL);  // create a new TwType associated to the struct defined by the lightMembers array
	for(unsigned int i=0; i<scene->GetGameObjects().size(); ++i)  // Add 'maxLights' variables of type lightType; 
    {                               // unused lights variables (over NumLights) will hidden by Scene::Update( )
		objects[i].x = scene->GetGameObjects().at(i)->getTransformComp()->getTranslate().x;
		objects[i].y = scene->GetGameObjects().at(i)->getTransformComp()->getTranslate().y;
		objects[i].z = scene->GetGameObjects().at(i)->getTransformComp()->getTranslate().z;
		objects[i].xR = scene->GetGameObjects().at(i)->getTransformComp()->getRotate().x;
		objects[i].yR = scene->GetGameObjects().at(i)->getTransformComp()->getRotate().y;
		objects[i].zR = scene->GetGameObjects().at(i)->getTransformComp()->getRotate().z;
		
        
		_snprintf(objects[i].type, sizeof(objects[i].type), "%s", scene->GetGameObjects().at(i)->getEntityType().c_str());//Copy data into objects.

		 _snprintf(objects[i].Name, sizeof(objects[i].Name), "%d", i+1); //Create a unique name;

		 std::string grouping = "group="+scene->GetGameObjects().at(i)->getEntityType();//Creates the string for grouping.
		 TwAddVarRW(bar, objects[i].Name, modelType, &objects[i], grouping.c_str());//Creates Type Grouping.

        char paramValue[64];
		_snprintf(paramValue, sizeof(paramValue), "%s", scene->GetGameObjects().at(i)->getName().c_str());
        TwSetParam(bar, objects[i].Name, "label", TW_PARAM_CSTRING, 1, paramValue); // Set label
		 std::string fold = "GameEngine/"+scene->GetGameObjects().at(i)->getEntityType()+" opened='false'";
		TwDefine(fold.c_str());
       
    }
	
	

	TwDefine("GameEngine help='Press N to change camera'");
	TwAddButton(bar, "Saving", Save, NULL , " label='Save Scene' ");
	TwAddButton(bar, "OpenFiles", OpenFile, NULL , " label='Open File BROKEN!!' ");


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
void GUI::update()
{
	if(scene)
	{
		int j =0;
		for(unsigned int i = 0; i < scene->GetGameObjects().size(); ++i)
		{   
			scene->GetGameObjects().at(i)->getTransformComp()->setTranslate(glm::vec3(objects[i].x,objects[i].y,objects[i].z));
			scene->GetGameObjects().at(i)->getTransformComp()->setRotate(glm::vec3(objects[i].xR,objects[i].yR,objects[i].zR)) ;
			if(scene->GetGameObjects().at(i)->getEntityType() == "light")
			{
 				scene->getLights().at(j++).position = glm::vec3(objects[i].x,objects[i].y,objects[i].z);
			}

		}

	}
}
void GUI::draw() {

	

	TwDraw();
}
