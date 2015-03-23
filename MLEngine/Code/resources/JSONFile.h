#ifndef JSONFile_H
#define JSONFile_H

#include "../Assert/Assert.h"
#include "..\Interfaces\IResource.h"
#include "..\File\FileManager.h"
#include "json\json.h"
#include <string>
#include <vector>

class JSONFile : public IResource
{
public:
	JSONFile();
	JSONFile(std::string path);

	~JSONFile();

	//Load it
	virtual bool LoadFromFile(std::string filename);

	const Json::Value& GetJsonKeyValue(const char* key);
	

private:
	Json::Reader reader;
	Json::Value* ParsedJson;
};
#endif