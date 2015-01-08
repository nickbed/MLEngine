#ifndef TEXTRENDER_H
#define TEXTRENDER_H
#include "..\resources\Shader.h"
#include "..\resources\ImageTexture.h"
#include "..\resources\ResourceManager.h"
#include "..\GLEW\include\GL\glew.h"
#include "..\GLFW\include\GLFW\glfw3.h"



class TextRender
{
public:
	TextRender();
	~TextRender();

	bool InitTextRender(int xScreen, int yScreen);
	void Draw2DText(std::string text, int x, int y, int textSize);

private:
	
	const char* texturePath;
	const char* shaderPath;

	int xScreen;
	int yScreen;

	GLuint fontTextureID;
	GLuint VBOid;
	GLuint uvBufferID;
	GLuint VAOid;

	ImageTexture* fontTexture;
	Shader* textShader;

};

#endif