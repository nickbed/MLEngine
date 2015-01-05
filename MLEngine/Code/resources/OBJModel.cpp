#include "OBJModel.h"
#include <algorithm>

OBJModel::OBJModel()
{
}

OBJModel::OBJModel(std::string path)
{
	bool result = LoadFromFile(path);
	result &= mauveassert::Assert::AssertTrue("Error loading model from file", result, mauveassert::ENUM_severity::SEV_ERROR);
	loaded = result;
}

OBJModel::~OBJModel()
{
}

bool OBJModel::LoadFromFile(std::string filename)
{
	std::vector<std::string> objString = mauvefile::FileManager::ReadFileLines(filename.c_str());
	parsedIndices.reserve(10000);
	parsedVertices.reserve(10000);
	parsedNormals.reserve(10000);
	parsedUVs.reserve(10000);
	bool result1 = mauveassert::Assert::AssertTrue("Failed to read object model file", (objString.size() > 0), mauveassert::ENUM_severity::SEV_ERROR);
	if (!result1) return false;

	std::vector<GLfloat> tempUVs;
	std::vector<GLfloat> tempNormals;
	std::vector<GLfloat> tempVerts;
	int indexCount = 0;

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
					tempVerts.push_back(std::stof(vert));
					
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
					tempUVs.push_back(std::stof(uvcoord));

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
					tempNormals.push_back(std::stof(normal));

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
				if (i == 0 || i == 3 || i == 6)
				{
					//parsedIndices.push_back(std::stoi(index) - 1);
					int position = ((std::stoi(index )-1) * 3);
					parsedVertices.push_back(tempVerts.at(position));
					parsedVertices.push_back(tempVerts.at(position + 1));
					parsedVertices.push_back(tempVerts.at(position + 2));
					++indexCount;
				}
				//UVs
				else if (i == 1 || i == 4 || i == 7)
				{
					int uvloc = ((std::stoi(index) - 1) * 2);
					parsedUVs.push_back(tempUVs.at(uvloc));
					parsedUVs.push_back(tempUVs.at(uvloc+1));
				}
				//Normals
				else if (i == 2 || i == 5 || i == 8)
				{
					int normalloc = ((std::stoi(index)-1) * 3);
					parsedNormals.push_back(tempNormals.at(normalloc));
					parsedNormals.push_back(tempNormals.at(normalloc+1));
					parsedNormals.push_back(tempNormals.at(normalloc+2));
				}

				//Set the starting position to second
				startingPos = second;
			}
		}
	}
	for (int i = 0; i < indexCount; ++i)
	{
		parsedIndices.push_back(i);
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