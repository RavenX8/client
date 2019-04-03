#ifndef _CClanMemberItem_
#define _CClanMemberItem_
#include "tgamectrl/WinCtrl.h"
#include "../../CInfo.h"

/**
* Ŭ��â���� Ŭ���ɹ� List�� ���Ǵ� item class
*
* @Author		������
* @Date			2005/9/15
*/
class CClanMemberItem : public CWinCtrl {
public:
          CClanMemberItem(const char* pszName, int iClass, int iClanPoint, int iChannelNo, short nLevel, short nJob);
  virtual ~CClanMemberItem(void       );

  void     Draw() override;
  unsigned Process(unsigned uiMsg, WPARAM wParam, LPARAM lParam) override;
  void     SetSelected() override;
  void     Update(POINT ptMouse) override;

  void        SetLevel(short nLevel);
  void        SetJob(short   nJob);
  short       GetLevel();
  short       GetJob();
  void        SetClass(int iClass);
  int         GetClass();
  int         GetClanPoint();
  int         GetChannelNo();
  void        SetConnectStatus(int iChannelNo);
  const char* GetName();

  enum {
    ///CONNECT STATUS
    CS_OFFLINE,
    CS_ONLINE,
  };

protected:

  int   m_iOnlineMarkImageID;
  int   m_iOfflineMarkImageID;
  int   m_iClass;
  int   m_iClanPoint;
  int   m_iChannelNo;
  short m_nLevel;
  short m_nJob;

  std::string m_strJob;
  std::string m_strClass;
  std::string m_strName;
  std::string m_strChannel;

  CInfo m_Info;
};
#endif
