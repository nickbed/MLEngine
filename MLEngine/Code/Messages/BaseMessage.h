#ifndef BASEMESSAGE_H
#define BASEMESSAGE_H
#include "MessageTypes.h"
#include <string>
//Class for defining the message base class
namespace mauvemessage
{
	class BaseMessage
	{
	public:
		BaseMessage(const char* messageType);
		virtual ~BaseMessage();

	private:
		std::string id;
		std::string messageString;
	};
}

#endif