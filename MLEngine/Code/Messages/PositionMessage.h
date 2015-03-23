#ifndef POSITIONMESSAGE_H
#define POSITIONMESSAGE_H
#include "BaseMessage.h"
#include "GLM\vec3.hpp"

namespace mauvemessage
{
	//Class for sending position data to any listners
	class PositionMessage : public BaseMessage
	{
	public:
		PositionMessage(const char* messageType, glm::vec3 content);

		//Conversion straight to vec3
		operator glm::vec3();
	private:
		glm::vec3 contents;
	};
}

#endif