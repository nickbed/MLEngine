#ifndef KeyMessage_H
#define KeyMessage_H
#include "BaseMessage.h"
#include "GLM\vec3.hpp"

namespace mauvemessage
{
	//Class for sending position data to any listners
	class KeyMessage : public BaseMessage
	{
	public:
		KeyMessage(const char* messageType, int key);

		//Conversion straight to int
		operator int();
	private:
		int contents;
	};
}

#endif