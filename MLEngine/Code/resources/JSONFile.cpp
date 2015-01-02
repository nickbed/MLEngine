#include "JSONFile.h"

JSONFile::JSONFile()
{
	
}

JSONFile::JSONFile(std::string path)
{
	bool result = LoadFromFile(path);
	result &= mauveassert::Assert::AssertTrue("Error loading JSON from file", result, mauveassert::ENUM_severity::SEV_ERROR);
	if (!result)
	{
		DEBUGWRITEINFO(reader.getFormattedErrorMessages(), "");
	}
	loaded = result;
}

JSONFile::~JSONFile()
{
	if (ParsedJson != nullptr) delete ParsedJson;
}

bool JSONFile::LoadFromFile(std::string filename)
{
	ParsedJson = new Json::Value(0);
	std::string JSONString = mauvefile::FileManager::ReadFile(filename.c_str(), false);
	bool parsed = reader.parse(JSONString, *ParsedJson, false);
	if (!parsed) return false;
	return true;
}

const Json::Value& JSONFile::GetJsonKeyValue(const char* key)
{
	try
	{
		Json::Value& jsonval = *ParsedJson;
		return jsonval[key];
	}
	catch (std::exception)
	{
		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_ERROR, "Error retrieving json key value");
		DEBUGWRITEINFO("Key value", key);
		Json::Value& empty = Json::Value(0);
		return empty;
	}
}