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

}


