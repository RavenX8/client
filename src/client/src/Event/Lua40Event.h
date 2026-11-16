#pragma once

class CEvent;

/// The game's event (.con) scripts are Lua 4.0 bytecode, which Lua 5.4 cannot
/// load. They run here, in a Lua 4.0 state (3rdParty/lua-4.0.1, renamed so it
/// links beside 5.4) with the quest and game functions (QF_*, GF_*)
/// registered as QF_Init does. No Lua type crosses this interface.
namespace lua40 {
struct Script;

/// A state with the base, string and math libraries and the game's functions.
Script* Open();
void    Close(Script* script);

/// Runs a chunk (an event's script); 0 when it loaded and ran.
int Run(Script* script, const char* buffer, int size, const char* name);

/// Calls `function` with the event; its integer result, or 0x80000000 when
/// there is no such function or it failed.
int CallInt(Script* script, const char* function, CEvent* event);
}
