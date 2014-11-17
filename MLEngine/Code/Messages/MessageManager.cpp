#include "MessageManager.h"
#include "..\Assert\Assert.h"

namespace mauvemessage
{
	typedef std::unordered_multimap<const char*, RecieverInfo>::iterator listnerIterator;

	std::unordered_multimap<const char*,mauvemessage::RecieverInfo>* MessageManager::listnerMap = nullptr;

	MessageManager::MessageManager()
	{
		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_FATAL, "Cannot use default constructor with the Message Manager");
	}

	MessageManager::MessageManager(std::unordered_multimap<const char*,mauvemessage::RecieverInfo>* listners)
	{
		listnerMap = listners;
	}

	MessageManager::~MessageManager()
	{
	}

	void MessageManager::SendListnerMessage(BaseMessage* message, const char* typeToSend)
	{
		listnerIterator startIt, endIt;
		std::pair<listnerIterator, listnerIterator> keyRange = listnerMap->equal_range(typeToSend);

		mauveassert::Assert::AssertFalse("Message Listners Empty", (keyRange.first == keyRange.second), mauveassert::ENUM_severity::SEV_WARNING);
		
		for(startIt = keyRange.first; startIt != keyRange.second; ++ startIt)
		{
			//Send our data into the void* in the function
			(*startIt).second.listnerFunction(message);
		}
	}

	void MessageManager::AddMessageListner(const char* typeToListen, RecieverInfo& reciever)
	{
		mauveassert::Assert::AssertFalse("Message listner list is not initialized", listnerMap == NULL, mauveassert::ENUM_severity::SEV_FATAL);
		mauveassert::Assert::AssertFalse("Calling add message listner with null type", typeToListen == "" ,mauveassert::ENUM_severity::SEV_FATAL);

		listnerMap->insert(std::make_pair(typeToListen, reciever));
	}

	void MessageManager::ClearMessageListner(void* listnerObject)
	{
		mauveassert::Assert::AssertTrue("Passing a null pointer to clear message listner", listnerObject == nullptr,mauveassert::ENUM_severity::SEV_FATAL);
		
		for(listnerIterator startIt = listnerMap->begin(); startIt != listnerMap->end(); ++ startIt)
		{
			if((*startIt).second.listenobjectptr == listnerObject)
			{
				//Bin it
				listnerMap->erase(startIt);
				return;
			}
		}

		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_WARNING, "Unable to clear an object from the message listner list!");
	}

	void MessageManager::ClearMessageListner(RecieverInfo& recieverToRemove)
	{
		for(listnerIterator startIt = listnerMap->begin(); startIt != listnerMap->end(); ++ startIt)
		{
			if(((*startIt).second.listenobjectptr == recieverToRemove.listenobjectptr) && ((*startIt).second.typeToListen == recieverToRemove.typeToListen))
			{
				//Bin it
				listnerMap->erase(startIt);
				return;
			}
		}

		mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_WARNING, "Unable to clear an object from the message listner list!");
	}
}