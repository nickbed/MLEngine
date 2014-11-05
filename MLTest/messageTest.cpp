#include "stdafx.h"
#include "CppUnitTest.h"
#include "Assert\Assert.h"
#include "Messages\MessageManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MLTEST
{

	TEST_CLASS(MessageTest)
	{
	public:

		static bool testHasPassed;
		static bool messageRecieved;

		static void ExceptionHandler(const char* message)
		{
			testHasPassed = true;
		}

		static void MessageHandler(mauvemessage::BaseMessage msg)
		{
			messageRecieved = true;
		}

		void init()
		{
			//Init our exception handler and test passed to be false
			MLTEST::MessageTest::testHasPassed = false;
			MLTEST::MessageTest::messageRecieved = false;
			mauveassert::Assert::InitExceptionHandler(&MLTEST::MessageTest::ExceptionHandler);
		}

		TEST_METHOD_INITIALIZE(Set_Up_Exception_Handler)
		{
			init();
		}

		TEST_METHOD(Using_Default_Constructor_With_Message_Manager_Results_In_A_Fatal_Assert)
		{
			//Invoke using the default constructor - Causes an assert
			mauvemessage::MessageManager testManager = mauvemessage::MessageManager();

			//Should drop into the assert handler and set testHasPassed to true;
			Assert::IsTrue(testHasPassed, TEXT("Assert has not been handled correctly testHasPassed is false"));
		}

		TEST_METHOD(Listner_Should_Recieve_Message_Sent_To_A_Type_It_Is_Listening_To)
		{
			//Listner setup
			const char* typeToListen = "testType";
			mauvemessage::RecieverInfo messageReciever;
			messageReciever.listenobjectptr = this;
			messageReciever.listnerFunction = &MLTEST::MessageTest::MessageHandler;
			messageReciever.typeToListen = typeToListen;

			//Create map and message handler instance
			std::unordered_multimap<const char*,mauvemessage::RecieverInfo>* listners = new std::unordered_multimap<const char*,mauvemessage::RecieverInfo>();
			mauvemessage::MessageManager messageHandler = mauvemessage::MessageManager(listners);

			//Add listner into map
			messageHandler.AddMessageListner(typeToListen, messageReciever);

			//Create message
			mauvemessage::BaseMessage messageToSend = mauvemessage::BaseMessage("testMessage");

			//Send message
			messageHandler.SendListnerMessage(messageToSend, "testType");

			Assert::IsTrue(messageRecieved, TEXT("Message has been recieved by the test handler function"));

		}

		TEST_METHOD(Adding_A_Message_Listner_Adds_It_To_The_Listner_List)
		{
			//Listner setup
			const char* typeToListen = "testType";
			mauvemessage::RecieverInfo messageReciever;
			messageReciever.listenobjectptr = this;
			messageReciever.listnerFunction = &MLTEST::MessageTest::MessageHandler;
			messageReciever.typeToListen = typeToListen;

			//Create map and message handler instance
			std::unordered_multimap<const char*,mauvemessage::RecieverInfo>* listners = new std::unordered_multimap<const char*,mauvemessage::RecieverInfo>();
			mauvemessage::MessageManager messageHandler = mauvemessage::MessageManager(listners);

			//Add listner into map
			messageHandler.AddMessageListner(typeToListen, messageReciever);

			//Check to see if it's in the map
			mauvemessage::RecieverInfo gotReciever = listners->find(typeToListen)->second;

			//Should both point to the same object (this)
			Assert::AreEqual((int)gotReciever.listenobjectptr, (int)messageReciever.listenobjectptr, TEXT("Both listners point to the same object address"));

		}

		TEST_METHOD(Clearing_A_Message_Listner_Removes_It_From_The_Listner_List_Using_Void_Pointer)
		{
			//Listner setup
			const char* typeToListen = "testType";
			mauvemessage::RecieverInfo messageReciever;
			messageReciever.listenobjectptr = this;
			messageReciever.listnerFunction = &MLTEST::MessageTest::MessageHandler;
			messageReciever.typeToListen = typeToListen;

			//Create map and message handler instance
			std::unordered_multimap<const char*,mauvemessage::RecieverInfo>* listners = new std::unordered_multimap<const char*,mauvemessage::RecieverInfo>();
			mauvemessage::MessageManager messageHandler = mauvemessage::MessageManager(listners);

			//Add listner into map
			messageHandler.AddMessageListner(typeToListen, messageReciever);

			//Check to see if it's in the map
			mauvemessage::RecieverInfo gotReciever = listners->find(typeToListen)->second;

			//Should both point to the same object (this)
			Assert::AreEqual((int)gotReciever.listenobjectptr, (int)messageReciever.listenobjectptr, TEXT("Both listners point to the same object address"));

			//Remove the listner using void pointer
			messageHandler.ClearMessageListner(this);

			//Map should be empty now
			Assert::AreEqual(0, (int)listners->size(), TEXT("Map is empty"));
		}

		TEST_METHOD(Clearing_A_Message_Listner_Removes_It_From_The_Listner_List_Using_Reciever_Type)
		{
			//Listner setup
			const char* typeToListen = "testType";
			mauvemessage::RecieverInfo messageReciever;
			messageReciever.listenobjectptr = this;
			messageReciever.listnerFunction = &MLTEST::MessageTest::MessageHandler;
			messageReciever.typeToListen = typeToListen;

			//Create map and message handler instance
			std::unordered_multimap<const char*,mauvemessage::RecieverInfo>* listners = new std::unordered_multimap<const char*,mauvemessage::RecieverInfo>();
			mauvemessage::MessageManager messageHandler = mauvemessage::MessageManager(listners);

			//Add listner into map
			messageHandler.AddMessageListner(typeToListen, messageReciever);

			//Check to see if it's in the map
			mauvemessage::RecieverInfo gotReciever = listners->find(typeToListen)->second;

			//Should both point to the same object (this)
			Assert::AreEqual((int)gotReciever.listenobjectptr, (int)messageReciever.listenobjectptr, TEXT("Both listners point to the same object address"));

			//Remove the listner using void pointer
			messageHandler.ClearMessageListner(gotReciever);

			//Map should be empty now
			Assert::AreEqual(0, (int)listners->size(), TEXT("Map is empty"));
		}
	
	};

	//Init for static class
	bool MLTEST::MessageTest::testHasPassed = false;
	bool MLTEST::MessageTest::messageRecieved = false;

}