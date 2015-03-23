#include "..\Interfaces\


IGPUResource::IGPUResource()
{
	this->filename = "";
}

IGPUResource::IGPUResource(std::string file) : filename(file)
{
}

IGPUResource::~IGPUResource()
{
}


std::string IGPUResource::GetFilename()
{
	return filename;
}

bool IGPUResource::HasLoaded()
{
	return loaded;
}