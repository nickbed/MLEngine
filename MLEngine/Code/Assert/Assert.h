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
			case ENUM_severity::SEV_ERROR: //error - red
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0xC );

				break;
			case ENUM_severity::SEV_WARNING: //warning - yellow
				SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0xE );
				break;
			case ENUM_severity::SEV_INFO: //info - green
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

		static void WriteDebug(std::string DebugText, std::string MoreDebugText, ENUM_severity sev)
		{
			std::stringstream result;

			result << DebugText << " " << MoreDebugText;

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

		static bool AssertTrue(const char* name, bool valueToAssert, ENUM_severity sev)
		{
			if(!valueToAssert)
			{
				std::stringstream result;
				result << "ASSERT FAILED: " << name;
				std::string temp = result.str();
				HandleAssert(sev, const_cast<char*>(temp.c_str()));
				return false;
			}
			return true;
		}

		static bool AssertFalse(const char* name, bool valueToAssert, ENUM_severity sev)
		{
			//Invert assert value
			return AssertTrue(name, !valueToAssert, sev); 
		}

	private:
		static FEP fatalExceptionHandler;
		
	};

}

//Assert Macros
#define NULLPTRCHECK(ptr, msg) ptr == nullptr?mauveassert::Assert::HandleException(msg):0;
#define DEBUGWRITEINFO(info, value) mauveassert::Assert::WriteDebug(info, value, mauveassert::ENUM_severity::SEV_INFO);

#endif