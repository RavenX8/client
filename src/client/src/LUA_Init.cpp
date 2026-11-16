/*
	$Header: /Client/LUA_Init.cpp 6     04-04-07 10:27a Jeddli $
*/
#include "StdAfx.h"
#include "Event/Quest_FUNC.h"
#include "Game_FUNC.h"
//-------------------------------------------------------------------------------------------------

#include "game_func_def.inc"
#include "Event/Quest_FUNC_DEF.inc"

// Global
void GF_Init(lua_State* L) {
#include "game_func_reg.inc"
}

// Quest
void QF_Init(lua_State* L) {
#include "game_func_reg.inc"
#include "Event/Quest_FUNC_REG.inc"
#include "Event/Quest_VARS.inc"
}

//-------------------------------------------------------------------------------------------------
