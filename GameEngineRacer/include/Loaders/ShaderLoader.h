#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include <fstream>
#include <sstream>
#include <string>
#include "3rdParty\gl_core_4_3.hpp"
#include "glfw3.h"



class ShaderLoader
{
private:
	GLuint m_fragShader;
	GLuint m_vertShader;
	GLuint m_programHandle;
	std::string m_name;

public:
	ShaderLoader();
	~ShaderLoader();
	bool LoadShader(std::string nVert, std::string nFrag);

	const std::string& getName(){return m_name;}; 
	const GLuint& getFrag(){return m_fragShader;};
	const GLuint& getVert(){return m_vertShader;};
	const GLuint& getProgramHandle(){return m_programHandle;};

};
#endif

