#include "SkyBox.h"

SkyBox::SkyBox()
{
}

void SkyBox::LoadFile(char* filename, int imagePosition)
{
	//Set filename in array at specified position
	filenames[imagePosition] = filename;
}

void SkyBox::InitCubeMap()
{
	//Generate and bind a cubemap texture
	glGenTextures(1, &textureIndex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureIndex);


	for(int i = 0; i < 6; ++i)
	{
		//Create a bitmap object
		Bitmap bmp = Bitmap::bitmapFromFile(filenames[i]);
		//bmp.flipVertically();
		GLenum cubeMapPos = GL_NONE;

		//Select side of cube depending on i
		switch(i)
		{
		case 0:
			//Top of cube
			cubeMapPos = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		break;
		case 1:
			//Left
			cubeMapPos = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		break;
		case 2:
			//Forward
			cubeMapPos = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		break;
		case 3:
			//Right
			cubeMapPos = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		break;
		case 4:
			//Back
			cubeMapPos = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
		break;
		case 5:
			//Bottom
			cubeMapPos = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
			break;
		}

		//Load texture in
		glTexImage2D(cubeMapPos,
				 0,
                 GL_RGB,
                 (GLsizei)bmp.width(), 
                 (GLsizei)bmp.height(),
                 0, 
                 GL_RGB, 
                 GL_UNSIGNED_BYTE, 
                 bmp.pixelBuffer());
	}

	//Setup cube map texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void SkyBox::BindTexture()
{
	//Called before rendering 
	glActiveTexture(textureIndex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureIndex);
}

void SkyBox::InitSkybox()
{
	GLfloat skyboxVertices[] =
    { 
		//Front
   -10.0f,  10.0f, -10.0f,
  -10.0f, -10.0f, -10.0f,
   10.0f, -10.0f, -10.0f,
   10.0f, -10.0f, -10.0f,
   10.0f,  10.0f, -10.0f,
  -10.0f,  10.0f, -10.0f,
  
  //Left
  -10.0f, -10.0f,  10.0f,
  -10.0f, -10.0f, -10.0f,
  -10.0f,  10.0f, -10.0f,
  -10.0f,  10.0f, -10.0f,
  -10.0f,  10.0f,  10.0f,
  -10.0f, -10.0f,  10.0f,
  
  //Right
   10.0f, -10.0f, -10.0f,
   10.0f, -10.0f,  10.0f,
   10.0f,  10.0f,  10.0f,
   10.0f,  10.0f,  10.0f,
   10.0f,  10.0f, -10.0f,
   10.0f, -10.0f, -10.0f,
   
   //Back
  -10.0f, -10.0f,  10.0f,
  -10.0f,  10.0f,  10.0f,
   10.0f,  10.0f,  10.0f,
   10.0f,  10.0f,  10.0f,
   10.0f, -10.0f,  10.0f,
  -10.0f, -10.0f,  10.0f,
  
  //Top
  -10.0f,  10.0f, -10.0f,
   10.0f,  10.0f, -10.0f,
   10.0f,  10.0f,  10.0f,
   10.0f,  10.0f,  10.0f,
  -10.0f,  10.0f,  10.0f,
  -10.0f,  10.0f, -10.0f,
  
  //Bottom
  -10.0f, -10.0f, -10.0f,
  -10.0f, -10.0f,  10.0f,
   10.0f, -10.0f, -10.0f,
   10.0f, -10.0f, -10.0f,
  -10.0f, -10.0f,  10.0f,
   10.0f, -10.0f,  10.0f
    };

	GLfloat skyboxNormals[] =
    {
	//Front
    0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,


  
  //Left
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  
  //Right
   -1.0f, 0.0f, 0.0f,
   -1.0f, 0.0f, 0.0f,
   -1.0f, 0.0f, 0.0f,
   -1.0f, 0.0f, 0.0f,
   -1.0f, 0.0f, 0.0f,
   -1.0f, 0.0f, 0.0f,
   
   //Back
  0.0f, 0.0f, 1.0f,
  0.0f, 0.0f, 1.0f,
  0.0f, 0.0f, 1.0f,
  0.0f, 0.0f, 1.0f,
  0.0f, 0.0f, 1.0f,
  0.0f, 0.0f, 1.0f,
  
  //Top
  0.0f, -1.0f, 0.0f,
  0.0f, -1.0f, 0.0f,
  0.0f, -1.0f, 0.0f,
  0.0f, -1.0f, 0.0f,
  0.0f, -1.0f, 0.0f,
  0.0f, -1.0f, 0.0f,
  
  //Bottom
  0.0f, 1.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
    };

	glGenVertexArrays( 1, &vaoIndex );
    glBindVertexArray(vaoIndex);
	GLuint buffers[2]; 
	
	floatCount = sizeof(skyboxVertices);

    glGenBuffers(2, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, floatCount, skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, FALSE, 0, NULL);
    glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, floatCount, skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer( (GLuint)1, 3, GL_FLOAT, FALSE, 0, NULL );
    glEnableVertexAttribArray(1);  // Vertex normal

	glBindVertexArray(0);
}

void SkyBox::RenderSkybox()
{
	glBindVertexArray(vaoIndex);
    glDrawArrays(GL_TRIANGLES, 0, (floatCount/sizeof(GLfloat))/3);
	glBindVertexArray(0);
}