#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include <LuaBridge/LuaBridge.h>

#include "ClassHelpers.h"
#include "../Interfaces/IEntity.h"
#include "../Components/Script.h"
#include "../Components/BasicKeyMovement.h"


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