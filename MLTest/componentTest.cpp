#include "stdafx.h"
#include "CppUnitTest.h"
#include "Assert\Assert.h"
#include "Messages\Messagemanager.h"
#include "Components\ComponentManager.h"
#include "Components\DebugComponent.h"
#include "Components\BasicKeyMovement.h"
#include "GLFW\glfw3.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MLTEST
{

	TEST_CLASS(ComponentTest)
	{
	public:

		static bool testHasPassed;

		static void ExceptionHandler(const char* message)
		{
			testHasPassed = true;
		}

		static void MessageHandler(mauvemessage::BaseMessage* msg)
		{
			if (static_cast<glm::vec3>(*(mauvemessage::PositionMessage*)msg) == glm::vec3(1.0f, 1.0f, 1.0f))
			{
				testHasPassed = true;
			}
		}



		void init()
		{
			//Init our exception handler and test passed to be false
			MLTEST::ComponentTest::testHasPassed = false;
			mauveassert::Assert::InitExceptionHandler(&MLTEST::ComponentTest::ExceptionHandler);
		}

		TEST_METHOD_INITIALIZE(Set_Up_Exception_Handler)
		{
			init();
		}

		TEST_METHOD(Adding_Component_Makes_It_Appear_In_The_Component_Map)
		{
			std::unique_ptr<componentMapType> testMap(new componentMapType());
			ComponentManager testComponents(std::move(testMap));
			const char* componentID = "I am a test component!";
			DebugComponent* testComponent = new DebugComponent(componentID);

			testComponents.AddComponent("testComponent", testComponent);

			IComponent* gotComponent = testComponents.GetComponentList()->find("testComponent")->second;

			Assert::AreEqual((int)testComponent, (int)gotComponent, TEXT("Test component and component in map are same"));

		}

		TEST_METHOD(Basic_Key_Movement_Sends_Test_Message)
		{
			std::unique_ptr<componentMapType> testMap(new componentMapType());
			ComponentManager testComponents(std::move(testMap));
			const char* componentID = "I am a test component!";
			int result = glfwInit();
			GLFWwindow* win = glfwCreateWindow(640, 480, "Test", NULL, NULL);

			BasicKeyMovement* testComponent = new BasicKeyMovement(componentID, win);

			testComponents.AddComponent("testComponent", testComponent);

			IComponent* gotComponent = testComponents.GetComponentList()->find("testComponent")->second;

			Assert::AreEqual((int)testComponent, (int)gotComponent, TEXT("Test component and component in map are same"));

			mauvemessage::RecieverInfo rec;
			rec.listenobjectptr = this;
			rec.listnerFunction = &MLTEST::ComponentTest::MessageHandler;
			rec.typeToListen = "keyboardMovement";
			std::unordered_multimap<const char*, mauvemessage::RecieverInfo>* listners = new std::unordered_multimap<const char*, mauvemessage::RecieverInfo>();
			mauvemessage::MessageManager::MessageManager(listners);
			mauvemessage::MessageManager::AddMessageListner("keyboardMovement", rec);

			testComponent->TestMessage();

			//Handler should be called now
			testComponent->Update(1.0f);

			Assert::IsTrue(testHasPassed, TEXT("Message handler hasn't been called correctly"));


		}

		TEST_METHOD(Removing_Component_From_Component_Map_Removes_It_From_The_Map)
		{
			Assert::Fail(TEXT("Test not implemented"));
		}

		TEST_METHOD(Getting_Componentes_Of_Type_Returns_Correct_Components)
		{
			Assert::Fail(TEXT("Test not implemented"));
		}

		TEST_METHOD(Getting_Component_Type_When_None_Exist_Return_Vector_Size_0)
		{
			//Make an empty map
			std::unique_ptr<componentMapType> testMap(new componentMapType());
			ComponentManager testComponents(std::move(testMap));

			const char* componentID = "I am a test component!";
			DebugComponent* testComponent = new DebugComponent(componentID);

			testComponents.AddComponent("testComponent", testComponent);

			//Try and get something with a category that isn't in the map
			std::vector<IComponent*> gotComponents = testComponents.GetComponentsOfType("nothing!");
			Assert::AreEqual(0, (int)gotComponents.size(), TEXT("No components should be got"));


		}
	
	};

bool MLTEST::ComponentTest::testHasPassed = false;

}