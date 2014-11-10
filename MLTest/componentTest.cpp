#include "stdafx.h"
#include "CppUnitTest.h"
#include "Assert\Assert.h"
#include "Components\ComponentManager.h"
#include "Components\DebugComponent.h"

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