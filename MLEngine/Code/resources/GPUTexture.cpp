#include "GPUTexture.h"

GPUTexture::GPUTexture()
{

}

GPUTexture::~GPUTexture()
{
	//DeleteData();
}

bool GPUTexture::UploadData()
{
	NULLPTRCHECK(texture, "Cannot upload null texture to the GPU");
	GLenum bitmapFormat;

	//TODO - maybe handle more of these formats?
	if (texture->GetBitmap()->format() == Bitmap::Format_RGB)
	{
		bitmapFormat = GL_RGB;
	}
	else
	{
		bitmapFormat = GL_RGBA;
	}

	//Generate, bind and upload data
	glGenTextures(1, &bufferloc);
	glBindTexture(GL_TEXTURE_2D, bufferloc);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		bitmapFormat,
		(GLsizei)texture->GetBitmap()->width(),
		(GLsizei)texture->GetBitmap()->height(),
		0,
		bitmapFormat,
		GL_UNSIGNED_BYTE,
		texture->GetBitmap()->pixelBuffer());
	//Reset the texture
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool GPUTexture::DeleteData()
{
	glDeleteTextures(1, &bufferloc);
	return true;
}

bool GPUTexture::SetTexture(ImageTexture* tex)
{
	texture = tex;
	return true;
}

