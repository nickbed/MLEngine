#include "MessageManager.h"
#include "..\Assert\Assert.h"

namespace mauvemessage
{
	typedef std::unordered_multimap<const char*, RecieverInfo>::iterator listnerIterator;

	std::unordered_multimap<const char*,mauvemessage::RecieverInfo>* MessageManager::listnerMap = nullptr;
	bool MessageManager::lockListnerList = false;
	void* MessageManager::voidListenersToClear[16];
	int MessageManager::voidListenersCount = 0;

	MessageManager::MessageManager()
	{
		//mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_FATAL, "Cannot use default constructor with the Message Manager");
		lockListnerList = false;
		voidListenersCount = 0;
	}

	MessageManager::MessageManager(std::unordered_multimap<const char*,mauvemessage::RecieverInfo>* listners)
	{
		listnerMap = listners;
		lockListnerList = false;
		voidListenersCount = 0;
	}

	MessageManager::~MessageManager()
	{
		listnerMap->clear();
	}

	void MessageManager::SendListnerMessage(BaseMessage* message, const char* typeToSend)
	{
		lockListnerList = true;
		listnerIterator startIt, endIt;
		std::pair<listnerIterator, listnerIterator> keyRange = listnerMap->equal_range(typeToSend);

		//mauveassert::Assert::AssertFalse("Attempt to send message when there are no listners of specified type", (keyRange.first == keyRange.second), mauveassert::ENUM_severity::SEV_WARNING);
		if(keyRange.first == keyRange.second) return;

		for(startIt = keyRange.first; startIt != keyRange.second; ++ startIt)
		{
			//Send our data into the void* in the function
			(*startIt).second.listnerFunction(message);
			if (voidListenersCount >= 1) break;
		}
		lockListnerList = false;
		if (voidListenersCount > 0)
		{
			for (int i = 0; i < voidListenersCount; ++i)
			{
				ClearMessageListner(voidListenersToClear[i]);
			}
			voidListenersCount = 0;
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
		if (lockListnerList)
		{
			if (voidListenersCount >= 1) return;
			voidListenersToClear[voidListenersCount++] = listnerObject;
			return;
		}
		//TODO - fix this...can't seem to null check a void pointer?
		//mauveassert::Assert::AssertTrue("Passing a null pointer to clear message listner", (int)listnerObject == (int)nullptr,mauveassert::ENUM_severity::SEV_FATAL);
		bool erased = false;
		listnerIterator startIt = listnerMap->begin();
		while(startIt != listnerMap->end())
		{
			if((*startIt).second.listenobjectptr == listnerObject)
			{
				//Bin it - (bug fix)there might be more than one thing this object is listening to!
				mauveassert::Assert::WriteDebug("Clearing listner listening to:", (*startIt).second.typeToListen, mauveassert::ENUM_severity::SEV_INFO);
				startIt = listnerMap->erase(startIt);
				erased = true;
			}
			else
			{
				++startIt;
			}
		}
		//Bit spammy tbh
		//if(!erased)mauveassert::Assert::HandleAssert(mauveassert::ENUM_severity::SEV_INFO, "Not clearing any listners for this object. Probably not listening to anything.");
	}

	void MessageManager::ClearMessageListner(RecieverInfo& recieverToRemove)
	{
		listnerIterator startIt = listnerMap->begin();
		while(startIt != listnerMap->end())
		{
			if(((*startIt).second.listenobjectptr == recieverToRemove.listenobjectptr) && ((*startIt).second.typeToListen == recieverToRemove.typeToListen))
			{
				mauveassert::Assert::WriteDebug("Clearing listner listening to:", (*startIt).second.typeToListen, mauveassert::ENUM_severity::SEV_INFO);
				//Bin it
				listnerMap->erase(startIt);
				return;
			}
			else
			{
				++startIt;
			}
		}
	}
	void MessageManager::ClearAllListners()
	{
		listnerMap->clear();
	}

	void MessageManager::LoadMap(std::unordered_multimap<const char*,mauvemessage::RecieverInfo>* listners)
	{
		listnerMap = listners;
	}

}