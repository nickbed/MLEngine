#ifndef GUI_H
#define GUI_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <AntTweakBar.h>
#include "Scene\Scene.h"
#include <vector>
#include <assert.h>
#include "3rdParty\gl_core_4_3.hpp"
#include "glfw3.h"
#include <commdlg.h>
#include "ResourceManager.h"
#include <fstream>

enum GUIstate {OPEN, DONTOPEN, NEWSCENE};

class GUI {
private:
	struct Object{
		///char type[32];
		glm::vec3 pos;
		bool useRotBall;
		glm::quat rot;
		glm::vec3 eulers;
		glm::vec3 scale;
		char    name[4];
	};

	struct GUILight{
		glm::vec3 position;
		glm::vec3 diffuse;
	};

	static GUIstate checkEnum;
	GUILight* lights;
	Light light;
	TwType modelType, lightType;
	Object* objects;
	TwBar* bar, *lightbar,*sceneBar;

	std::vector<Scene*>* m_scenes;
	Scene* n_scene;


	int* m_activeScene;
	ResourceManager* rManager;
	int width;
	int height;
	Scene* m_scene;
	
	Json::StyledWriter writer;
	int j;
	void saveData();
	void addToLights();
	void addToScene();
	void createLight();
	void updateObjects();
	void updateLights();
	void updateBar(const std::string& name);
	void createScene();
	void openFile(std::vector<Scene*>& scene, int& activeScene);
public:
	GUI();
	~GUI();
	bool setup(int w, int h, Scene* nScene );
	void onMouseMoved(double x, double y);
	void onMouseClicked(int bt, int action);
	void onKeyPressed(int key, int mod);
	void draw();


	void checkEnums(std::vector<Scene*>& scene, int& activeScene);
	void update(Scene* scene);

	Scene* getScene(){return m_scene;};
	static void TW_CALL DeleteBar(void *clientData);
	static void TW_CALL AddtoLights(void *clientData);
	static void TW_CALL AddtoScene(void *clientData);
	static void TW_CALL NewScene(void *clientData);
	static void TW_CALL Save(void *clientData);
	static void TW_CALL OpenFile(void *clientData);
	static void TW_CALL CreateLight(void *clientData);
	static void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString);
	
};


#endif

