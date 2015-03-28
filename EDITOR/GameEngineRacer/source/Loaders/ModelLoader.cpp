#include "Loaders\ModelLoader.h"


ModelLoader::ModelLoader():min(10000),max(-10000)
{
}
ModelLoader::ModelLoader(const std::string& nfileName):min(10000),max(-10000)
{
	loadFromfile(nfileName);
}


ModelLoader::~ModelLoader()
{
}
void ModelLoader::loadFromfile(const std::string& nfileName)
{
	m_filename = nfileName;
	
	input.open( m_filename);
	allFaces_vertices_coords.resize(0);
	allfaces_vertices_normals.resize(0);
	allfaces_vertices_texture.resize(0);

	if(!input.is_open())
	{
		std::cout << "Object not found" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Loading...\n";

	while(!input.eof())
	{
		//cout << "file is loaded \n";

		getline(input, line);
		thisline = unknown;

		found = line.find("v ");
		if (found!= std::string::npos)
		{
			//cout<<line<<endl;
			thisline = v;
		}
		found = line.find("f ");
		if (found!= std::string::npos)
		{
			thisline = f;
		}
		found = line.find("vn");
		if (found!= std::string::npos)
		{
			thisline = vn;
		}
		found = line.find("vt");
		if (found!= std::string::npos)
		{
			thisline = vt;
		}
		ss.str("");
		ss.clear();
		ss.str(line);
		ss.ignore(2);//ignores the first 2 chars in the string.


		switch(thisline){
		case v:
			//Extract Values from line v.
			ss >> vertices[0] >> vertices[1] >> vertices[2];
			//pushes them into a vector.
			if(vertices[0] < min.x)
			{
				min.x = vertices[0];
			}
			if(vertices[0] > max.x)
			{
				max.x = vertices[0];
			}
			if(vertices[1] < min.y)
			{
				min.y = vertices[1];
			}
			if(vertices[1] > max.y)
			{
				max.y = vertices[1];
			}
			if(vertices[2] < min.z)
			{
				min.z = vertices[2];
			}
			if(vertices[2] > max.z)
			{
				max.z = vertices[2];
			}
			vArray.push_back(vertices[0]);
			vArray.push_back(vertices[1]);
			vArray.push_back(vertices[2]);
			break;
		case vn:
			//Extract Values from line vn.
			ss >> verticesNormal[0] >> verticesNormal[1] >> verticesNormal[2];
			//pushes them into a vector.
			vnArray.push_back(verticesNormal[0]);
			vnArray.push_back(verticesNormal[1]);
			vnArray.push_back(verticesNormal[2]);
			break;
		case vt:
			//Extract Values from line vt.
			ss >> verticesTexture[0] >> verticesTexture[1];
			//pushes them into a vector.
			vtArray.push_back(verticesTexture[0]);
			vtArray.push_back(verticesTexture[1]);
			vtArray.push_back(0);
			break;
		case f:

			for(int i=0; i < 3; i++)
			{
				//This holds 1//4 pattern
				ss>>face[i];//Takes the line of vertices it needs.
				if(ss.peek() == '/')//If the next line is a / then we know there must be facetextures and/or face normals.
				{
					ss.ignore(1);//Skips the / assuming its written correctly.
					if(ss.peek() != '/' && ss.peek() != ' ')//Peek looks at the next character in the string.
					{
						ss>>faceTexture[i];//Takes in the line of texture coords it needs.
						//cout << "Texture is Loaded \n";
					}
					ss.ignore(1);//Ignores the /.
					if(ss.peek() != ' ')
					{
						ss>>faceNormal[i];//Takes in the line of facenormal it needs.
						//cout << "Normal is Loaded \n";
					}
				}

				//All vertices in order.
				allFaces_vertices_coords.push_back(vArray[(face[i]-1)*3]);
				allFaces_vertices_coords.push_back(vArray[(face[i]-1)*3+1]);
				allFaces_vertices_coords.push_back(vArray[(face[i]-1)*3+2]);

				//All vertex normals in order.
				if(vnArray.size() != 0)//If the size of the vertex normal array is zero then dont push back.
				{
					allfaces_vertices_normals.push_back(vnArray[(faceNormal[i]-1)*3]);
					allfaces_vertices_normals.push_back(vnArray[(faceNormal[i]-1)*3+1]);
					allfaces_vertices_normals.push_back(vnArray[(faceNormal[i]-1)*3+2]);
				}
				//All vertex textures in order.
				if(vtArray.size() != 0)//If the size of the vertex texture array is zero then dont push back.
				{
					
					allfaces_vertices_texture.push_back(vtArray[(faceTexture[i]-1)*3]);
					allfaces_vertices_texture.push_back(vtArray[(faceTexture[i]-1)*3+1]);
					allfaces_vertices_texture.push_back(vtArray[(faceTexture[i]-1)*3+2]);
					allfaces_vertices_texture.pop_back();

				}

			}
			
			break;


		case unknown:
			break;
		}
	}
	vnArray.resize(0);
	vtArray.resize(0);
	vArray.resize(0);
	input.close();
}