#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include <LuaBridge/LuaBridge.h>


class ScriptManager
{
public:
	ScriptManager();
	~ScriptManager();

	bool Init();

	void Close();

private:
	lua_State *luaVM;

	void bindElements();

};

#endif;