/** 
 * @file zz_font_d3d.h
 * @brief 3d font class
 * @author Jiho Choi (zho@korea.com)
 * @version 1.0
 * @date    15-nov-2003
 *
 * $Header: /engine/include/zz_font_d3d.h 15    04-10-14 10:37a Zho $
 */

#ifndef __ZZ_FONT_D3D_H__
#define __ZZ_FONT_D3D_H__

#ifndef __ZZ_FONT_H__
#include "zz_font.h"
#endif

#ifndef __ZZ_STRING_H__
#include "zz_string.h"
#endif

#ifndef __ZZ_FONT_TEXTURE_D3D_H__
#include "zz_font_texture_d3d.h"
#endif

#include <d3d9.h>
#include <d3dx9.h>

//--------------------------------------------------------------------------------
class zz_font_d3d : public zz_font {
  //--------------------------------------------------------------------------------
private:
  INT _draw_text(LPD3DXSPRITE pSprite, const char* pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color); // just wrapper for DrawText

protected:
  LPD3DXFONT        _d3d_font;
  LPDIRECT3DDEVICE9 _d3d_device;

  int                 _logical_pixels_y; // get by GetDeviceCaps
  int                 _num_font_textures;
  zz_font_texture_d3d _d3d_font_texture;

  // this is primitive virtual functions
  void draw_text_prim(const zz_font_text&                text_item) override;
  void draw_text_prim_offset(const zz_font_text&         text_item, float offsetx, float offsety) override;
  void draw_text_prim_outline_simple(const zz_font_text& text_item) override;
  void draw_text_prim_shadow_simple(const zz_font_text&  text_item) override;

  zz_font_texture* get_font_texture() override {
    if ( _num_font_textures == 0 ) return nullptr;
    return static_cast<zz_font_texture*>(&_d3d_font_texture);
  }

public:
          zz_font_d3d();
  virtual ~zz_font_d3d();

  // from node
  bool load() override;
  bool unload() override;

  bool get_text_extent(const char* text, zz_size* extent) override;

  // restoring device stuff
  bool restore_device_objects() override;    // create unmanaged objects
  bool init_device_objects() override;       // create managed objects
  bool invalidate_device_objects() override; // destroy unmanaged objects
  bool delete_device_objects() override;     // destroy managed objects

  void set_cache_property(int num_textures_in, int texture_width_in, int texture_height_in) override;

ZZ_DECLARE_DYNAMIC(zz_font_d3d);
};

#endif // __ZZ_FONT_D3D_H__
