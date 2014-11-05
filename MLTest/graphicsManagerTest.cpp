#include "stdafx.h"
#include "CppUnitTest.h"
#include "Assert\Assert.h"
#include "Graphics\GraphicsManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MLTEST
{

	TEST_CLASS(GraphicsManagerTest)
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
			MLTEST::GraphicsManagerTest::testHasPassed = false;
			mauveassert::Assert::InitExceptionHandler(&MLTEST::GraphicsManagerTest::ExceptionHandler);
		}

		TEST_METHOD_INITIALIZE(Set_Up_Exception_Handler)
		{
			init();
		}

		TEST_METHOD(Creating_Window_Returns_Success)
		{
			GraphicsManager testGraphics = GraphicsManager();

			Assert::IsTrue(testGraphics.Init(), TEXT("Initing GLFW"));

			//Arbitary size - not testing that here.
			bool result = testGraphics.CreateGraphicsWindow(640, 480, "Test Window");

			Assert::IsTrue(result, TEXT("Create window success"));

			glfwTerminate();
		}

		TEST_METHOD(Creating_Window_Creates_Valid_Window_Pointer)
		{
			GraphicsManager testGraphics = GraphicsManager();
			Assert::IsTrue(testGraphics.Init(), TEXT("Initing GLFW"));

			//Arbitary size - not testing that here.
			bool result = testGraphics.CreateGraphicsWindow(640, 480, "Test Window");

			Assert::IsTrue(result, TEXT("Create window success"));

			Assert::IsNotNull(testGraphics.GetCurrentWindow(), TEXT("Window is stored as a pointer in the class"));

			glfwTerminate();
		}

		TEST_METHOD(Creating_Window_Of_Invalid_Size_Returns_Fail)
		{
			GraphicsManager testGraphics = GraphicsManager();
			
			Assert::IsTrue(testGraphics.Init(), TEXT("Initing GLFW"));

			bool result = testGraphics.CreateGraphicsWindow(-10, -10, "Test Window");

			Assert::IsFalse(result, TEXT("Create window should return fail"));

			glfwTerminate();
		}

		TEST_METHOD(Creating_A_Window_When_A_Window_Is_Already_Present_Fails)
		{
			GraphicsManager testGraphics = GraphicsManager();

			Assert::IsTrue(testGraphics.Init(), TEXT("Initing GLFW"));
			//Arbitary size - not testing that here.
			bool result1 = testGraphics.CreateGraphicsWindow(100, 100, "Test Window");

			Assert::IsTrue(result1, TEXT("Create window success"));

			bool result2 = testGraphics.CreateGraphicsWindow(100, 100, "Test Window 2");

			Assert::IsFalse(result2, TEXT("Create window should return false"));

			glfwTerminate();
		}

		TEST_METHOD(Creating_Window_With_Null_Title_Returns_Fail)
		{
			GraphicsManager testGraphics = GraphicsManager();
			
			Assert::IsTrue(testGraphics.Init(), TEXT("Initing GLFW"));

			bool result = testGraphics.CreateGraphicsWindow(640, 480, nullptr);

			Assert::IsFalse(result, TEXT("Create window should return fail"));

			glfwTerminate();
		}

		TEST_METHOD(Loading_A_Shader_From_File_Puts_It_Into_The_map)
		{
			Assert::Fail(TEXT("Not Implemented"));
		}

		TEST_METHOD(Loading_An_Invalid_Shader_Returns_False)
		{
			Assert::Fail(TEXT("Not Implemented"));
		}

		TEST_METHOD(Getting_A_Valid_Shader_Returns_Its_ID)
		{
			Assert::Fail(TEXT("Not Implemented"));
		}

		TEST_METHOD(Getting_A_Shader_That_Doesnt_Exist_Returns_minus_1)
		{
			Assert::Fail(TEXT("Not Implemented"));
		}

	};

bool MLTEST::GraphicsManagerTest::testHasPassed = false;

}