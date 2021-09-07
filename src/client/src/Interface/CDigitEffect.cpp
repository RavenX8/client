#include "StdAfx.h"
#include "CDigitEffect.h"

#include <functional>

#include "..//Game.h"

/*
const char* _digit_Texture[ MAX_DIGIT_EFFECT ] = { "3DData\\Effect\\Special\\DigitWorkTex01.tga",
"3DData\\Effect\\Special\\DigitWorkTex02.tga",
"3DData\\Effect\\Special\\DigitWorkTex03.tga",
"3DData\\Effect\\Special\\DigitWorkTex04.tga",
"3DData\\Effect\\Special\\DigitWorkTex05.tga",
"3DData\\Effect\\Special\\DigitWorkTex06.tga",
"3DData\\Effect\\Special\\DigitWorkTex07.tga",
"3DData\\Effect\\Special\\DigitWorkTex08.tga",
"3DData\\Effect\\Special\\DigitWorkTex09.tga",
"3DData\\Effect\\Special\\DigitWorkTex10.tga",
};

*/

const char* _digit_Mesh[MAX_DIGIT_EFFECT] = {
  "3DData\\Effect\\Special\\DigitWork01.zms",
  "3DData\\Effect\\Special\\DigitWork01.zms",
  "3DData\\Effect\\Special\\DigitWork01.zms",
  "3DData\\Effect\\Special\\DigitWork01.zms",
  "3DData\\Effect\\Special\\DigitWork01.zms",
  "3DData\\Effect\\Special\\DigitWork01.zms",
  "3DData\\Effect\\Special\\DigitWork01.zms",
  "3DData\\Effect\\Special\\DigitWork01.zms",
  "3DData\\Effect\\Special\\DigitWork01.zms",
  "3DData\\Effect\\Special\\DigitWork01.zms",
};

#define DIGIT_WIDTH			32
#define DIGIT_HEIGHT		32
#define DIGIT_COUNT			8

CDigitEffect::CDigitEffect() {
  int i = 0;
  for (i = 0; i < MAX_DIGIT_EFFECT; i++) {
    m_DigitNode[i].m_hAnimatable = NULL;
    m_DigitNode[i].m_hMesh = NULL;
    m_DigitNode[i].m_hMat = NULL;
  }

  m_hMotion = NULL;

  m_DigitTex[DIGIT_NORMAL] = NULL;
  m_DigitTex[DIGIT_ATTACKED] = NULL;

  m_DigitClearTex = NULL;
  m_DigitMissTex = NULL;
}

CDigitEffect::~CDigitEffect() {
  /// ¿£Áø ÆÄ±«ÈÄ¿¡ È£ÃâµÇ±â¶§¹®¿¡..
  // ClearAll();
}

/// ÀÛ¾÷ °´Ã¼µé ÃÊ±âÈ­..
bool CDigitEffect::Init() {
  ClearAll();

  int i = 0;

  HNODE hDummy = NULL;
  HNODE hAnimatable = NULL;
  HNODE hMesh = NULL;
  HNODE hMat = NULL;

  int ZZ_INTERP_SQUAD = 3, ZZ_INTERP_CATMULLROM = 4;
  m_hMotion = loadMotion("DigitEffectMotion", "3DData\\Effect\\Special\\hit_figure_01.ZMO", 1, ZZ_INTERP_CATMULLROM, ZZ_INTERP_SQUAD, 1, 1);
  if (m_hMotion == NULL) {
    ClearAll();
    return false;
  }

  for (i = 0; i < MAX_DIGIT_EFFECT; i++) {
    hMat = loadNullColormapMaterial(CStr::Printf("DigitEffect_%d", i), g_GameDATA.m_hShader_nolit, DIGIT_WIDTH * DIGIT_COUNT, DIGIT_HEIGHT);
    if (hMat == NULL) {
      ClearAll();
      return false;
    }

    hMesh = loadMesh(CStr::Printf("DigitMesh_%d", i), "3DData\\Effect\\Special\\DigitWork01.zms");
    if (hMesh == NULL) {
      ClearAll();
      return false;
    }

    hAnimatable = loadAnimatable(CStr::Printf("DigitNode_%d", i), hMesh, m_hMotion, hMat, g_GameDATA.m_hShader_nolit);
    if (hAnimatable == NULL) {
      ClearAll();
      return false;
    }

    hDummy = loadVisible(CStr::Printf("DigitDummy_%d", i), 0, 0, 0);
    if (hDummy == NULL) {
      ClearAll();
      return false;
    }

    m_DigitNode[i].m_hPositionDummy = hDummy;
    m_DigitNode[i].m_hAnimatable = hAnimatable;
    m_DigitNode[i].m_hMesh = hMesh;
    m_DigitNode[i].m_hMat = hMat;

    /// Material attribute setting

    setMaterialBlendTypeCustom(hMat, D3DBLEND_SRCALPHA,
      D3DBLEND_INVSRCALPHA,
      D3DBLENDOP_ADD
    );

    setMaterialUseAlpha(hMat, 1);
    setMaterialUseLight(hMat, 0); // 라이팅 적용 안함.

    /// 
    setBillboard(m_DigitNode[i].m_hAnimatable, 1);
    controlAnimatable(m_DigitNode[i].m_hAnimatable, 0);

    setRepeatCount(m_DigitNode[i].m_hAnimatable, 1);

    linkNode(hDummy, hAnimatable);

    //::insertToScene( hDummy );
    //----------------------------------------------------------------------------------------------------	
    /// @brief CHILDÀÇInsertToSceneÁ¦°Å
    //----------------------------------------------------------------------------------------------------
    //::insertToScene( hAnimatable );
  }

  /// Load digit number
  m_DigitTex[DIGIT_NORMAL] = loadTextureWithPool("3DData\\Effect\\Special\\DigitNumber01.dds",
    "3DData\\Effect\\Special\\DigitNumber01.dds",
    1,
    0,
    2);
  if (m_DigitTex[DIGIT_NORMAL] == NULL) {
    g_pCApp->ErrorBOX("Normal Digit Texture load failed !!", "File open error");
    //½ÇÆÐÇÑ ÀÌÀ¯¸¦ Àû¾îÁØ´Ù..
    ClearAll();
    return false;
  }

  /// Load digit number
  m_DigitTex[DIGIT_ATTACKED] = loadTextureWithPool("3DData\\Effect\\Special\\DigitNumber02.dds",
    "3DData\\Effect\\Special\\DigitNumber02.dds",
    1,
    0,
    2);
  if (m_DigitTex[DIGIT_ATTACKED] == NULL) {
    g_pCApp->ErrorBOX("Attacked Digit Texture load failed !!", "File open error");
    //½ÇÆÐÇÑ ÀÌÀ¯¸¦ Àû¾îÁØ´Ù..
    ClearAll();
    return false;
  }

  /// Load digit clear number
  m_DigitClearTex = loadTextureWithPool("3DData\\Effect\\Special\\DigitWorkClear.dds",
    "3DData\\Effect\\Special\\DigitWorkClear.dds",
    1,
    0,
    2);
  if (m_DigitClearTex == NULL) {
    g_pCApp->ErrorBOX("Clear Digit Texture load failed !!", "File open error");
    //½ÇÆÐÇÑ ÀÌÀ¯¸¦ Àû¾îÁØ´Ù..
    ClearAll();
    return false;
  }

  /// Load digit Miss number
  m_DigitMissTex = loadTextureWithPool("3DData\\Effect\\Special\\DigitNumberMiss.dds",
    "3DData\\Effect\\Special\\DigitNumberMiss.dds",
    1,
    0,
    2);
  if (m_DigitMissTex == NULL) {
    g_pCApp->ErrorBOX("Miss Digit Texture load failed !!", "File open error");
    //½ÇÆÐÇÑ ÀÌÀ¯¸¦ Àû¾îÁØ´Ù..
    ClearAll();
    return false;
  }

  return true;
}

void    CDigitEffect::ClearAll() {
  HNODE hVisible = NULL;
  HNODE hMesh = NULL;
  HNODE hMat = NULL;

  for (int i = 0; i < MAX_DIGIT_EFFECT; i++) {
    if (m_DigitNode[i].m_hPositionDummy != NULL) {
      removeFromScene(m_DigitNode[i].m_hPositionDummy);
      unloadVisible(m_DigitNode[i].m_hPositionDummy);
      m_DigitNode[i].m_hPositionDummy = NULL;
    }

    if (m_DigitNode[i].m_hAnimatable != NULL) {
      //----------------------------------------------------------------------------------------------------	
      /// @brief CHILDÀÇRemoveToSceneÁ¦°Å
      //----------------------------------------------------------------------------------------------------
      //::removeFromScene( m_DigitNode[ i ].m_hAnimatable );
      unloadAnimatable(m_DigitNode[i].m_hAnimatable);
      m_DigitNode[i].m_hAnimatable = NULL;
    }

    if (m_DigitNode[i].m_hMesh != NULL) {
      unloadMesh(m_DigitNode[i].m_hMesh);
      m_DigitNode[i].m_hMesh = NULL;
    }

    if (m_DigitNode[i].m_hMat != NULL) {
      unloadMaterial(m_DigitNode[i].m_hMat);
      m_DigitNode[i].m_hMat = NULL;
    }

  }

  if (m_hMotion != NULL) {
    unloadMotion(m_hMotion);
    m_hMotion = NULL;
  }

  if (m_DigitTex[DIGIT_NORMAL] != NULL) {
    unloadTexture(m_DigitTex[DIGIT_NORMAL]);
    m_DigitTex[DIGIT_NORMAL] = NULL;
  }
  if (m_DigitTex[DIGIT_ATTACKED] != NULL) {
    unloadTexture(m_DigitTex[DIGIT_ATTACKED]);
    m_DigitTex[DIGIT_ATTACKED] = NULL;
  }

  if (m_DigitClearTex != NULL) {
    unloadTexture(m_DigitClearTex);
    m_DigitClearTex = NULL;
  }

  if (m_DigitMissTex != NULL) {
    unloadTexture(m_DigitMissTex);
    m_DigitMissTex = NULL;
  }
}

/// Animation 이 끝난놈들은 안보이게 한다.
void        CDigitEffect::Refresh() {
  for (int i = 0; i < MAX_DIGIT_EFFECT; i++) {
    if ((m_DigitNode[i].m_hAnimatable != NULL) && m_DigitNode[i].m_bVisible) {
      const short int iState = ::getAnimatableState(m_DigitNode[i].m_hAnimatable);

      if (iState == 0) {
        //----------------------------------------------------------------------------------------------------	
        /// @brief Visibility ·Î Á¶Á¤ÇÏ´Â°Ô ¾Æ´Ï¶ó ¾Æ¿¹ Scene¿¡¼­ »«´Ù.
        //----------------------------------------------------------------------------------------------------
        ::removeFromScene(m_DigitNode[i].m_hPositionDummy);
        m_DigitNode[i].m_bVisible = false;
      }
    }
  }
}

/// ÇöÀç »ç¿ëÁßÀÌÁö ¾ÊÀº ³ëµå¸¦ ±¸ÇÑ´Ù.
int         CDigitEffect::GetEmptyNode() {
  for (int i = 0; i < MAX_DIGIT_EFFECT; i++) {
    if ((m_DigitNode[i].m_bVisible == false) && (m_DigitNode[i].m_hAnimatable != NULL)) {
      const short int iState = ::getAnimatableState(m_DigitNode[i].m_hAnimatable);

      if (iState == 0) {
        return i;
      }
    }
  }

  return INVALID_EFFECTNODE;
}

#define __DIGITEFFECT8
#ifdef __DIGITEFFECT8

void  CDigitEffect::CreateDigitEffect(const int iPoint, const float x, const float y, const float z, const bool bIsUSER) {
  int iEmptyNode = GetEmptyNode();

  if (iEmptyNode == INVALID_EFFECTNODE)
    return;

  /// °¢ ÀÚ¸´¼ö¸¦ ±¸ÇÑ´Ù..
  int iDigit[8];
  int iNumber = iPoint % 100000000;
  iDigit[0] = iNumber / 10000000;
  iDigit[1] = (iNumber % 10000000) / 1000000;
  iDigit[2] = (iNumber % 1000000) / 100000;
  iDigit[3] = (iNumber % 100000) / 10000;
  iDigit[4] = (iNumber % 10000) / 1000;
  iDigit[5] = (iNumber % 1000) / 100;
  iDigit[6] = (iNumber % 100) / 10;
  iDigit[7] = (iNumber % 10);

  /// 몇개의 숫자를 표시해야하는가?
  int iDigitCount = DIGIT_COUNT;
  for (int i = 0; i < DIGIT_COUNT; i++) {
    if (iDigit[i] == 0)
      iDigitCount--;
    else
      break;
  }

  LPDIRECT3DSURFACE9 psurfWork = nullptr;
  LPDIRECT3DSURFACE9 psurfDigit = nullptr;
  LPDIRECT3DSURFACE9 psurfMiss = nullptr;
  LPDIRECT3DSURFACE9 psurfClear = nullptr;

  std::function<void()> cleanupMem = [&]()
  {
    SAFE_RELEASE(psurfWork);
    SAFE_RELEASE(psurfDigit);
    SAFE_RELEASE(psurfMiss);
    SAFE_RELEASE(psurfClear);
  };

  auto work_texture = reinterpret_cast<LPD3DTEXTURE>(getTexture(m_DigitNode[iEmptyNode].m_hMat, 0));
  auto clear_texture = reinterpret_cast<LPD3DTEXTURE>(::getTexturePointer(m_DigitClearTex));

  /// 출력할 숫자가 없다..
  if (iDigitCount == 0) {
    int          iMissWidth = 0, iMissHeight = 0;
    auto miss_texture = reinterpret_cast<LPD3DTEXTURE>(getTexturePointer(m_DigitMissTex));

    if (!work_texture || !miss_texture || !clear_texture) return; // ÅØ½ºÃÄ°¡ ¾ÆÁ÷ ·ÎµùµÇÁö ¾ÊÀº »óÅÂ¶ó¸é °Ç³Ê¶Ü.

    HRESULT hr = S_OK;

    /// ÀÛ¾÷ ÅØ½ºÃÄ..
    hr = work_texture->GetSurfaceLevel(0, &psurfWork);
    if (FAILED(hr))
    {
      cleanupMem();
      return;
    }

    /// Miss ÅØ½ºÃÄ..
    hr = miss_texture->GetSurfaceLevel(0, &psurfMiss);
    if (FAILED(hr))
    {
      cleanupMem();
      return;
    }

    hr = clear_texture->GetSurfaceLevel(0, &psurfClear);
    if (FAILED(hr))
    {
      cleanupMem();
      return;
    }

    RECT        SourceRect = { 0, 0, 0, 0 };
    auto pDevice = reinterpret_cast<LPD3DDEVICE>(::getDevice());
    POINT       DestinationPoint = { 0, 0 };

    int clear_width = 0, clear_height = 0;
    ::getTextureSize(m_DigitClearTex, clear_width, clear_height);
    SourceRect.right = clear_width;
    SourceRect.bottom = clear_height;

    hr = pDevice->UpdateSurface(psurfClear,
      &SourceRect,
      psurfWork,
      &DestinationPoint);
    if (FAILED(hr))
    {
      cleanupMem();
      return;
    }

    ::getTextureSize(m_DigitMissTex, iMissWidth, iMissHeight);
    SourceRect.right = iMissWidth;
    SourceRect.bottom = iMissHeight;

    hr = pDevice->UpdateSurface(psurfMiss,
      &SourceRect,
      psurfWork,
      &DestinationPoint);

    if (FAILED(hr))
    {
      cleanupMem();
      return;
    }

    ::setPosition(m_DigitNode[iEmptyNode].m_hPositionDummy, x, y, z);
    ::controlAnimatable(m_DigitNode[iEmptyNode].m_hAnimatable, 1);

    //----------------------------------------------------------------------------------------------------	
    /// @brief Visibility ·Î Á¶Á¤ÇÏ´Â°Ô ¾Æ´Ï¶ó ¾Æ¿¹ Scene¿¡¼­ ³Ö°Ô »©±â...
    //----------------------------------------------------------------------------------------------------
    ::insertToScene(m_DigitNode[iEmptyNode].m_hPositionDummy);
    m_DigitNode[iEmptyNode].m_bVisible = true;
    ///::setVisibility( m_DigitNode[ iEmptyNode ].m_hAnimatable, 1 );
  }
  else {
    if (m_DigitNode[iEmptyNode].m_hMat == NULL)
      return;

    /// ³»°¡ ¸÷À» ¶§·ÈÀ»¶§¶û, ¾Æ¹ÙÅ¸°¡ ¸÷À» ¶§¸°°Å¶ûÀº Æ²¸®´Ù »öÀÌ.
    LPD3DTEXTURE digit_texture;
    if (bIsUSER)
      digit_texture = reinterpret_cast<LPD3DTEXTURE>(getTexturePointer(m_DigitTex[DIGIT_ATTACKED]));
    else
      digit_texture = reinterpret_cast<LPD3DTEXTURE>(getTexturePointer(m_DigitTex[DIGIT_NORMAL]));

    if (!work_texture || !digit_texture || !clear_texture) return; // ÅØ½ºÃÄ°¡ ¾ÆÁ÷ ·ÎµùµÇÁö ¾ÊÀº »óÅÂ¶ó¸é °Ç³Ê¶Ü.

    HRESULT hr = S_OK;

    /// ÀÛ¾÷ ÅØ½ºÃÄ..
    hr = work_texture->GetSurfaceLevel(0, &psurfWork);
    if (FAILED(hr))
    {
      cleanupMem();
      return;
    }

    /// ¼ýÀÚ ÅØ½ºÃÄ..
    hr = digit_texture->GetSurfaceLevel(0, &psurfDigit);
    if (FAILED(hr))
    {
      cleanupMem();
      return;
    }

    /// Clear ÅØ½ºÃÄ..
    hr = clear_texture->GetSurfaceLevel(0, &psurfClear);
    if (FAILED(hr))
    {
      cleanupMem();
      return;
    }

    RECT  SourceRect = { 0, 0, 0, 0 };
    POINT DestinationPoint = { 0, 0 };

    int clear_width = 0, clear_height = 0;
    ::getTextureSize(m_DigitClearTex, clear_width, clear_height);
    SourceRect.right = clear_width;
    SourceRect.bottom = clear_height;

    LPD3DDEVICE pDevice = reinterpret_cast<LPD3DDEVICE>(::getDevice());
    hr = pDevice->UpdateSurface(psurfClear,
      &SourceRect,
      psurfWork,
      &DestinationPoint);

    if (FAILED(hr))
    {
      cleanupMem();
      return;
    }

    /// ¼¾Å¸·Î À§Ä¡
    int iStartPos = ((DIGIT_COUNT - iDigitCount) * DIGIT_WIDTH) / 2;

    for (int i = 0; i < iDigitCount; i++) {
      SetRect(&SourceRect, (DIGIT_WIDTH * iDigit[DIGIT_COUNT - iDigitCount + i]), 0, (DIGIT_WIDTH * iDigit[DIGIT_COUNT - iDigitCount + i]) + DIGIT_WIDTH, DIGIT_HEIGHT);
      DestinationPoint.x = iStartPos + (i - 1) * DIGIT_WIDTH;
      DestinationPoint.y = 0;

      hr = pDevice->UpdateSurface(psurfDigit,
        &SourceRect,
        psurfWork,
        &DestinationPoint);

      if (FAILED(hr))
      {
        cleanupMem();
        return;
      }
    }

    setPosition(m_DigitNode[iEmptyNode].m_hPositionDummy, x, y, z);
    controlAnimatable(m_DigitNode[iEmptyNode].m_hAnimatable, 1);

    //----------------------------------------------------------------------------------------------------	
    /// @brief Visibility ·Î Á¶Á¤ÇÏ´Â°Ô ¾Æ´Ï¶ó ¾Æ¿¹ Scene¿¡¼­ ³Ö°Ô »©±â...
    //----------------------------------------------------------------------------------------------------
    insertToScene(m_DigitNode[iEmptyNode].m_hPositionDummy);
    m_DigitNode[iEmptyNode].m_bVisible = true;
    ///::setVisibility( m_DigitNode[ iEmptyNode ].m_hAnimatable, 1 );
  }

  cleanupMem();
}

#else

/// ÃÖ´ë 4ÀÚ¸® Å¸°ÝÄ¡¸¦ Ç¥ÇöÇÑ´Ù.. ( 32 * 32 * 4 )
void CDigitEffect::CreateDigitEffect(DWORD iPoint, float x, float y, float z, bool bIsUSER)
{
  int iEmptyNode = GetEmptyNode();

  if (iEmptyNode == INVALID_EFFECTNODE)
    return;

  int iDigit[8];
  iPoint = iPoint % 100000000;
  iDigit[0] = iPoint / 10000000;
  iDigit[1] = (iPoint % 10000000) / 1000000;
  iDigit[2] = (iPoint % 1000000) / 100000;
  iDigit[3] = (iPoint % 100000) / 10000;
  iDigit[4] = (iPoint % 10000) / 1000;
  iDigit[5] = (iPoint % 1000) / 100;
  iDigit[6] = (iPoint % 100) / 10;
  iDigit[7] = (iPoint % 10);

  /// °¢ ÀÚ¸´¼ö¸¦ ±¸ÇÑ´Ù..

  /// ¸î°³ÀÇ ¼ýÀÚ¸¦ Ç¥½ÃÇØ¾ßÇÏ´Â°¡?
  int iDigitCount = DIGIT_COUNT;
  for (int i = 0; i < DIGIT_COUNT; i++)
  {
    if (iDigit[i] == 0)
      iDigitCount--;
    else
      break;
  }

  /// Ãâ·ÂÇÒ ¼ýÀÚ°¡ ¾ø´Ù..
  if (iDigitCount == 0)
  {
    LPD3DTEXTURE		workTexture = (LPD3DTEXTURE) ::getTexture(m_DigitNode[iEmptyNode].m_hMat, 0);
    LPD3DTEXTURE		MissTexture = (LPD3DTEXTURE) ::getTexturePointer(m_DigitMissTex);

    LPDIRECT3DSURFACE9 psurfWork = NULL;
    LPDIRECT3DSURFACE9 psurfMiss = NULL;

    if (!workTexture || !MissTexture) return; // ÅØ½ºÃÄ°¡ ¾ÆÁ÷ ·ÎµùµÇÁö ¾ÊÀº »óÅÂ¶ó¸é °Ç³Ê¶Ü.

    HRESULT hr = S_OK;

    /// ÀÛ¾÷ ÅØ½ºÃÄ..
    hr = workTexture->GetSurfaceLevel(0, &psurfWork);
    if (FAILED(hr))
      return;

    /// Miss ÅØ½ºÃÄ..
    hr = MissTexture->GetSurfaceLevel(0, &psurfMiss);
    if (FAILED(hr))
      return;

    RECT SourceRect = { 0, 0, DIGIT_WIDTH * DIGIT_COUNT, DIGIT_HEIGHT };
    POINT DestinationPoint = { 0, 0 };

    LPD3DDEVICE pDevice = (LPD3DDEVICE)::getDevice();
    hr = pDevice->UpdateSurface(psurfMiss,
      &SourceRect,
      psurfWork,
      &DestinationPoint);

    if (FAILED(hr))
      return;


    ::setPosition(m_DigitNode[iEmptyNode].m_hPositionDummy, x, y, z);
    ::controlAnimatable(m_DigitNode[iEmptyNode].m_hAnimatable, 1);

    //----------------------------------------------------------------------------------------------------	
/// @brief Visibility ·Î Á¶Á¤ÇÏ´Â°Ô ¾Æ´Ï¶ó ¾Æ¿¹ Scene¿¡¼­ ³Ö°Ô »©±â...
    //----------------------------------------------------------------------------------------------------
    ::insertToScene(m_DigitNode[iEmptyNode].m_hPositionDummy);
    m_DigitNode[iEmptyNode].m_bVisible = true;
    ///::setVisibility( m_DigitNode[ iEmptyNode ].m_hAnimatable, 1 );

    SAFE_RELEASE(psurfWork);
    SAFE_RELEASE(psurfMiss);
  }
  else
  {

    if (m_DigitNode[iEmptyNode].m_hMat == NULL)
      return;

    LPD3DTEXTURE		workTexture = (LPD3DTEXTURE) ::getTexture(m_DigitNode[iEmptyNode].m_hMat, 0);

    /// ³»°¡ ¸÷À» ¶§·ÈÀ»¶§¶û, ¾Æ¹ÙÅ¸°¡ ¸÷À» ¶§¸°°Å¶ûÀº Æ²¸®´Ù »öÀÌ.
    LPD3DTEXTURE		DigitTexture = NULL;
    if (bIsUSER)
      DigitTexture = (LPD3DTEXTURE) ::getTexturePointer(m_DigitTex[DIGIT_ATTACKED]);
    else
      DigitTexture = (LPD3DTEXTURE) ::getTexturePointer(m_DigitTex[DIGIT_NORMAL]);

    LPD3DTEXTURE		ClearTexture = (LPD3DTEXTURE) ::getTexturePointer(m_DigitClearTex);

    LPDIRECT3DSURFACE9 psurfWork = NULL;
    LPDIRECT3DSURFACE9 psurfDigit = NULL;
    LPDIRECT3DSURFACE9 psurfClear = NULL;

    if (!workTexture || !DigitTexture || !ClearTexture) return; // ÅØ½ºÃÄ°¡ ¾ÆÁ÷ ·ÎµùµÇÁö ¾ÊÀº »óÅÂ¶ó¸é °Ç³Ê¶Ü.

    HRESULT hr = S_OK;

    /// ÀÛ¾÷ ÅØ½ºÃÄ..
    hr = workTexture->GetSurfaceLevel(0, &psurfWork);
    if (FAILED(hr))
      return;

    /// ¼ýÀÚ ÅØ½ºÃÄ..
    hr = DigitTexture->GetSurfaceLevel(0, &psurfDigit);
    if (FAILED(hr))
      return;

    /// Clear ÅØ½ºÃÄ..
    hr = ClearTexture->GetSurfaceLevel(0, &psurfClear);
    if (FAILED(hr))
      return;


    RECT SourceRect = { 0, 0, DIGIT_WIDTH * DIGIT_COUNT, DIGIT_HEIGHT };
    POINT DestinationPoint = { 0, 0 };


    LPD3DDEVICE pDevice = (LPD3DDEVICE)::getDevice();
    hr = pDevice->UpdateSurface(psurfClear,
      &SourceRect,
      psurfWork,
      &DestinationPoint);

    /// ¼¾Å¸·Î À§Ä¡
    int iStartPos = ((DIGIT_COUNT - iDigitCount) * DIGIT_WIDTH) / 2;

    for (int i = 0; i < iDigitCount; i++)
    {
      SetRect(&SourceRect, (DIGIT_WIDTH * iDigit[DIGIT_COUNT - iDigitCount + i]), 0, (DIGIT_WIDTH * iDigit[DIGIT_COUNT - iDigitCount + i]) + DIGIT_WIDTH, DIGIT_HEIGHT);
      DestinationPoint.x = iStartPos + i * DIGIT_WIDTH;
      DestinationPoint.y = 0;

      hr = pDevice->UpdateSurface(psurfDigit,
        &SourceRect,
        psurfWork,
        &DestinationPoint);


      if (FAILED(hr))
      {
        return;
      }
    }

    ::setPosition(m_DigitNode[iEmptyNode].m_hPositionDummy, x, y, z);
    ::controlAnimatable(m_DigitNode[iEmptyNode].m_hAnimatable, 1);

    //----------------------------------------------------------------------------------------------------	
/// @brief Visibility ·Î Á¶Á¤ÇÏ´Â°Ô ¾Æ´Ï¶ó ¾Æ¿¹ Scene¿¡¼­ ³Ö°Ô »©±â...
    //----------------------------------------------------------------------------------------------------
    ::insertToScene(m_DigitNode[iEmptyNode].m_hPositionDummy);
    m_DigitNode[iEmptyNode].m_bVisible = true;
    ///::setVisibility( m_DigitNode[ iEmptyNode ].m_hAnimatable, 1 );

    SAFE_RELEASE(psurfWork);
    SAFE_RELEASE(psurfDigit);
    SAFE_RELEASE(psurfClear);
  }
}

#endif
