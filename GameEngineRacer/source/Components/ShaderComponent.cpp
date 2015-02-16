#include "Components\ShaderComponent.h"



ShaderComponent::ShaderComponent()
{

}
ShaderComponent::~ShaderComponent()
{

}
void ShaderComponent::SetShader(const std::string& shaderName)
{
	// Attach the shaders to the program object

	m_shaderName = shaderName;
}
void ShaderComponent::Render()
{
	/*GLuint loc= gl::GetUniformLocation(programHandle, "frag");
	gl::Uniform1i(loc, false);*/

    //gl::UseProgram( programHandle );


	
}


