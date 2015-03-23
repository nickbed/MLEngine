#include "../Interfaces/IGPUResource.h"


IGPUResource::IGPUResource()
{
	this->name = "";
}


IGPUResource::~IGPUResource()
{
}


bool IGPUResource::HasLoaded()
{
	return loaded;
}


std::string IGPUResource::GetName()
{
	return name;
}

GLuint IGPUResource::GetDataLocation()
{
	return bufferloc;
}
