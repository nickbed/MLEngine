#ifndef ASSERT_H
#define ASSERT_H

#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>



namespace mauveassert
{
	typedef void (*FEP)(const char* message);

	enum class ENUM_severity
	{
		SEV_FATAL,
		SEV_ERROR, 
		SEV_WARNING,
		SEV_INFO
	};

	class Assert
	{
	public:
		Assert();

		static void InitExceptionHandler(void(*handler)(const char*))
		{
			fatalExceptionHandler = handler;
		}

			//Writes some coloured debug text to the console
		static void WriteDebug(std::string DebugText, ENUM_severity sev)
		{
			switch(sev)
			{
			case 0: //error - red
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0xC );
				break;
			case 1: //warning - yellow
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0xE );
				break;
			case 2: //info - green
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0xA );
				break;
			}

			std::cout << DebugText << std::endl;

			SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0x0F );
		}

		static void WriteDebug(std::string DebugText, int number, ENUM_severity sev)
		{
			std::stringstream result;

			result << DebugText << " " << number;

			WriteDebug(result.str(), sev);
		}

		static void HandleAssert(ENUM_severity sev, char* message)
		{
			//Get out, something has gone very wrong
			if(sev == ENUM_severity::SEV_FATAL)
			{
				HandleException(message);
				return;
			}
			WriteDebug(message, sev);
		}

		static void HandleException(const char* message)
		{
			//Get out, something has gone very wrong
			WriteDebug("A fatal exception has occured",ENUM_severity::SEV_FATAL);
			WriteDebug(message,ENUM_severity::SEV_FATAL);
			fatalExceptionHandler(message);
		}

		static void AssertTrue(const char* name, bool valueToAssert, ENUM_severity sev)
		{
			if(valueToAssert)
			{
				std::stringstream result;
				result << "ASSERT FAILED: " << name;
				std::string temp = result.str();
				HandleAssert(sev, const_cast<char*>(temp.c_str()));
			}
		}

		static void AssertFalse(const char* name, bool valueToAssert, ENUM_severity sev)
		{
			if(valueToAssert)
			{
				std::stringstream result;
				result << "ASSERT FAILED: " << name;
				std::string temp = result.str();
				HandleAssert(sev, const_cast<char*>(temp.c_str()));
			}
		}

	private:
		static FEP fatalExceptionHandler;
		
	};
}

#endif