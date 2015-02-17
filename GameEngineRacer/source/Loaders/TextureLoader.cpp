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
	gTexture = new Texture(flip, gl::LINEAR, gl::REPEAT);
	return gTexture;
}
void TextureLoader::FlipImage()
{
	flip.flipVertically();
}