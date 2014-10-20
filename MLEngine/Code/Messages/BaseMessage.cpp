#include "../Messages/BaseMessage.h"

BaseMessage::BaseMessage(MessageTypes messageType)
{
	this->messageString = "";
	this->id = messageType;
}

BaseMessage::~BaseMessage()
{
}

