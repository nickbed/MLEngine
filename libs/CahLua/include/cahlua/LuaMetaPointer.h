#ifndef CAHLUA_METAPOINTER_H
#define CAHLUA_METAPOINTER_H
//-----------------------------------------------------------------------------
#define CahLua_MetaPointer     "cahlua.metapointer"    //-- Name given to the metatable for all lightuserdata (instances of LMetaPointer in C++)
//-----------------------------------------------------------------------------s
#include "lua/lua.hpp"
#include "LuaState.h"

#include <map>
#include <string>

namespace CahLua 
{
	class MetaPointer
	{
		public:
			MetaPointer();
			virtual ~MetaPointer();

			inline lua_State*  lua()    { return m_lua; }
			inline std::string global() { return m_name; }
			inline size_t      size()   { return m_pointers.size(); }

			void setGlobal(std::string n);      //-- Shall make this pointer globally accessible to LUA
			void setLocal(std::string envName, std::string n);      //-- Shall make this pointer accessible to the environment name provided

			virtual int get(lua_State* L) = 0;  //-- To be implemented by inherited classes
			virtual int set(lua_State* L) = 0;  //-- To be implemented by inherited classes

			MetaPointer* operator [](std::string n);


			static void initialize(lua_State* L);       //-- Creates the metatable (only once) and publishes it

		private:
			static lua_State*                           m_lua;           //-- All LMetaPointers will share a common lua State
			static const luaL_Reg                       m_lmembers[];    //-- Member functions (for later expansion)
			static const luaL_Reg                       m_lfunctions[];  //-- Metamethods
			static std::map<MetaPointer*, std::string>  m_pointers;      //-- List of all LMetaPointer instances

			std::string m_name;                  //-- Name of LUA global variable pointing to me.

			static int __index(lua_State* L);    //-- Shall be mapped to __index metamethod of the metatable for all lightuserdata pointers
			static int __newindex(lua_State* L); //-- Shall be mapped to __newindex metamethod of the metatable for all lightuserdata pointers

		
	};
}
#endif