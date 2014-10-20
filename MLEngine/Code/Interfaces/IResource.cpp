#include "../Interfaces/IResource.h"


IResource::IResource()
{
	this->filename = "";
}

IResource::~IResource()
{

}

bool IResource::LoadFromFile(std::string filename)
{
	return false;
}

std::string IResource::GetFilename()
{
	return filename;
}
