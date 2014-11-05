#include "stdafx.h"
#include "CppUnitTest.h"
#include "Assert\Assert.h"
#include "File\FileManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MLTEST
{

	TEST_CLASS(FileTest)
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
			MLTEST::FileTest::testHasPassed = false;
			mauveassert::Assert::InitExceptionHandler(&MLTEST::FileTest::ExceptionHandler);
		}

		TEST_METHOD_INITIALIZE(Set_Up_Exception_Handler)
		{
			init();
		}

		TEST_METHOD(Invalid_File_Returns_Empty_String)
		{
			std::string result = mauvefile::FileManager::ReadFile("somefilethatdoesntexist", true);
			Assert::IsTrue(result.size() == 0, TEXT("Returned string should have size of 0"));
		}
	
	};

bool MLTEST::FileTest::testHasPassed = false;

}