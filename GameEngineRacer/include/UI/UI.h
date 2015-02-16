#ifndef UI_H
#define UI_H
#include <vector>
#include "glm\glm.hpp"
#include "glm\gtc\constants.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "ResourceManager.h"

class UI
{
private:
	ResourceManager* rManager;
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
public:
	UI();
	~UI();
	void initText2D();
	void printText2D(std::string text, int x, int y, int size);
	void cleanupText2D();

};
#endif

