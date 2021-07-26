#ifndef _PREVENT_DUPLICATED_COMMAND_
#define _PREVENT_DUPLICATED_COMMAND_

#include "Util/JSingleton.h"
#include "ObjectCommand/ObjCommand.h"
#include < list >

struct SendedCommand {
  CObjCommand* pCommand;
  int          iElapsedTime;

  SendedCommand() {
    pCommand     = nullptr;
    iElapsedTime = 0;
  }
};

//------------------------------------------------------------------------------------
///
/// class CPreventDuplicatedCommand
/// ������ �ߺ��� ���( Ư�� ��ų��� ) ������ �����ϴ�.
///
//------------------------------------------------------------------------------------

class CPreventDuplicatedCommand {
private:
  std::list<CObjCommand*>   m_CommandPool[ OBJECT_COMMAND_MAX ];
  std::list<SendedCommand*> m_CommandStructPool;
  std::list<SendedCommand*> m_SendedCommandQueue;

  CPreventDuplicatedCommand(void );
public:
  ~CPreventDuplicatedCommand(void);

  CPreventDuplicatedCommand(const CPreventDuplicatedCommand&) = delete;
  CPreventDuplicatedCommand& operator=(const CPreventDuplicatedCommand &) = delete;
  CPreventDuplicatedCommand(CPreventDuplicatedCommand &&) = delete;
  CPreventDuplicatedCommand & operator=(CPreventDuplicatedCommand &&) = delete;

  static auto& GetSingleton(){
    static CPreventDuplicatedCommand inst;
    return inst;
  }

  void Init();
  void ClearCommandQueue();
  void ClearAll();

  void Proc();

  CObjCommand*   GetObjCommand(int iType);
  SendedCommand* GetSendedCommand();

  //----------------------------------------------------------------------------------------------------
  /// �ϴ� ��ų���� Ư�� �ð������� ���� ����� �Ұ����ϴ�..
  //----------------------------------------------------------------------------------------------------
  bool PushSelfSkillCommand(int            iSkillSlot);
  bool PushTargetSkillCommand(int          iServerTarget, int iSkillSlot);
  bool PushPositionSkillCommand(D3DVECTOR& PosTO, int         iSkillSlot);

  bool CanSendSelfSkillCommand(int            iSkillSlot);
  bool CanSendTargetSkillCommand(int          iServerTarget, int iSkillSlot);
  bool CanSendPositionSkillCommand(D3DVECTOR& PosTO, int         iSkillSlot);

};

#endif //_PREVENT_DUPLICATED_COMMAND_
