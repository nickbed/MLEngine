#include "stdafx.h"
#include "CppUnitTest.h"
#include "Assert\Assert.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MLTEST
{

	TEST_CLASS(AssertTest)
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
			MLTEST::AssertTest::testHasPassed = false;
			mauveassert::Assert::InitExceptionHandler(&MLTEST::AssertTest::ExceptionHandler);
		}

		TEST_METHOD_INITIALIZE(Set_Up_Exception_Handler)
		{
			init();
		}

		TEST_METHOD(Assert_True_Should_Assert_On_False)
		{
			mauveassert::Assert::AssertTrue("Testing assert true", false, mauveassert::ENUM_severity::SEV_FATAL);

			//Should drop into the assert handler and set testHasPassed to true;
			Assert::IsTrue(testHasPassed, TEXT("Assert has not been handled correctly testHasPassed is false"));
		}

		TEST_METHOD(Assert_False_Should_Assert_On_True)
		{
			mauveassert::Assert::AssertFalse("Testing assert false", true, mauveassert::ENUM_severity::SEV_FATAL);

			//Should drop into the assert handler and set testHasPassed to true;
			Assert::IsTrue(testHasPassed, TEXT("Assert has not been handled correctly testHasPassed is false"));
		}
	
	};

bool MLTEST::AssertTest::testHasPassed = false;

}