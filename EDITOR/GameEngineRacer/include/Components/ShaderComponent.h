#ifndef SHADERCOMPONENT_H
#define SHADERCOMPONENT_H

#include "3rdParty\gl_core_4_3.hpp"
#include "glfw3.h"
#include <fstream>

class ShaderComponent
{
private:
	std::string m_shaderName;
public:
	ShaderComponent();
	~ShaderComponent();
	void SetShader(const std::string& shaderName);
	//GLuint& GetProgramHandle(){return programHandle;};
	void Render();
	
};

#endif
