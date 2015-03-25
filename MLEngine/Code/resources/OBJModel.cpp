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
	bool result1 = mauvefile::FileManager::OpenFile(filename.c_str());
	result1 &= mauveassert::Assert::AssertTrue("Failed to read object model file", (result1), mauveassert::ENUM_severity::SEV_ERROR);
	if (!result1) return false;

	//std::vector<GLfloat> tempUVs;
	//std::vector<GLfloat> tempNormals;
	//std::vector<GLfloat> tempVerts;
	int currentUVCount = 0;
	int currentNormalCount = 0;
	int currentVertCount = 0;
	parsedUVCount = 0;
	parsedIndexCount = 0;
	parsedNormalCount = 0;
	parsedVertCount = 0;
	GLfloat tempUVs[50000];
	GLfloat tempNormals[50000];
	GLfloat tempVerts[50000];
	char *next_token = NULL;


	//Parse each line
	char gotLine[128];
	while (mauvefile::FileManager::ReadLineFromFileChar(gotLine))
	{
		if (gotLine[0] == '\n' || gotLine[1] == '\n') continue;
		const char identifierChar[3] =
		{
			gotLine[0],
			gotLine[1],
			gotLine[2]
		};
		try{
		if (identifierChar[0] == 'v')
		{
			//Handle if gotLine is a vertex (has a blank space next to gotLine)
			if (identifierChar[1] == ' ')
			{
				char * vertStart = strtok_s(gotLine, " ", &next_token);
				for (int i = 0; i < 3; ++i)
				{
					vertStart = strtok_s(NULL, " ", &next_token);
					tempVerts[currentVertCount++] = ((GLfloat)atof(vertStart));
				}
			}

			//Handle if gotLine is a UV coord (has a t next to gotLine)
			//Only has 2 values
			else if (identifierChar[1] == 't')
			{
				//Parse the UV
				char * vertStart = strtok_s(gotLine, " ", &next_token);
				//Parse the normals
				for (int i = 0; i < 2; ++i)
				{
					vertStart = strtok_s(NULL, " ", &next_token);
					tempUVs[currentUVCount++] = (GLfloat)atof(vertStart);
				}
			}

			//Handle if gotLine is a normal (has a n next to gotLine)
			else if (identifierChar[1] == 'n')
			{
				char * vertStart = strtok_s(gotLine, " ", &next_token);
				//Parse the vertex
				for (int i = 0; i < 3; ++i)
				{
					char * vertStart = strtok_s(gotLine, " ", &next_token);
					tempNormals[currentNormalCount++] = (GLfloat)(atof(vertStart));
				}
			}
		}

		//Index
		if (identifierChar[0] == 'f' && identifierChar[1] == ' ')
		{
			//std::size_t startingPos = gotLine.find(' ') +1;
			char * vertStart = strtok_s(gotLine, " /", &next_token);
			//Parse the vertex
			for (int i = 0; i < 9; ++i)
			{
				vertStart = strtok_s(NULL, " /", &next_token);
				if (i == 0 || i == 3 || i == 6)
				{
					int position = ((std::stoi(vertStart) - 1) * 3);
					if(position < 0)
					{
						position = 0;
					}
					parsedVertices[parsedVertCount++] = tempVerts[position];
					parsedVertices[parsedVertCount++] = tempVerts[position + 1];
					parsedVertices[parsedVertCount++] = tempVerts[position + 2];
					parsedIndices[parsedIndexCount] = parsedIndexCount++;
				}
				//UVs
				else if (i == 1 || i == 4 || i == 7)
				{
					int uvloc = ((std::stoi(vertStart) - 1) * 2);
					if(uvloc < 0)
					{
						uvloc = 0;
					}
					parsedUVs[parsedUVCount++] = (tempUVs[uvloc]);
					parsedUVs[parsedUVCount++] = (tempUVs[uvloc + 1]);
				}
				//Normals
				else if (i == 2 || i == 5 || i == 8)
				{
					int normalloc = ((std::stoi(vertStart) - 1) * 3);
					if(normalloc < 0 )
					{
						normalloc = 0;
					}
					parsedNormals[parsedNormalCount++] = tempNormals[normalloc];
					parsedNormals[parsedNormalCount++] = tempNormals[normalloc + 1];
					parsedNormals[parsedNormalCount++] = tempNormals[normalloc + 2];
				}
			}
		}
		else if(identifierChar[0] == 's' && identifierChar[1] == ' ')
		{
			continue;
		}
		}
		catch(std::exception e)
		{
			continue;
		}
	}
	//for (unsigned int i = 0; i < parsedIndexCount; ++i)
	//{
	//	parsedIndices[i] = i;
	//}
	mauvefile::FileManager::CloseFile();
	return true;
}

//void OBJModel::GetAllData(std::vector<GLfloat> &verts, std::vector<GLfloat> &UVs, std::vector<GLfloat> &normals, std::vector<GLuint> &indices)
//{
//	verts = parsedVertices;
//	UVs = parsedUVs;
//	normals = parsedNormals;
//	indices = parsedIndices;
//}

GLfloat* OBJModel::GetVertices()
{
	return parsedVertices;
}

GLfloat* OBJModel::GetUVs()
{
	return parsedUVs;
}

GLfloat* OBJModel::GetNormals()
{
	return parsedNormals;
}

GLuint* OBJModel::GetIndicies()
{
	return parsedIndices;
}

const unsigned int OBJModel::GetVertexCount()
{
	return parsedVertCount;
}

const unsigned int OBJModel::GetIndexCount()
{
	return parsedIndexCount;
}

const unsigned int OBJModel::GetNormalCount()
{
	return parsedNormalCount;
}

const unsigned int OBJModel::GetUVCount()
{
	return parsedUVCount;
}