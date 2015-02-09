#ifndef CahLuaClass_h
#define CahLuaClass_h

#include "lua/lua.hpp"
#include "LuaState.h"

#include <map>

namespace CahLua{
	class LuaClass
	{
	public:
		LuaClass();
		LuaClass(std::string className);
		~LuaClass();

		static LuaClass& createClass(std::string classname);

		LuaClass& addConstructor(void* func);
		LuaClass& addDestructor(void* func);
		LuaClass& addFunc(std::string funcName, void* func);

		void bind();


	private:
		std::string className;


		std::map<std::string, void*> funcList;
		static std::map<std::string, LuaClass> classList;
	};
}
#endif