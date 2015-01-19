#ifndef IGPUResource_H
#define IGPUResource_H
#include <string>
#include <vector>
#include "GLFW\glfw3.h"

class IGPUResource
{
public:
	IGPUResource();
	//IGPUResource(std::string name, std::vector<GLfloat> vertices, std::vector<GLfloat> normals, std::vector<GLfloat> UVs, std::vector<GLuint> indices);
	virtual ~IGPUResource();
	virtual bool UploadData() = 0;
	virtual bool DeleteData() = 0;
	GLuint GetDataLocation();
	std::string GetName();
	virtual bool HasLoaded();

protected:
	bool loaded;
	//Assuming every resource of every type has a buffer location as a uint
	GLuint bufferloc;
	std::string name;

private:


};
#endif