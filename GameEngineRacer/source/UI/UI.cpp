#include "UI\UI.h"
#include <vector>
#include <cstring>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

GLuint Text2DTextureID;
GLuint Text2DVertexBufferID;
GLuint Text2DUVBufferID;
GLuint Text2DShaderID;
GLuint Text2DUniformID;
UI::UI()
{
	rManager = ResourceManager::getInstance();
};
UI::~UI(){};
void UI::initText2D(){

	
	// Initialize VBO
	gl::GenBuffers(1, &Text2DVertexBufferID);
	gl::GenBuffers(1, &Text2DUVBufferID);
	
	// Initialize Shader
	Text2DShaderID = rManager->getShaders().at("data\\shaders\\TextVertexShader")->programhandle;

	// Initialize uniforms' IDs
	Text2DUniformID = gl::GetUniformLocation( Text2DShaderID, "myTextureSampler" );

}

void UI::printText2D(std::string text, int x, int y, int size){

	unsigned int length = text.length();

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	for ( unsigned int i=0 ; i<length ; i++ ){
		
		glm::vec2 vertex_up_left    = glm::vec2( x+i*size     , y+size );
		glm::vec2 vertex_up_right   = glm::vec2( x+i*size+size, y+size );
		glm::vec2 vertex_down_right = glm::vec2( x+i*size+size, y      );
		glm::vec2 vertex_down_left  = glm::vec2( x+i*size     , y      );

		vertices.push_back(vertex_up_left   );
		vertices.push_back(vertex_down_left );
		vertices.push_back(vertex_up_right  );

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16)/16.0f;

		glm::vec2 uv_up_left    = glm::vec2( uv_x           , uv_y );
		glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, uv_y );
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
		glm::vec2 uv_down_left  = glm::vec2( uv_x           , (uv_y + 1.0f/16.0f) );
		UVs.push_back(uv_up_left   );
		UVs.push_back(uv_down_left );
		UVs.push_back(uv_up_right  );

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	gl::BindBuffer(gl::ARRAY_BUFFER, Text2DVertexBufferID);
	gl::BufferData(gl::ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], gl::STATIC_DRAW);
	gl::BindBuffer(gl::ARRAY_BUFFER, Text2DUVBufferID);
	gl::BufferData(gl::ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], gl::STATIC_DRAW);

	// Bind shader
	gl::UseProgram(Text2DShaderID);
	// Bind texture
	gl::ActiveTexture(gl::TEXTURE0);
	gl::BindTexture(gl::TEXTURE_2D, rManager->getTexture().at("data\\images\\holstein1.png")->object());
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	gl::Uniform1i(Text2DUniformID, 0);
	gl::GenVertexArrays( 1, &vaoHandle );
	gl::BindVertexArray(vaoHandle);
	// 1rst attribute buffer : vertices
	gl::EnableVertexAttribArray(0);
	gl::BindBuffer(gl::ARRAY_BUFFER, Text2DVertexBufferID);
	gl::VertexAttribPointer(0, 2, gl::FLOAT, gl::FALSE_, 0, (void*)0 );

	// 2nd attribute buffer : UVs
	gl::EnableVertexAttribArray(1);
	gl::BindBuffer(gl::ARRAY_BUFFER, Text2DUVBufferID);
	gl::VertexAttribPointer(1, 2, gl::FLOAT, gl::FALSE_, 0, (void*)0 );

	gl::Enable(gl::BLEND);
	gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);
	
	// Draw call
	gl::DrawArrays(gl::TRIANGLES, 0, vertices.size() );
	gl::BindVertexArray(0);
	gl::Disable(gl::BLEND);

	gl::DisableVertexAttribArray(0);
	gl::DisableVertexAttribArray(1);

}

void UI::cleanupText2D(){

	// Delete buffers
	gl::DeleteBuffers(1, &Text2DVertexBufferID);
	gl::DeleteBuffers(1, &Text2DUVBufferID);

	// Delete texture
	gl::DeleteTextures(1, &Text2DTextureID);

	// Delete shader
	gl::DeleteProgram(Text2DShaderID);
}
