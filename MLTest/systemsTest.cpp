
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Assert\Assert.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#include "Graphics\GraphicsManager.h"
namespace MLTEST
{

	
	TEST_CLASS(SystemsTest)
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
			MLTEST::SystemsTest::testHasPassed = false;
			mauveassert::Assert::InitExceptionHandler(&MLTEST::SystemsTest::ExceptionHandler);
		}

		TEST_METHOD_INITIALIZE(Set_Up_Exception_Handler)
		{
			init();
		}

		//TEST_METHOD(Creating_Window_Returns_Success)
		//{
		//	GraphicsManager testGraphics = GraphicsManager();

		//	Assert::IsTrue(testGraphics.Init(), TEXT("Initing GLFW"));

		//	//Arbitary size - not testing that here.
		//	bool result = testGraphics.CreateGraphicsWindow(640, 480, "Test Window");

		//	Assert::IsTrue(result, TEXT("Create window success"));

		//	glfwTerminate();
		//}

	};

bool MLTEST::SystemsTest::testHasPassed = false;

}