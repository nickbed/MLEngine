#include "OBJModel.h"
#include <algorithm>

OBJModel::OBJModel()
{
}

OBJModel::OBJModel(std::string path)
{
	bool result = LoadFromFile(path);
	result &= mauveassert::Assert::AssertTrue("Error loading model from file", result, mauveassert::ENUM_severity::SEV_ERROR);
	if (!result) return;
}

OBJModel::~OBJModel()
{
}

bool OBJModel::LoadFromFile(std::string filename)
{
	std::vector<std::string> objString = mauvefile::FileManager::ReadFileLines(filename.c_str());
	bool result1 = mauveassert::Assert::AssertTrue("Failed to read object model file", (objString.size() > 0), mauveassert::ENUM_severity::SEV_ERROR);
	if (!result1) return false;

	//Parse each line
	for (std::vector<std::string>::iterator it = objString.begin(); it != objString.end(); ++it)
	{
		const char identifierChar[3] =
		{
			(*it)[0],
			(*it)[1],
			(*it)[2]
		};

		if (identifierChar[0] == 'v')
		{
			//Handle if it is a vertex (has a blank space next to it)
			if (identifierChar[1] == ' ')
			{
				std::size_t startingPos = (*it).find(' ');
				//Parse the vertex
				for (int i = 0; i < 3; ++i)
				{
					//Find next space
					std::size_t second = (*it).find(' ', startingPos+1);
					
					//If we can't find one, set second to the end of the string
					if (second == std::string::npos) second = (*it).size();
					
					//Parse the vertex string and put the value into the vector
					std::string vert = (*it).substr(startingPos, second-startingPos);
					parsedVertices.push_back(std::stof(vert));
					
					//Set the starting position to second
					startingPos = second;
				}
			}

			//Handle if it is a UV coord (has a t next to it)
			//Only has 2 values
			else if (identifierChar[1] == 't')
			{
				//Parse the UV
				std::size_t startingPos = (*it).find(' ');
				//Parse the normals
				for (int i = 0; i < 2; ++i)
				{
					//Find next space
					std::size_t second = (*it).find(' ', startingPos + 1);

					//If we can't find one, set second to the end of the string
					if (second == std::string::npos) second = (*it).size();

					//Parse the vertex string and put the value into the vector
					std::string uvcoord = (*it).substr(startingPos, second - startingPos);
					parsedUVs.push_back(std::stof(uvcoord));

					//Set the starting position to second
					startingPos = second;
				}
			}

			//Handle if it is a normal (has a n next to it)
			else if (identifierChar[1] == 'n')
			{
				std::size_t startingPos = (*it).find(' ');
				//Parse the vertex
				for (int i = 0; i < 3; ++i)
				{
					//Find next space
					std::size_t second = (*it).find(' ', startingPos + 1);

					//If we can't find one, set second to the end of the string
					if (second == std::string::npos) second = (*it).size();

					//Parse the vertex string and put the value into the vector
					std::string normal = (*it).substr(startingPos, second - startingPos);
					parsedNormals.push_back(std::stof(normal));

					//Set the starting position to second
					startingPos = second;
				}
			}
		}

		//Index
		if (identifierChar[0] == 'f' && identifierChar[1] == ' ')
		{
			std::size_t startingPos = (*it).find(' ') +1;
			//Parse the vertex
			for (int i = 0; i < 9; ++i)
			{
				//Find next slash#
				std::size_t second = std::string::npos;
				if (i > 0 && ((i+1) % 3) == 0)
				{
					second = (*it).find(' ', startingPos + 1);
				}
				else
				{
					second = (*it).find('/', startingPos + 1);
				}

				//If we can't find one, set second to the end of the string
				if (second == std::string::npos) second = (*it).size();

				//Parse the vertex string and put the value into the vector
				std::string index = (*it).substr(startingPos, second - startingPos);
				index.erase(std::remove(index.begin(), index.end(), '/'), index.end());
				//index.replace(index.begin(), index.begin() + 1, "");
				parsedIndices.push_back(std::stoi(index));

				//Set the starting position to second
				startingPos = second;
			}
		}
	}
	return true;
}

void OBJModel::GetAllData(std::vector<GLfloat> &verts, std::vector<GLfloat> &UVs, std::vector<GLfloat> &normals, std::vector<GLuint> &indices)
{
	verts = parsedVertices;
	UVs = parsedUVs;
	normals = parsedNormals;
	indices = parsedIndices;
}

const std::vector<GLfloat> OBJModel::GetVertices()
{
	return parsedVertices;
}

const std::vector<GLfloat> OBJModel::GetUVs()
{
	return parsedUVs;
}

const std::vector<GLfloat> OBJModel::GetNormals()
{
	return parsedNormals;
}

const std::vector<GLuint> OBJModel::GetIndicies()
{
	return parsedIndices;
}