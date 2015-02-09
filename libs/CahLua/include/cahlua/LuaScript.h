#ifndef CahLuaScript_h
#define CahLuaScript_h

#include "lua/lua.hpp"
#include "LuaState.h"

#include <vector>
#include <string>

namespace CahLua
{
	class Script
	{
	public:
		Script(bool global = true, std::string filename = "");

		int load(std::string);


		int execute();
		int callFunc(std::string funcName);

		int getError();
		std::string getEnvName();

	private:
		static int uid;

		int error;
		bool global;

		std::string envName;

		void handleError();
	};
}
#endif