/** 
 * @file zz_script_lua.h
 * @brief script for lua class.
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    28-nov-2002
 *
 * $Header: /engine/include/zz_script_lua.h 2     03-12-23 10:51a Zho $
 * $History: zz_script_lua.h $
 * 
 * *****************  Version 2  *****************
 * User: Zho          Date: 03-12-23   Time: 10:51a
 * Updated in $/engine/include
 * null filename bug fixed
 * 
 * *****************  Version 1  *****************
 * User: Zho          Date: 03-11-30   Time: 8:06p
 * Created in $/engine/include
 * 
 * *****************  Version 1  *****************
 * User: Zho          Date: 03-11-30   Time: 7:57p
 * Created in $/engine_1/include
 * 
 * *****************  Version 1  *****************
 * User: Zho          Date: 03-11-30   Time: 7:54p
 * Created in $/engine/engine/include
 * 
 * *****************  Version 1  *****************
 * User: Zho          Date: 03-11-30   Time: 7:48p
 * Created in $/engine/msvc71/include
 * 
 * *****************  Version 1  *****************
 * User: Zho          Date: 03-11-30   Time: 6:20p
 * Created in $/Engine/BUILD/msvc71/msvc71.root/msvc71/include
 * 
 * *****************  Version 1  *****************
 * User: Zho          Date: 03-11-30   Time: 6:08p
 * Created in $/Engine/INCLUDE
 * 
 * *****************  Version 6  *****************
 * User: Zho          Date: 03-11-28   Time: 10:44p
 * Updated in $/znzin11/engine/include
 * parameter evaluation ordering problem fixed. adjust compiler option not
 * to inline.
 * 
 * *****************  Version 5  *****************
 * User: Zho          Date: 03-11-28   Time: 2:58p
 * Updated in $/znzin11/engine/include
 * small code cleanup #include
 */

#ifndef __ZZ_SCRIPT_LUA_H__
#define __ZZ_SCRIPT_LUA_H__

#ifndef __ZZ_SCRIPT__
#include "zz_script.h"
#endif

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
  //#include "c:/code/tolua/include/tolua.h"
}

class zz_script_lua : public zz_script {
private:
  lua_State* L;
  int        param_count;

  void set_initial_globals();

public:
          zz_script_lua(const char* filename = nullptr);
  virtual ~zz_script_lua();

  bool do_script(const char* filename = nullptr, const char* buffer = nullptr) override;

  virtual bool get_global(const char* variable_name, int&   variable_to_set);
  virtual bool get_global(const char* variable_name, char*  variable_to_set);
  virtual bool get_global(const char* variable_name, float& variable_to_set);

  void set_global(const char* variable_name, int         value);
  void set_global(const char* variable_name, float       value);
  void set_global(const char* variable_name, const char* value);

  //bool get_table_size (const char * table_name, int& size) = 0;
  //bool get_table_item_string (const char * table_name, int index, char * string_to_set) = 0;

  virtual void set_param_uint(unsigned int   val);
  void         set_param_int(int             val) override;
  void         set_param_float(float         val) override;
  void         set_param_float3(const float* val) override;
  void         set_param_string(const char*  val) override;
  virtual void set_param_hnode(void*         val);

  bool call(const char* function_name, va_list va) override;

  // get return value after lua call for callXXX interface
  int          get_return_int(void   ) override;
  unsigned int get_return_uint(void  ) override;
  float        get_return_float(void ) override;
  const float* get_return_float3(void) override;
  const char*  get_return_string(void) override;

ZZ_DECLARE_DYNAMIC(zz_script_lua);
};

#endif //__ZZ_SCRIPT_LUA_H__
