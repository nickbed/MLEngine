#include "../Interfaces/IResource.h"


IResource::IResource()
{
	this->filename = "";
}

IResource::IResource(std::string file) : filename(file)
{
}

IResource::~IResource()
{
}


std::string IResource::GetFilename()
{
	return filename;
}

bool IResource::HasLoaded()
{
	return loaded;
}