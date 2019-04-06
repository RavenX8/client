/** 
 * @file zz_cursor.h
 * @brief cursor class
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    24-jul-2004
 *
 * $Header: /engine/include/zz_cursor.h 4     04-08-11 6:20p Zho $
 */

#ifndef __ZZ_CURSOR_H__
#define __ZZ_CURSOR_H__

#include <d3d9.h>

#ifdef WIN32
//--------------------------------------------------------------------------------
class zz_cursor : public zz_node {
private:
  LPDIRECT3DSURFACE9 surface_;
  HCURSOR            hcursor_;
  int                xhotspot_, yhotspot_;
  static bool        d3d_mouse_; // if this is d3d hardware mouse or not

  static zz_cursor* current; // which cursor is the current running cursor. this is set by show_sursor()

  static HRESULT CreateCursorSurface(LPDIRECT3DDEVICE9 pd3dDevice, LPDIRECT3DSURFACE9& pCursorSurface, HCURSOR hCursor, int& xhotspot_out, int& yhotspot_out, BOOL bAddWatermark);

  static LPDIRECT3DDEVICE9 get_device();

public:
          zz_cursor();
  virtual ~zz_cursor();

  void set_property(HCURSOR hcursor);

  bool load() override;

  bool unload() override;

  void        set_position(int x, int y);
  void        show_cursor(bool show_in);
  static void hide_cursor();

  // restoring device stuff
  bool restore_device_objects() override;    // create unmanaged objects
  bool init_device_objects() override;       // create managed objects
  bool invalidate_device_objects() override; // destroy unmanaged objects
  bool delete_device_objects() override;     // destroy managed objects

ZZ_DECLARE_DYNAMIC(zz_cursor);
};

#endif // WIN32

#endif // __ZZ_CURSOR_H__
