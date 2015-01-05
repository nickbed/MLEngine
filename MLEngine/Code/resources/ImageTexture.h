#ifndef IMAGETEXTURE_H
#define IMAGETEXTURE_H

#include "../Assert/Assert.h"
#include "..\Interfaces\IResource.h"
#include "..\File\FileManager.h"
#include "..\..\Libs\TextureLoader\Bitmap.h"

#include <string>
#include <vector>

class ImageTexture : public IResource
{
public:
	ImageTexture();
	ImageTexture(std::string path);

	~ImageTexture();

	//Load it
	virtual bool LoadFromFile(std::string filename);

	Bitmap* GetBitmap();


private:
	Bitmap* imageBitmap;

};
#endif