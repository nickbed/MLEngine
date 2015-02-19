#ifndef MODELLOADER_H
#define MODELLOADER_H
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>

#include <glfw3.h>

enum linetype {unknown, v, f, vn, vt };



class ModelLoader
{
private:
	std::string line, m_filename;
	size_t found;
	std::ifstream input;
	std::stringstream ss;
	float verticesTexture[2];
	float vertices[3];
	float verticesNormal[3];
	int face[3];
	int faceNormal[3];
	int faceTexture[3];

	
	linetype thisline;
	std::vector<float> vArray, vnArray, vtArray;
	std::vector<GLfloat> allFaces_vertices_coords, allfaces_vertices_normals, allfaces_vertices_texture;
public:
	ModelLoader(const std::string& nfileName);
	ModelLoader();
	~ModelLoader();
	void loadFromfile(const std::string& nfileName);//Loads in an OBJ 

	void setName(const std::string& name){m_filename = name;}; 
	const std::string& getName(){return m_filename;}; 

	const std::vector<GLfloat>& getVerts(){return allFaces_vertices_coords;};
	const std::vector<GLfloat>& getNormals(){return allfaces_vertices_normals;};
	const std::vector<GLfloat>& getTextureCoords(){return allfaces_vertices_texture;};
};
#endif
