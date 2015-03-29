#include "Loaders\TextureLoader.h"


TextureLoader::TextureLoader()
{
	gTexture = NULL;
}


TextureLoader::~TextureLoader()
{
	delete gTexture;
	gTexture = NULL;
}

void TextureLoader::LoadTexture(std::string filename)
{
	m_name = filename;
	flip = Bitmap::bitmapFromFile(filename);
	
}

Texture* TextureLoader::getTexture()
{
	gTexture = new Texture(flip, GL_LINEAR, GL_REPEAT);
	gTexture->TextureFormatForBitmapFormat(Bitmap::Format::Format_RGBA);
	return gTexture;
}
void TextureLoader::FlipImage()
{
	flip.flipVertically();
}