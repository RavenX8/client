#ifndef _COUNTRY_
#define _COUNTRY_

#include "Util/JSingleton.h"

//---------------------------------------------------------------------------------------
///
/// �� ������ ������ ����..
///
//---------------------------------------------------------------------------------------
class CCountry {
  CCountry(void );
public:
  ~CCountry(void);

  CCountry(const CCountry&) = delete;
  CCountry& operator=(const CCountry &) = delete;
  CCountry(CCountry &&) = delete;
  CCountry & operator=(CCountry &&) = delete;

  static auto& GetSingleton(){
    static CCountry inst;
    return inst;
  }

  bool CheckCountry();
  bool IsUseItemDelayNewVersion();
  bool IsApplyNewVersion();

  DWORD GetCountryCode();

  bool IsTAIWAN();
  bool IsROSE();
  bool IsPhilipplines();
  bool IsJapan();
  bool IsUSA();
  bool IsKorea();
  bool IsEU();

  bool IsCountry(int country);

  enum COUNTRY_TYPE {
    COUNTRY_KOR = 0,
    COUNTRY_IROSE = 1,
    COUNTRY_JPN = 2,
    COUNTRY_CHA = 3,
    COUNTRY_CHA_TW = 4,
    COUNTRY_PH = 5,
    COUNTRY_USA = 6,
    COUNTRY_EU = 7,
    COUNTRY_MAX,
  };

private:
  /*
  ���� ����� ���Ǵ� ���ڴ� Ŭ���̾�Ʈ�� Ʋ����
  LANGUAGE_KOR = 0,
  LANGUAGE_USA = 1,
  LANGUAGE_JPN = 2,
  LANGUAGE_CHA_TRADITIONAL = 3,
  LANGUAGE_CHA_SIMPLE = 4,
  �������� ȥ��.. ���Ŀ� ������.. ������ �����ڵ尡 �븸�� 4������ �Ǿ��ִ�.. �׷��� ��� ��Ʈ�� ���̺��� 3����..
  */

  bool  m_bIsThatCountry[ COUNTRY_MAX ] = {};
  DWORD m_dwCountryCode = 0;

};

bool IsTAIWAN();
//bool IsIROSE ();

#endif //_COUNTRY_
