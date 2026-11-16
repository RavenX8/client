// The Lua 4.0 side of the event scripts (Lua40Event.h). Not built with the
// precompiled header: that pulls in Lua 5.4, whose include guards match 4.0's.
// Under lua40_names.h, lua_State and the API are 4.0's, so the declarations in
// LUA_Func.h and the generated *_FUNC_DEF.inc wrappers below become Lua 4.0
// overloads, distinct from the 5.4 ones in LUA_Func.cpp and LUA_Init.cpp.
// The game's usual includes, without classLUA.h (Lua 5.4): its guard first.
#define __CLASSLUA_H
#include "../StdAfx.h"
#include <cstdarg>
#include <cstdio>

#include "lua-4.0.1/include/lua40_names.h"
extern "C" {
#include "lua-4.0.1/include/lua.h"
#include "lua-4.0.1/include/lualib.h"
#include "lua-4.0.1/include/lauxlib.h"
}

#include "Quest_FUNC.h"
#include "../Game_FUNC.h"
#include "../Util/CrashLog.h"
#include "Lua40Event.h"

//-------------------------------------------------------------------------------------------------
// The glue's parameter helpers, for Lua 4.0 (as before the move to 5.4).

static int param_index(lua_State* L, int& index_from_end) {
  int index = lua_gettop( L ) - index_from_end;
  index_from_end++;
  return index;
}

unsigned int get_param_uint(lua_State* L, int& index_from_end, const char* where) {
  int index = param_index( L, index_from_end );
  if ( index < 1 || !lua_isnumber( L, index ) ) {
    DevLog( "lua40 %s: a parameter is not a number", where );
    return 0;
  }
  return (unsigned int)lua_tonumber( L, index );
}

unsigned long long get_param_uint64(lua_State* L, int& index_from_end, const char* where) {
  return get_param_uint( L, index_from_end, where );
}

int get_param_int(lua_State* L, int& index_from_end, const char* where) {
  int index = param_index( L, index_from_end );
  if ( index < 1 || !lua_isnumber( L, index ) ) {
    DevLog( "lua40 %s: a parameter is not a number", where );
    return 0;
  }
  return (int)lua_tonumber( L, index );
}

float get_param_float(lua_State* L, int& index_from_end, const char* where) {
  int index = param_index( L, index_from_end );
  if ( index < 1 || !lua_isnumber( L, index ) ) {
    DevLog( "lua40 %s: a parameter is not a number", where );
    return 0;
  }
  return (float)lua_tonumber( L, index );
}

float* get_param_float3(lua_State* L, int& index_from_end, const char* where) {
  static float ret[3] = { 0, 0, 0 };
  int index = lua_gettop( L ) - index_from_end;
  index_from_end += 3;
  if ( index < 3 || !lua_isnumber( L, index - 2 ) || !lua_isnumber( L, index - 1 ) || !lua_isnumber( L, index ) ) {
    DevLog( "lua40 %s: a parameter is not three numbers", where );
    return ret;
  }
  ret[0] = (float)lua_tonumber( L, index - 2 );
  ret[1] = (float)lua_tonumber( L, index - 1 );
  ret[2] = (float)lua_tonumber( L, index );
  return ret;
}

const char* get_param_string(lua_State* L, int& index_from_end, const char* where) {
  int index = param_index( L, index_from_end );
  if ( index < 1 || !lua_isstring( L, index ) ) {
    DevLog( "lua40 %s: a parameter is not a string", where );
    return "";
  }
  return lua_tostring( L, index );
}

void set_param_uint(lua_State* L, int& return_num, unsigned int val) {
  lua_pushnumber( L, val );
  return_num++;
}

void set_param_uint64(lua_State* L, long long& return_num, unsigned long long val) {
  lua_pushnumber( L, (double)val );
  return_num++;
}

void set_param_int(lua_State* L, int& return_num, int val) {
  lua_pushnumber( L, val );
  return_num++;
}

void set_param_float(lua_State* L, int& return_num, float val) {
  lua_pushnumber( L, val );
  return_num++;
}

void set_param_float3(lua_State* L, int& return_num, const float* val) {
  lua_pushnumber( L, val[0] );
  lua_pushnumber( L, val[1] );
  lua_pushnumber( L, val[2] );
  return_num += 3;
}

void set_param_string(lua_State* L, int& return_num, const char* val) {
  lua_pushstring( L, val );
  return_num++;
}

void set_global(lua_State* L, const char* variable_name, int value) {
  lua_pushnumber( L, value );
  lua_setglobal( L, variable_name );
}

void set_global(lua_State* L, const char* variable_name, float value) {
  lua_pushnumber( L, value );
  lua_setglobal( L, variable_name );
}

void set_global(lua_State* L, const char* variable_name, const char* value) {
  lua_pushstring( L, value );
  lua_setglobal( L, variable_name );
}

//-------------------------------------------------------------------------------------------------
// The game's functions, wrapped for Lua 4.0 from the same generated lists.

#include "../game_func_def.inc"
#include "Quest_FUNC_DEF.inc"

static void register_functions(lua_State* L) {
#include "../game_func_reg.inc"
#include "Quest_FUNC_REG.inc"
#include "Quest_VARS.inc"
}

// A script's errors (Lua 4.0 calls _ERRORMESSAGE, then _ALERT) to client.log.
static int log_alert(lua_State* L) {
  DevLog( "lua40: %s", lua_isstring( L, 1 ) ? lua_tostring( L, 1 ) : "(error)" );
  return 0;
}

//-------------------------------------------------------------------------------------------------

namespace lua40 {
struct Script {
  lua_State* state;
};

Script* Open() {
  lua_State* L = lua_open( 0 );
  if ( !L )
    return nullptr;
  lua_baselibopen( L );
  lua_strlibopen( L );
  lua_mathlibopen( L );
  lua_register( L, "_ALERT", log_alert );
  register_functions( L );
  return new Script{ L };
}

void Close(Script* script) {
  if ( !script )
    return;
  lua_close( script->state );
  delete script;
}

int Run(Script* script, const char* buffer, int size, const char* name) {
  if ( !script )
    return -1;
  return lua_dobuffer( script->state, buffer, size, name );
}

int CallInt(Script* script, const char* function, CEvent* event) {
  const int failed = (int)0x80000000;
  if ( !script || !function )
    return failed;
  lua_State* L   = script->state;
  int        top = lua_gettop( L );
  lua_getglobal( L, function );
  if ( !lua_isfunction( L, -1 ) ) {
    DevLog( "lua40: no function %s", function );
    lua_settop( L, top );
    return failed;
  }
  // The event as the scripts were given it (an int); none of the functions
  // turns it back into a pointer.
  lua_pushnumber( L, (double)(int)(INT_PTR)event );
  int err    = lua_call( L, 1, 1 );
  int result = failed;
  if ( err == 0 && lua_gettop( L ) > top && lua_isnumber( L, -1 ) )
    result = (int)lua_tonumber( L, -1 );
  else if ( err != 0 )
    DevLog( "lua40: %s failed (%d)", function, err );
  lua_settop( L, top );
  return result;
}
}
