#ifndef GPUTEXTURE_H
#define GPUTEXTURE_H

#include "ImageTexture.h"
#include "GL\glew.h"
#include "..\Interfaces\IGPUResource.h"



class GPUTexture : public IGPUResource
{
public:
	GPUTexture();
	virtual ~GPUTexture();
	bool UploadData();
	bool DeleteData();
	bool SetTexture(ImageTexture* tex);

private:
	ImageTexture* texture;

};
#endif