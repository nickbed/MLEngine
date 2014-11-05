#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace mauvefile
{
	class FileManager
	{
	public:

		//Reads a file into a string, shader formatting puts /n at the end of each line
		static std::string ReadFile(const char* filename, bool shaderFormatting);
	};
}

#endif