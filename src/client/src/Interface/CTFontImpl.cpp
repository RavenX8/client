#include "StdAfx.h"
#include "./CTFontImpl.h"
#include "../Game.h"
#include "zz_interface.h"

CTFontImpl g_FontImpl;

CTFontImpl::CTFontImpl(void) {}

CTFontImpl::~CTFontImpl(void) {}

void CTFontImpl::Draw(int iFont, bool bUseSprite, RECT* pRect, D3DCOLOR Color, DWORD dwFormat, const char* pMsg) {
  if ( pMsg == nullptr )
    return;
  int iFontIndex = iFont;
  if ( iFont < 0 || iFont >= MAX_FONT )
    iFontIndex = 0;

  drawFont( g_GameDATA.m_hFONT[iFontIndex], bUseSprite, pRect, Color, dwFormat, pMsg );
}

void CTFontImpl::Draw(int iFont, bool bUseSprite, int x, int y, D3DCOLOR Color, const char* pMsg) {
  if ( pMsg == nullptr || iFont < 0 || iFont >= MAX_FONT )
    return;

  int iFontIndex = iFont;
  if ( iFont < 0 || iFont >= MAX_FONT )
    iFontIndex = 0;

  drawFont( g_GameDATA.m_hFONT[iFontIndex], bUseSprite, x, y, Color, pMsg );
}

int CTFontImpl::GetFontHeight(int iFont) {
  if ( iFont < 0 || iFont >= MAX_FONT )
    return 0;
  return getFontHeight( g_GameDATA.m_hFONT[iFont] );
}

SIZE   CTFontImpl::GetFontTextExtent(int iFont, const char* pszText) {
  SIZE size = { 0, 0 };
  if ( iFont < 0 || iFont >= MAX_FONT )
    return size;
  if ( pszText == nullptr )
    return size;
  return getFontTextExtent( g_GameDATA.m_hFONT[iFont], pszText );
}

void         CTFontImpl::SetTransformSprite(float x, float y, float z) {
  D3DXMATRIX mat;
  D3DXMatrixTranslation( &mat, x, y, z );
  setTransformSprite( mat );
}
