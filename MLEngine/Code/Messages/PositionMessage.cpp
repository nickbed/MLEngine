#include "PositionMessage.h"

namespace mauvemessage
{
	PositionMessage::PositionMessage(const char* messageType, glm::vec3 content) : contents(content), mauvemessage::BaseMessage(messageType){}

	PositionMessage::operator glm::vec3()
	{
		//Can never be null
		return contents;
	}
}