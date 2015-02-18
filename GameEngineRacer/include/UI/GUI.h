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

enum LoadState {OPEN, DONTOPEN};

class GUI {
private:
	struct Object{
		///char type[32];
		glm::vec3 pos;
		glm::quat rot;
		glm::vec3 scale;
		char    Name[4];
	};
	static LoadState loadfile;
	TwType modelType;
	Object* objects;
	TwBar* bar;
	ResourceManager* rManager;
	int width;
	int height;
	Scene* m_scene;
	Json::StyledWriter writer;
	int j;
	void updateLayout();
	void openFile(std::vector<Scene*>& scene, int& activeScene);
public:
	GUI();
	~GUI();
	bool setup(int w, int h, Scene* nScene );
	void onMouseMoved(double x, double y);
	void onMouseClicked(int bt, int action);
	void onKeyPressed(int key, int mod);
	void onResize(int w, int h);
	void draw();
	void saveData();
	void checkOpenFile(std::vector<Scene*>& scene, int& activeScene);
	void update(Scene* scene);
	
	Scene* getScene(){return m_scene;};
	static void TW_CALL Save(void *clientData);
	static void TW_CALL OpenFile(void *clientData);
	//bool getRotate()const{return rotate;};
};

#endif

