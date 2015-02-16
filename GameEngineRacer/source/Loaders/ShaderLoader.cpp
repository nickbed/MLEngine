#include "Loaders\ShaderLoader.h"


ShaderLoader::ShaderLoader()
{
}


ShaderLoader::~ShaderLoader()
{
}
void ShaderLoader::LoadShader(std::string nVert, std::string nFrag)
{
	//////////////////////////////////////////////////////
	/////////// Vertex shader //////////////////////////
	//////////////////////////////////////////////////////
	m_name = nVert;
	// Load contents of file
	std::ifstream inFile( "shader/"+nVert+".vert" );
	if (!inFile) {
		fprintf(stderr, "Error opening file: shader\n" );
		exit(1);
	}

	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	std::string codeStr(code.str());

	// Create the shader object
	m_vertShader = gl::CreateShader( gl::VERTEX_SHADER );
	if (0 == m_vertShader) {
		fprintf(stderr, "Error creating vertex shader.\n");
		exit(EXIT_FAILURE);
	}

	// Load the source code into the shader object
	const GLchar* codeArray[] = {codeStr.c_str()};
	gl::ShaderSource(m_vertShader, 1, codeArray, NULL);

	// Compile the shader
	GLint result;
	gl::CompileShader( m_vertShader );
	gl::GetShaderiv( m_vertShader, gl::COMPILE_STATUS, &result );
	if (FALSE == result) {
		fprintf( stderr, "Vertex shader compilation failed!\n" );

		GLint logLen;
		gl::GetShaderiv( m_vertShader, gl::INFO_LOG_LENGTH, &logLen );

		if (logLen > 0) {
			char * log = (char *)malloc(logLen);

			GLsizei written;
			gl::GetShaderInfoLog(m_vertShader, logLen, &written, log);

			fprintf(stderr, "Vertex log: \n%s", log);

			free(log);
		}
	}

	//////////////////////////////////////////////////////
	/////////// Fragment shader //////////////////////////
	//////////////////////////////////////////////////////
	
	// Load contents of file into shaderCode here
	std::ifstream fragFile( "shader/"+nFrag+".Frag" );
	if (!fragFile) {
		fprintf(stderr, "Error opening file: shader\n" );
		exit(1);
	}

	std::stringstream fragCode;
	fragCode << fragFile.rdbuf();
	fragFile.close();
	codeStr = fragCode.str();

	// Create the shader object
	m_fragShader = gl::CreateShader( gl::FRAGMENT_SHADER );
	if (0 == m_fragShader) {
		fprintf(stderr, "Error creating fragment shader.\n");
		exit(1);
	}

	// Load the source code into the shader object
	codeArray[0] = codeStr.c_str();
	gl::ShaderSource( m_fragShader, 1, codeArray, NULL );

	// Compile the shader
	gl::CompileShader( m_fragShader );
	gl::GetShaderiv( m_fragShader, gl::COMPILE_STATUS, &result );
	if (FALSE == result) {
		fprintf( stderr, "Fragment shader compilation failed!\n" );

		GLint logLen;
		gl::GetShaderiv( m_fragShader, gl::INFO_LOG_LENGTH, &logLen );

		if (logLen > 0) {
			char * log = (char *)malloc(logLen);

			GLsizei written;
			gl::GetShaderInfoLog(m_fragShader, logLen, &written, log);

			fprintf(stderr, "Shader log: \n%s", log);

			free(log);
		}
	}
	m_programHandle = gl::CreateProgram();
	if(0 == m_programHandle) {
		fprintf(stderr, "Error creating program object.\n");
		exit(1);
	}
	
	gl::AttachShader( m_programHandle, m_vertShader );
	gl::AttachShader( m_programHandle, m_fragShader );

	gl::LinkProgram( m_programHandle );
	
	
	
}

