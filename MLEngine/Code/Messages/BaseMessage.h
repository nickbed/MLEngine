#ifndef BASEMESSAGE_H
#define BASEMESSAGE_H
#include "MessageTypes.h"
#include <string>
//Class for defining the message base class
class BaseMessage
{
public:
	BaseMessage(MessageTypes messageType);
	virtual ~BaseMessage();
	MessageTypes id;
	std::string messageString;
};

#endif