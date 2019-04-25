#ifndef _COMMANDPT_
#define _COMMANDPT_

enum {
  COMMAND_COMMAND,
  /// Normal command
  EMOTION_COMMAND,
  SKILL_COMMAND,
  /// Skill command
  MAX_QUICK_COMMAND,
};

class CCommand;

extern CCommand* g_QuickCommandPool[ MAX_QUICK_COMMAND ];

//----------------------------------------------------------------------------------------------------
/// Class CCommand
/// @brief Abstraction class for command
//----------------------------------------------------------------------------------------------------

class CCommand {
protected:

  int m_iSlotNO;

public:
          CCommand() : m_iSlotNO( 0 ) {};
  virtual ~CCommand() {};

  /// 클래스 상태 초기화.
  void SetMember(int iSlotNO) {
    m_iSlotNO = iSlotNO;
  }

  virtual bool Execute() = 0 { *(int*)nullptr = 10; };
};

//----------------------------------------------------------------------------------------------------
/// class CBasicCommand
/// @brief Basic command class
//----------------------------------------------------------------------------------------------------

class CBasicCommand : public CCommand {
public:
          CBasicCommand();
  virtual ~CBasicCommand();

  bool Execute() override;
};

//----------------------------------------------------------------------------------------------------
/// class CEmotionCommand
/// @brief Emotion expression command
//----------------------------------------------------------------------------------------------------

class CEmotionCommand : public CCommand {
public:
          CEmotionCommand();
  virtual ~CEmotionCommand();

  bool Execute() override;
};

//----------------------------------------------------------------------------------------------------
/// class CInventoryCommand
/// @brief Inventory slot command class
//----------------------------------------------------------------------------------------------------

class CInventoryCommand : public CCommand {
public:
          CInventoryCommand();
  virtual ~CInventoryCommand();

  bool Execute() override;
};

//----------------------------------------------------------------------------------------------------
/// class CNormalCommand
/// @brief Normal command class
//----------------------------------------------------------------------------------------------------

class CNormalCommand : public CCommand {
public:
          CNormalCommand();
  virtual ~CNormalCommand();

  bool Execute() override;
};

//----------------------------------------------------------------------------------------------------
/// @param
/// @brief Skill command class
//----------------------------------------------------------------------------------------------------

class CSkillCommand : public CCommand {
public:
          CSkillCommand();
  virtual ~CSkillCommand();

  bool Execute() override;
};

#endif //_COMMANDPT_
