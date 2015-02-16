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
	Bitmap temp = Bitmap::bitmapFromFile("Texture/"+filename);
	flip = temp;
	
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