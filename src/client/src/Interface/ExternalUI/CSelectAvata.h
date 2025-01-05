#ifndef _SELECTAVATA_
#define _SELECTAVATA_

#include <srv_char_list_reply.h>

#include "CExternalUI.h"
#include <vector>
#include <string>
#include "tgamectrl/TDialog.h"

struct t_PACKET;

/**
* ĳ���� ����â���� ���Ǵ� �������̽�
* - CGameDataCreateAvatar�� ���� �κ��� ó���ϰ� �ִ�.
*
* @Warning		����ó���� ���� 4,5��° ������ ĳ������ ������ ����/�Ұ���������.
* @Todo			CGameDataCreateAvatar�� ���� �����ؼ� �������̽��� ����Ÿ�� Ȯ���� �����ϴ��� �ѱ���( CSelectAvata )�� ��ġ�°�?
* @Author		������
* @Date			2005/9/6
*/
class CSelectAvata : public CTDialog {
public:
  CExternalUIManager* m_pEUIManager;

public:
  CSelectAvata();
  ~CSelectAvata();

  void SetEUIObserver(CExternalUIManager* pObserver);

  UINT Process(UINT uiMsg, WPARAM wParam, LPARAM lParam) override;
  void Show() override;

  void SendSelectAvataReq();
  void RecvAvataList(t_PACKET* recvPacket);
  void RecvAvataList(RoseCommon::Packet::SrvCharListReply& packet);

  enum {
    IID_BTN_CREATE = 10,
    IID_BTN_DELETE = 11,
    IID_BTN_OK = 12,
    IID_BTN_CANCEL = 13
  };

  enum {
    MSG_TYPE_NORMAL,
    MSG_TYPE_DELETECHAR
  };

protected:
  void OnLButtonUp(unsigned iProcID);
  bool m_bWaitServerReply;

};

#endif //_SELECTAVATA_
