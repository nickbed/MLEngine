#ifndef IRESOURCE_H
#define IRESOURCE_H
#include <string>

class IResource
{
public:
	IResource();
	virtual ~IResource();
	virtual bool LoadFromFile(std::string filename);
	std::string GetFilename();

private:
	//Assuming every resource of every type is loaded from a file and has a filename.
	std::string filename;
};
#endif