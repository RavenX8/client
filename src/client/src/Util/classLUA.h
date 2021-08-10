#ifndef	__CLASSLUA_H
#define __CLASSLUA_H

extern "C" {
#include "lua.h"
#include "lualib.h"
//#include "luadebug.h"
#ifdef ZZ_LUA500
#include "lauxlib.h"
#endif
}

//-------------------------------------------------------------------------------------------------

/*
LUA���� ȣ��ǵ��� ����� �Լ��� ����.
int lua_Write (lua_State *L)
{
  int i, n = lua_gettop(L);	//

  printf ("-----> lua_Write ArgCnt: %d\n", n);

  char *pStr;
  for (i=1; i<=n; i++) {
    pStr = (char*)lua_tostring(L, i);
    printf ("%s", pStr);
  }

  return 0;
}
*/

/*
    static index�� 1���� !!! 0���� �ƴ�.
*/
class classLUA {
private :
  long           m_lBufferSize;
  unsigned char* m_pBuffer;

public :
  lua_State* m_pState;

  classLUA(int iStackSize = 0);
  ~classLUA();

  // ����.
  int LoadToBuffer(const char* szFileName);
  int Do_File(const char*      szFileName) { return luaL_dofile( m_pState, szFileName ); }
  int Do_String(const char*    pString) { return luaL_dostring( m_pState, pString ); }
  int Do_Buffer(const char*    pBuffer, size_t iSize, const char* szNameOfTheChunk = nullptr);
  int Do_Buffer(void           ) { return ((m_pBuffer) ? Do_Buffer( (const char*)m_pBuffer, m_lBufferSize ) : -1); }

  // C �Լ� ���.
  void Reg_Function(const char* szFunctionName, lua_CFunction Function);

  // ���� �˻�.  iIndex == ���� �ε���
  int         Stack_QueryType(int iIndex) { return lua_type( m_pState, iIndex ); }   // ������ iIndex�� element �ڷ� ���¸� �Ǵ��Ѵ�.
  const char* Stack_TypeName(int  iType) { return lua_typename( m_pState, iType ); } // ������ Stack_QueryType���� ���� ������ �̸��� ��´�.
//  int         Stack_GetSpaceCount() { return lua_stackspace( m_pState ); }           // ������ ���� ������ ��´�.
  int         Stack_GetElementCount() { return lua_gettop( m_pState ); }             // ������ element ���� ��´�.
  void        Stack_Clear() { lua_settop( m_pState, 0 ); }                           // ������ ����.
  void        Stack_SetTop(int    iIndex) { lua_settop( m_pState, iIndex ); }        // ������ top index�� iIndex�� �ٲ۴�.
  void        Stack_PushValue(int iIndex) { lua_pushvalue( m_pState, iIndex ); }     // ������ iIndex�� element�� ���ؿ� push�Ѵ�.
  void        Stack_Remove(int    iIndex) { lua_remove( m_pState, iIndex ); }        // ������ iIndex�� �����Ѵ�.
  void        Stack_Move(int      iIndex) { lua_insert( m_pState, iIndex ); }        // ������ top element�� iIndex�� �ű��.
//  int         Stack_Tag(int       iIndex) { return lua_tag( m_pState, iIndex ); }

  void Stack_PushNumber(double      dbValue) { lua_pushnumber( m_pState, dbValue ); }
  void Stack_PushString(const char* pStr, size_t iLen = -1);
//  void Stack_PushUserTag(void*      pData, int   iTag) { lua_pushusertag( m_pState, pData, iTag ); }
  void Stack_PushNil() { lua_pushnil( m_pState ); }

  void Stack_PushCFunction(lua_CFunction Func) {
    lua_pushcfunction (m_pState, Func);
  }

  bool Is_Nil(int       iIndex) { return lua_isnil (m_pState, iIndex); }
//  bool Is_Null(int      iIndex) { return lua_isnull (m_pState, iIndex); }
  bool Is_Number(int    iIndex) { return (0 != lua_isnumber( m_pState, iIndex )); }
  bool Is_String(int    iIndex) { return (0 != lua_isstring( m_pState, iIndex )); }
  bool Is_CFunction(int iIndex) { return (0 != lua_iscfunction( m_pState, iIndex )); }
  bool Is_Table(int     iIndex) { return lua_istable (m_pState, iIndex); }
  bool Is_Function(int  iIndex) { return lua_isfunction (m_pState, iIndex); }
  bool Is_UserData(int  iIndex) { return lua_isuserdata (m_pState, iIndex); }

  double        To_Double(int    iIndex) { return lua_tonumber( m_pState, iIndex ); }
  int           To_Number(int    iIndex) { return (int)lua_tonumber( m_pState, iIndex ); }
  const char*   To_String(int    iIndex) { return lua_tostring( m_pState, iIndex ); }
//  size_t        StringLength(int iIndex) { return lua_strlen( m_pState, iIndex ); }
  lua_CFunction To_CFunction(int iIndex) { return lua_tocfunction( m_pState, iIndex ); }
  void*         To_UserData(int  iIndex) { return lua_touserdata( m_pState, iIndex ); }

  void GetGlobal(const char* VarName) { lua_getglobal( m_pState, VarName ); }
  void SetGlobal(const char* VarName) { lua_setglobal( m_pState, VarName ); }

  bool GetGlobalValue(const char* VarName, double& dbNumber);
  bool GetGlobalValue(const char* VarName, int&    iNumber);
  bool GetGlobalValue(const char* VarName, char*   pString);

  void SetGolbalValue(const char* VarName, double dbValue);
  void SetGolbalValue(const char* VarName, int    iValue);
  void SetGolbalValue(const char* VarName, char*  szValue);

#ifdef ZZ_LUA500
  int		Call(int iArgCnt, int iResultCnt) { return lua_pcall(m_pState, iArgCnt, iResultCnt, 0); }
#else // 40
  int Call(int iArgCnt, int iResultCnt) { return lua_call( m_pState, iArgCnt, iResultCnt ); }
#endif
};

inline int classLUA::Do_Buffer(const char* pBuffer, size_t iSize, const char* szNameOfTheChunk) {
  auto lua_error = luaL_loadbuffer( m_pState, pBuffer, iSize, szNameOfTheChunk );
  if (lua_error == LUA_OK) lua_error = lua_pcall(m_pState, 0, LUA_MULTRET, 0);
  if (lua_error != LUA_OK) {
    fprintf(stderr, "%s\n", lua_tostring(m_pState, -1));
    lua_pop(m_pState, 1);/* pop error message from the stack */
  }
  return lua_error;
}

inline void classLUA::Reg_Function(const char* szFunctionName, lua_CFunction Function) {
  lua_register(m_pState, szFunctionName, Function);
}

inline void classLUA::Stack_PushString(const char* pStr, size_t iLen) {
  if ( iLen > 0 )
    lua_pushlstring( m_pState, pStr, iLen );
  else
    lua_pushstring( m_pState, pStr );
}

//-------------------------------------------------------------------------------------------------
#endif
