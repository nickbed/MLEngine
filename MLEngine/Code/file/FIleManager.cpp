#include "FileManager.h"
#include "..\Assert\Assert.h"

namespace mauvefile
{
	std::ifstream FileManager::openFile = std::ifstream();
	FILE* FileManager::openFilePTR;
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
		result.reserve(10000);
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

	bool FileManager::OpenFile(const char* filename)
	{
		//openFile = std::ifstream(filename, std::ios::in);
		fopen_s(&openFilePTR, filename, "r");
		//if (!openFile.good())
		//{
		//	std::string errorMessage;
		//	errorMessage.append("Error reading file: ");
		//	errorMessage.append(filename);
		//	mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_ERROR, (char*)errorMessage.c_str());
		//	return false;
		//}
		return true;
	}

	void FileManager::CloseFile()
	{
		openFile.close();
	}

	bool FileManager::ReadLineFromFile(std::string& line)
	{
		std::ios::sync_with_stdio(false);
		bool result = std::getline(openFile, line) != 0;
		return result;
	}

	bool FileManager::ReadLineFromFileChar(char* line)
	{
		if (fgets(line, 128, openFilePTR) != NULL)
		{
			return true;
		}
		return false;
	}

}