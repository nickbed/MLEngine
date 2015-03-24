#ifndef SkyBox_H
#define SkyBox_H
#include "GL\glew.h"
#include "..\Resources\ImageTexture.h"

class SkyBox
{
public:
	SkyBox();
	void LoadFile(char* filename, int imagePosition);
	void InitCubeMap();
	void InitSkybox();
	void BindTexture();
	void RenderSkybox();

private:
	char* filenames[6];
	GLuint textureIndex;
	GLuint skyBoxIndex;
	GLuint vaoIndex;
	GLuint floatCount;
};

#endif