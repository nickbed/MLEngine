#include "Components\RenderComponent.h"

RenderComponent::RenderComponent()
{
	rManager = ResourceManager::getInstance();
};

void RenderComponent::init(Model* model, Texture* nTexture, const std::string& shaderID)
{
	texture = nTexture;
	m_model = *model;
	indicesCount  = m_model.verts.size()/3;
	// Create and populate the buffer objects using separate buffers

	GLuint vboHandles[3];
	gl::GenBuffers(3, vboHandles);
	positionBufferHandle = vboHandles[0];
	normalBufferHandle = vboHandles[1];
	uvBufferHandle = vboHandles[2];

	gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferHandle);
	gl::BufferData(gl::ARRAY_BUFFER, m_model.verts.size() * sizeof(GLfloat), &m_model.verts[0], gl::STATIC_DRAW);

	gl::BindBuffer(gl::ARRAY_BUFFER, normalBufferHandle);
	gl::BufferData(gl::ARRAY_BUFFER, m_model.normals.size() * sizeof(GLfloat),&m_model.normals[0] , gl::STATIC_DRAW);

	gl::BindBuffer(gl::ARRAY_BUFFER, uvBufferHandle);
	gl::BufferData(gl::ARRAY_BUFFER,  m_model.textureCoords.size() * sizeof(GLfloat),&m_model.textureCoords[0], gl::STATIC_DRAW);
	
	// Create and set-up the vertex array object
	gl::GenVertexArrays( 1, &vaoHandle );
	gl::BindVertexArray(vaoHandle);

	gl::EnableVertexAttribArray(0);  // Vertex position
	gl::EnableVertexAttribArray(1);  // Vertex normal
	gl::EnableVertexAttribArray(2); //uv

	gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferHandle);
	gl::VertexAttribPointer( 0, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL );

	gl::BindBuffer(gl::ARRAY_BUFFER, normalBufferHandle);
	gl::VertexAttribPointer( 1, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL );

	gl::BindBuffer(gl::ARRAY_BUFFER, uvBufferHandle);
	gl::VertexAttribPointer( 2, 2, gl::FLOAT, FALSE, 0, (GLubyte *)NULL );




	
	gl::Enable(gl::TEXTURE_2D);
	gl::Enable(gl::DEPTH_TEST);
	gl::DepthFunc(gl::LESS);
	gl::Enable(gl::BLEND);

	
	gl::BindVertexArray(vaoHandle);
	gl::DrawArrays(gl::TRIANGLES, 0,indicesCount );
	/*gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::REPEAT);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_R, gl::REPEAT);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::REPEAT);*/
	
	
}
void RenderComponent::update()
{
	// Bind shader
	
	
	gl::ActiveTexture(gl::TEXTURE0);
	gl::BindTexture(gl::TEXTURE_2D,texture->object());
	
	gl::BindVertexArray(vaoHandle);
	gl::DrawArrays(gl::TRIANGLES, 0,indicesCount );
	
}
