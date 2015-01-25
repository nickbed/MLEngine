#include "KeyMessage.h"

namespace mauvemessage
{
	KeyMessage::KeyMessage(const char* messageType, int content) : contents(content), mauvemessage::BaseMessage(messageType){}

	KeyMessage::operator int()
	{
		//Can never be null
		return contents;
	}
}