#ifndef _TUTORIAL_EVENT_UTILITY_
#define _TUTORIAL_EVENT_UTILITY_

#include "../Common/IO_STB.h"
#include "../Util/JSingleton.h"

//---------------------------------------------------------------------------------------
///
/// class CTutorialEventUtility
///
///	�̺�Ʈ ���࿡ �ʿ��� ��ƿ��Ƽ Ŭ�����Դϴ�.
///
//---------------------------------------------------------------------------------------

class CTutorialEventUtility {
private:
  STBDATA m_TblNotifyButtonEvent; /// �˸���ư �̺�Ʈ

  CTutorialEventUtility(void );
public:
  ~CTutorialEventUtility(void);

  CTutorialEventUtility(const CTutorialEventUtility&) = delete;
  CTutorialEventUtility& operator=(const CTutorialEventUtility &) = delete;
  CTutorialEventUtility(CTutorialEventUtility &&) = delete;
  CTutorialEventUtility & operator=(CTutorialEventUtility &&) = delete;

  static auto& GetSingleton(){
    static CTutorialEventUtility inst;
    return inst;
  }

  bool Init();
  void Release();

  /// �˸� ��ư �̺�Ʈ�� �����Ѵ�.
  bool CreateNotifyEventButton(int iEventIdx);
  /// EventButton.STB�� ������ �˸� ��ư �̺�Ʈ�� �����Ѵ�.
  bool ExecNotifyEvent(int iEventIdx);
};

#endif //_TUTORIAL_EVENT_UTILITY_
