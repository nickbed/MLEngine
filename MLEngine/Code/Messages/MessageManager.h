#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include "MessageTypes.h"
#include "BaseMessage.h"

namespace mauvemessage
{
	//Struct to hold info about listners to messages.
	//Must be created through the constructor, then passed through to anything that needs it.
	struct RecieverInfo
	{
		RecieverInfo() {}
		RecieverInfo(const char* listentype, void* objectptr, std::function<void(BaseMessage*)> func) : typeToListen(listentype), listenobjectptr(objectptr), listnerFunction(func) {}
		const char* typeToListen;
		void* listenobjectptr;
		std::function<void(BaseMessage*)> listnerFunction;
	};

	class MessageManager
	{
	public:
		MessageManager();
		MessageManager(std::unordered_multimap<const char*,mauvemessage::RecieverInfo>* listners);
		virtual ~MessageManager();

		//Send Messages, Add listners and remove them.
		static void SendListnerMessage(BaseMessage* message, const char* typeToSend);
		static void AddMessageListner(const char* typeToListen, RecieverInfo& reciever);
		static void ClearMessageListner(void* listnerObject);
		static void ClearMessageListner(RecieverInfo& recieverToRemove);

	private:
		static std::unordered_multimap<const char*,mauvemessage::RecieverInfo>* listnerMap;
	};
}


#endif