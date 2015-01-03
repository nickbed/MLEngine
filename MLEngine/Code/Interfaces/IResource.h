#ifndef IRESOURCE_H
#define IRESOURCE_H
#include <string>

class IResource
{
public:
	IResource();
	IResource(std::string file);
	virtual ~IResource();
	virtual bool LoadFromFile(std::string filename) = 0;
	std::string GetFilename();
	virtual bool HasLoaded();

protected:
	bool loaded;

private:
	//Assuming every resource of every type is loaded from a file and has a filename.
	std::string filename;
	
};
#endif