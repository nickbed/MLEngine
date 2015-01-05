#include "ImageTexture.h"

ImageTexture::ImageTexture()
{
}

ImageTexture::ImageTexture(std::string path)
{
	if (path.size() == 0)
	{
		loaded = false;
		return;
	}
	bool result = LoadFromFile(path);
	result &= mauveassert::Assert::AssertTrue("Error loading image from file", result, mauveassert::ENUM_severity::SEV_ERROR);
	loaded = result;
}

ImageTexture::~ImageTexture()
{
	if (imageBitmap != nullptr)
	{
		delete imageBitmap;
	}
}

bool ImageTexture::LoadFromFile(std::string filename)
{
	try
	{
		Bitmap* bmp = new Bitmap(Bitmap::bitmapFromFile(filename));
		bmp->flipVertically();
		imageBitmap = bmp;
		loaded = true;
	}
	catch (std::exception e)
	{
		loaded = false;
		return false;
	}
	return true;
}

Bitmap* ImageTexture::GetBitmap()
{
	return imageBitmap;
}