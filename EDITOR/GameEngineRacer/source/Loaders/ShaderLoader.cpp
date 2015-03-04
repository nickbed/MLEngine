#include "Loaders\ShaderLoader.h"


ShaderLoader::ShaderLoader():m_vertShader(0), m_fragShader(0)
{
}


ShaderLoader::~ShaderLoader()
{
}
bool ShaderLoader::LoadShader(std::string nVert, std::string nFrag)
{
	//////////////////////////////////////////////////////
	/////////// Vertex shader //////////////////////////
	//////////////////////////////////////////////////////

	m_name = nVert;
	m_name.erase(nVert.find_last_of("."));

	// Load contents of file
	std::ifstream inFile( nVert );
	if (!inFile) {
		fprintf(stderr, "Error opening file: shader\n" );
		return false;
	}

	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	std::string codeStr(code.str());

	// Create the shader object
	m_vertShader = glCreateShader( GL_VERTEX_SHADER );
	if (0 == m_vertShader) {
		fprintf(stderr, "Error creating vertex shader.\n");
		return false;
	}

	// Load the source code into the shader object
	const char* codeArray[] = {codeStr.c_str()};
	glShaderSource(m_vertShader, 1, codeArray, NULL);

	// Compile the shader
	GLint result;
	glCompileShader( m_vertShader );
	glGetShaderiv( m_vertShader, GL_COMPILE_STATUS, &result );
	if (GL_FALSE == result) {
		fprintf( stderr, "Vertex shader compilation failed!\n" );

		GLint logLen;
		glGetShaderiv( m_vertShader, GL_INFO_LOG_LENGTH, &logLen );

		if (logLen > 0) {
			char * log = (char *)malloc(logLen);

			GLsizei written;
			glGetShaderInfoLog(m_vertShader, logLen, &written, log);

			fprintf(stderr, "Vertex log: \n%s", log);

			free(log);
		}
		return false;
	}

	//////////////////////////////////////////////////////
	/////////// Fragment shader //////////////////////////
	//////////////////////////////////////////////////////

	// Load contents of file into shaderCode here
	std::ifstream fragFile( nFrag );
	if (!fragFile) {
		fprintf(stderr, "Error opening file: shader\n" );
		return false;
	}

	std::stringstream fragCode;
	fragCode << fragFile.rdbuf();
	fragFile.close();
	codeStr = fragCode.str();

	// Create the shader object
	m_fragShader = glCreateShader( GL_FRAGMENT_SHADER );
	if (0 == m_fragShader) {
		fprintf(stderr, "Error creating fragment shader.\n");
		return false;
	}

	// Load the source code into the shader object
	codeArray[0] = codeStr.c_str();
	glShaderSource( m_fragShader, 1, codeArray, NULL );

	// Compile the shader
	glCompileShader( m_fragShader );
	glGetShaderiv( m_fragShader, GL_COMPILE_STATUS, &result );
	if (GL_FALSE == result) {
		fprintf( stderr, "Fragment shader compilation failed!\n" );

		GLint logLen;
		glGetShaderiv( m_fragShader, GL_INFO_LOG_LENGTH, &logLen );

		if (logLen > 0) {
			char * log = (char *)malloc(logLen);

			GLsizei written;
			glGetShaderInfoLog(m_fragShader, logLen, &written, log);

			fprintf(stderr, "Shader log: \n%s", log);

			free(log);

		}
		return false;
	}
	m_programHandle = glCreateProgram();
	if(0 == m_programHandle) {
		fprintf(stderr, "Error creating program object.\n");
		return false;
	}

	glAttachShader( m_programHandle, m_vertShader );
	glAttachShader( m_programHandle, m_fragShader );

	glLinkProgram( m_programHandle );
	glGetProgramiv(m_programHandle, GL_LINK_STATUS, &result);
	if(result == GL_FALSE)
	{
		fprintf(stderr, "Error linking the shader: %s\n", m_name);
		return false;
	}
	
	return true;
}

