#ifndef CahLuaBase_h
#define CahLuaBase_h

#include "lua/lua.hpp"
#include "LuaState.h"

namespace CahLua{

	bool open();
	void close();
}
#endif