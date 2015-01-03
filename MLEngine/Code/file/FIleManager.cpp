#include "FileManager.h"
#include "..\Assert\Assert.h"

namespace mauvefile
{

	std::string FileManager::ReadFile(const char* file, bool shaderFormatting)
	{
		std::string output;
		std::ifstream fileStream(file, std::ios::in);
		if(!fileStream.good())
		{
			std::string errorMessage;
			errorMessage.append("Error reading file: ");
			errorMessage.append(file);
			mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_ERROR, (char*)errorMessage.c_str());
			return output;
		}
		if(fileStream.is_open())
		{
			std::string Line = "";
			while(getline(fileStream, Line))
			{
				if(shaderFormatting) output += "\n"; 
				output += Line;
			}
			fileStream.close();
		}
		return output;
	}

	std::vector<std::string> FileManager::ReadFileLines(const char* filename)
	{
		std::string output;
		std::ifstream fileStream(filename, std::ios::in);
		std::vector<std::string> result;
		if (!fileStream.good())
		{
			std::string errorMessage;
			errorMessage.append("Error reading file: ");
			errorMessage.append(filename);
			mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_ERROR, (char*)errorMessage.c_str());
			return result;
		}

		//Read each line into the vector
		std::string currentLine;
		while (std::getline(fileStream, currentLine))
		{
			result.push_back(currentLine);
		}
		return result;
	}

}