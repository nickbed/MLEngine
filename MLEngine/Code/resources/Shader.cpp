#include "Shader.h"

Shader::Shader()
{
	loadedShaderProgramID = 0;
	vertexSource = "";
	fragmentSource = "";
}

Shader::~Shader()
{
	if(vertexSource != "" && fragmentSource != "")
	{
		glDeleteShader(loadedShaderProgramID);
	}
}

Shader::Shader(std::string file) : IResource(file)
{
	bool result = LoadFromFile(file);
	result &= mauveassert::Assert::AssertTrue("Error loading shader from file", result, mauveassert::ENUM_severity::SEV_ERROR);
	if(!result) return;
}

bool Shader::LoadFromFile(std::string filename)
{
	std::string fragFile = filename + ".frag";
	std::string vertFile = filename + ".vert";

	//Load fragment shader
	std::string fragResultStr = mauvefile::FileManager::ReadFile(fragFile.c_str(), true);
	bool result1 = mauveassert::Assert::AssertTrue("Failed to read Fragment Shader file", (fragResultStr != ""), mauveassert::ENUM_severity::SEV_ERROR);
	if(!result1) return false;
	

	//Load vertex shader
	std::string vertResultStr = mauvefile::FileManager::ReadFile(vertFile.c_str(), true);
	bool result2 = mauveassert::Assert::AssertTrue("Failed to read Vertex Shader file", (vertResultStr != ""), mauveassert::ENUM_severity::SEV_ERROR);
	if(!result2) return false;

	fragmentSource = fragResultStr;
	vertexSource = vertResultStr;
	
	bool result3 = CompileAndLinkShader(vertexSource, fragmentSource, loadedShaderProgramID);
	return result3;
}

bool Shader::SendUniformMat4(const char* uniformName, const glm::mat4 matrix)
{
	GLint uniformLocation = GetUniformLocation(uniformName);
	if(!mauveassert::Assert::AssertTrue("Got -1 from getting uniform location. Unable to send data to shader.", uniformLocation != -1, mauveassert::ENUM_severity::SEV_ERROR)) return false;
	glUniformMatrix4fv(uniformLocation, 1, false, &matrix[0][0]);
	return true;
}

bool Shader::SendUniformVec4(const char* uniformName, const glm::vec4 vec)
{
	GLint uniformLocation = GetUniformLocation(uniformName);
	if(!mauveassert::Assert::AssertTrue("Got -1 from getting uniform location. Unable to send data to shader.", uniformLocation != -1, mauveassert::ENUM_severity::SEV_ERROR)) return false;
	glUniform4f(uniformLocation, vec.x, vec.y, vec.z, vec.w);
	return true;
}

void Shader::UseShader()
{
	glUseProgram(loadedShaderProgramID);
}

GLint Shader::GetUniformLocation(const char* uniformName)
{
	if (uniformCache.find(uniformName) == uniformCache.end())
	{
		GLchar* tempUniform = (GLchar*)uniformName + '\0';
		GLint result = glGetUniformLocation(loadedShaderProgramID, tempUniform);
		if (result == -1) return result;
		uniformCache.insert(std::pair<const char*, GLint>(uniformName, result));
		DEBUGWRITEINFO("Caching Uniform Location:", uniformName)
		return result;
	}
	return uniformCache.find(uniformName)->second;
}

bool Shader::CompileAndLinkShader(std::string vertShader, std::string fragShader, GLuint& programID)
{
	if(vertShader == "" || fragShader == "")
	{
		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_ERROR, "Fragment or vertex shader source empty!");
		return false;
	}

	const GLchar* charVertSource = vertShader.c_str();
	const GLchar* charFragSource = fragShader.c_str();

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShaderID, 1, &charVertSource, NULL);
	glShaderSource(fragmentShaderID, 1, &charFragSource, NULL);

	bool result = true;

	result &= mauveassert::Assert::AssertTrue("Compile vertex shader", CompileShader(vertexShaderID), mauveassert::ENUM_severity::SEV_ERROR);
	result &= mauveassert::Assert::AssertTrue("Compile fragment shader", CompileShader(fragmentShaderID), mauveassert::ENUM_severity::SEV_ERROR);
	if(!result) return false;

	loadedShaderProgramID = glCreateProgram();
	glAttachShader(loadedShaderProgramID, vertexShaderID);
	glAttachShader(loadedShaderProgramID, fragmentShaderID);

	glLinkProgram(loadedShaderProgramID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	return true;
}

bool Shader::CompileShader(GLuint shaderID)
{
	glCompileShader(shaderID);
	GLint compileResult = -1;
	GLint logLen = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE)
	{
		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_ERROR, "Error compiling shader");
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char * log = (char *)malloc(logLen);

			GLsizei written;
			glGetShaderInfoLog(shaderID, logLen, &written, log);

			fprintf(stderr, "Shader log: \n%s", log);

			free(log);
		}
		return false;
	}
	return true;
}

