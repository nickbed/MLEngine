#include "TextRender.h"

TextRender::TextRender()
{
	texturePath = "data\\images\\font.tga";
	shaderPath = "data\\shaders\\text";
}

bool TextRender::InitTextRender(int xScr, int yScr)
{
	xScreen = xScr;
	yScreen = yScr;

	textShader = mauveresource::ResourceManager::GetResource<Shader>(shaderPath);
	NULLPTRCHECK(textShader, "Failed to init text renderer due to failing to load text shader");

	fontTexture = mauveresource::ResourceManager::GetResource<ImageTexture>(texturePath);
	NULLPTRCHECK(fontTexture, "Failed to init text renderer due to failing to load font texture");
	fontTexture->GetBitmap()->flipVertically();

	textShader->UseShader();

	//Load our texture in
	GLenum bitmapFormat;
	//TODO - maybe handle more of these formats?
	if (fontTexture->GetBitmap()->format() == Bitmap::Format_RGB)
	{
		bitmapFormat = GL_RGB;
	}
	else
	{
		bitmapFormat = GL_RGBA;
	}

	glGenVertexArrays(1, &VAOid);
	glBindVertexArray(VAOid);
	//Generate, bind and upload data
	glGenTextures(1, &fontTextureID);
	glBindTexture(GL_TEXTURE_2D, fontTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		bitmapFormat,
		(GLsizei)fontTexture->GetBitmap()->width(),
		(GLsizei)fontTexture->GetBitmap()->height(),
		0,
		bitmapFormat,
		GL_UNSIGNED_BYTE,
		fontTexture->GetBitmap()->pixelBuffer());
	//Reset the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	GLenum error = glGetError();

	//Generate buffers for UVs and vertices
	glGenBuffers(1, &VBOid);
	glGenBuffers(1, &uvBufferID);
	glBindVertexArray(0);
	
	return true;
}

void TextRender::Draw2DText(std::string text, int x, int y, int size)
{
	textShader->UseShader();
	const char* textcstr = text.c_str();

	std::vector<glm::vec2> verts;
	std::vector<glm::vec2> UVs;
	
	int xMin = 0 + x;
	for(unsigned int i = 0; i < text.length(); ++i)
	{
		int offset = size /2 - 5;
		//Get as ascii value
		char charval = text[i];

		//Generate vertices and UVs for each character
		//Vertices

		//Fixes for wide/narrow chars
		if(charval == 'i' || charval == 'L' || charval == 'J' || charval == 'I' || charval == 'l')
		{
			xMin -= size/8;
			offset += size/3;
		}
		if(charval == 'm' || charval == 'M' || charval == 'w' || charval == 'W')
		{
			offset -= size/9;
		}

		int xMax = xMin + size;
		if((xMax - xMin) <=10) xMax += 10;

		int yMin = y;
		int yMax = y + size;

		
		glm::vec2 topLeft = glm::vec2(xMin, yMax);
		glm::vec2 topRight = glm::vec2(xMax, yMax);
		glm::vec2 bottomLeft = glm::vec2(xMin, yMin);
		glm::vec2 bottomRight = glm::vec2(xMax, yMin);
		
		//Start with finishing position of old one
		xMin = xMax - offset;

		//Tri 1
		verts.push_back(topLeft);
		verts.push_back(bottomLeft);
		verts.push_back(topRight);

		//Tri 2
		verts.push_back(bottomRight);
		verts.push_back(topRight);
		verts.push_back(bottomLeft);

		

		//Get top left of character in the texture
		float xUV = (charval%16)/16.0f-1;
		float yUV = (charval/16)/16.0f-1;
		float charWidth = 1.0f/16.0f;

		glm::vec2 topLeft1 = glm::vec2(xUV, yUV);
		glm::vec2 topRight1 = glm::vec2(xUV + charWidth, yUV);
		glm::vec2 bottomLeft1 = glm::vec2(xUV, yUV + charWidth);
		glm::vec2 bottomRight1 = glm::vec2(xUV + charWidth, yUV + charWidth);

		UVs.push_back(topLeft1);
		UVs.push_back(bottomLeft1);
		UVs.push_back(topRight1);

		UVs.push_back(bottomRight1);
		UVs.push_back(topRight1);
		UVs.push_back(bottomLeft1);
	}
	glBindVertexArray(VAOid);
	//Upload the stuff
	glBindBuffer(GL_ARRAY_BUFFER, VBOid);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec2), &verts[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	glBindTexture(GL_TEXTURE_2D, fontTextureID);

	textShader->SendUniformVec2("screenSize", glm::vec2(xScreen, yScreen));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBOid);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Draw it
	glDrawArrays(GL_TRIANGLES, 0, verts.size());
	
	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

}

TextRender::~TextRender()
{
	glDeleteBuffers(1, &uvBufferID);
	glDeleteBuffers(1, &VBOid);
}