#include "../Messages/BaseMessage.h"
namespace mauvemessage
{
	BaseMessage::BaseMessage(const char* messageType)
	{
		this->messageString = "";
		this->id = messageType;
	}

	BaseMessage::~BaseMessage()
	{
	}
}
