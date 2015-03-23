#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace mauvefile
{
	class FileManager
	{
	public:

		//Reads a file into a string, shader formatting puts /n at the end of each line
		static std::string ReadFile(const char* filename, bool shaderFormatting);

		//Reads a file into a vector of strings, 1 per line
		static std::vector<std::string> ReadFileLines(const char* filename);

		//File manager will store one file to read and write from
		static bool OpenFile(const char* filename);
		static void CloseFile();

		static bool ReadLineFromFile(std::string& line);

	private:
		static std::ifstream openFile; 
	};
}

#endif