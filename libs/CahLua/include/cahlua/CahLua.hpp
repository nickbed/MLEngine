// CahLua.hpp
// Lua header files for C++
// <<extern "C">> not supplied automatically because Lua also compiles as C++

//#include "CahLua.h"



#define CAHLUA_VERSION_MAJOR 0
#define CAHLUA_VERSION_MINOR 1
#define CAHLUA_LUA_VERSION_MAJOR 5
#define CAHLUA_LUA_VERSION_MINOR 2
#define CAHLUA_LUA_VERSION_REVISION 3

#include "lua/lua.hpp"

#include "LuaBase.h"
#include "LuaScript.h"
#include "LuaState.h"
#include "LuaMetaPointer.h"
#include "LuaClass.h"