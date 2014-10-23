#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H
#include <string>
#include <vector>
#include <unordered_map>

#include "MessageTypes.h"
#include "BaseMessage.h"

namespace mauvemessage
{
	//Struct to hold info about listners to messages.
	//Must be created through the constructor, then passed through to anything that needs it.
	struct RecieverInfo
	{
		RecieverInfo(const char* listentype, const void* objectptr, const void (*func)(BaseMessage message)) : typeToListen(listentype), listenobjectptr(objectptr), listnerFunction(func) {}
		const char* typeToListen;
		const void* listenobjectptr;
		const void (*listnerFunction)(BaseMessage message);
	};

	class MessageManager
	{
	public:
		MessageManager();
		MessageManager(std::unordered_multimap<const char*,mauvemessage::RecieverInfo>* listners);
		virtual ~MessageManager();

		//Send Messages, Add listners and remove them.
		static void SendListnerMessage(const BaseMessage& message, const char* typeToSend);
		static void AddMessageListner(const char* typeToListen, RecieverInfo& reciever);
		static void ClearMessageListner(const void* listnerObject);
		static void ClearMessageListner(RecieverInfo& recieverToRemove);

	private:
		static std::unordered_multimap<const char*,mauvemessage::RecieverInfo>* listnerMap;
	};
}


#endif