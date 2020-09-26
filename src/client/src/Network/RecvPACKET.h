/*
	$Header: /Client/Network/RecvPACKET.h 102   05-10-18 3:10p Young $
*/
#ifndef	__CRECVPACKET_H
#define	__CRECVPACKET_H

//-------------------------------------------------------------------------------------------------

class CRecvPACKET {
protected:
  CStrVAR m_WSV_IP;
  WORD    m_wWSV_PORT;
  DWORD   m_dwWSV_ID;

  CStrVAR m_GSV_IP;
  WORD    m_wGSV_PORT;
  DWORD   m_dwGSV_IDs[2];

  virtual void DisconnectFromServer(short) = 0;

public :

  bool Recv_tag_ADD_CHAR(short    nCliObjIDX, tag_ADD_CHAR*    tagAddChar);
  void Recv_tag_ADJ_STATUS(short& nPacketOffset, tag_ADD_CHAR* tagAddChar);

          CRecvPACKET();
  virtual ~CRecvPACKET();

  void Recv_srv_ERROR(t_PACKET* packet);
  void Recv_gsv_GM_COMMAND(t_PACKET* packet);

  void Recv_gsv_SET_GLOBAL_VAR(t_PACKET* packet);
  void Recv_gsv_SET_GLOVAL_FLAG(t_PACKET* packet);

  void Recv_srv_ANNOUNCE_TEXT(t_PACKET* packet);
  void Recv_gsv_ANNOUNCE_CHAT(t_PACKET* packet);

  bool Recv_lsv_LOGIN_REPLY(t_PACKET* packet);

  void Recv_lsv_CHANNEL_LIST_REPLY(t_PACKET* packet);
  int  Recv_lsv_SELECT_SERVER(t_PACKET* packet);

  int  Recv_srv_JOIN_SERVER_REPLY(t_PACKET* packet);
  void Recv_gsv_INIT_DATA(t_PACKET* packet);

  void Recv_wsv_CHAR_LIST(t_PACKET* packet);
  void Recv_wsv_DELETE_CHAR(t_PACKET* packet);
  bool Recv_wsv_CREATE_CHAR(t_PACKET* packet);

  void Recv_wsv_MOVE_SERVER(t_PACKET* packet);

  void Recv_gsv_JOIN_ZONE(t_PACKET* packet);
  void Recv_gsv_SELECT_CHAR(t_PACKET* packet);

  void Recv_gsv_INVENTORY_DATA(t_PACKET* packet);
  void Recv_gsv_QUEST_DATA(t_PACKET* packet);

  void Recv_gsv_REVIVE_REPLY(t_PACKET* packet);
  void Recv_gsv_SET_VAR_REPLY(t_PACKET* packet);

  void Recv_gsv_CHEAT_CODE(t_PACKET* packet);

  void Recv_gsv_SET_MOTION(t_PACKET* packet);
  void Recv_gsv_TOGGLE(t_PACKET* packet);
  void Recv_gsv_CHAT(t_PACKET* packet);
  void Recv_gsv_WHISPER(t_PACKET* packet);
  void Recv_gsv_SHOUT(t_PACKET* packet);
  void Recv_gsv_PARTY_CHAT(t_PACKET* packet);

  void Recv_gsv_SET_NPC_SHOW(t_PACKET* packet);

  void Recv_gsv_NPC_CHAR(t_PACKET* packet);
  void Recv_gsv_MOB_CHAR(t_PACKET* packet);
  void Recv_gsv_AVT_CHAR(t_PACKET* packet);
  void Recv_gsv_SUB_OBJECT(t_PACKET* packet);

  void Recv_gsv_SET_WEIGHT_RATE(t_PACKET* packet);
  void Recv_gsv_ADJUST_POS(t_PACKET* packet);

  void Recv_gsv_STOP(t_PACKET* packet);
  void Recv_gsv_ATTACK(t_PACKET* packet);
  void Recv_gsv_ATTACK_START(t_PACKET* packet);

  void Recv_gsv_MOUSECMD(t_PACKET* packet);
  void Recv_gsv_MOVE(t_PACKET* packet);

  void Recv_gsv_DAMAGE(t_PACKET* packet);

  void Recv_gsv_CHANGE_NPC(t_PACKET* packet);

  void Recv_gsv_SETEXP(t_PACKET* packet);
  void Recv_gsv_LEVELUP(t_PACKET* packet);

  void Recv_gsv_HP_REPLY(t_PACKET* packet);

  void Recv_gsv_STORE_TRADE_REPLY(t_PACKET* packet);

  void Recv_gsv_P_STORE_MONEYnINV(t_PACKET* packet);
  void Recv_gsv_SET_MONEYnINV(t_PACKET* packet);
  void Recv_gsv_SET_INV_ONLY(t_PACKET* packet);
  void Recv_gsv_USE_ITEM(t_PACKET* packet);

  void Recv_gsv_CHANGE_SKIN(t_PACKET* packet);
  void Recv_gsv_EQUIP_ITEM(t_PACKET* packet);

  void Recv_gsv_ADD_FIELDITEM(t_PACKET* packet);
  void Recv_gsv_GET_FIELDITEM_REPLY(t_PACKET* packet);

  void Recv_gsv_TELEPORT_REPLY(t_PACKET* packet);

  void Recv_gsv_SET_HOTICON(t_PACKET* packet);

  void Recv_gsv_USE_BPOINT_REPLY(t_PACKET* packet);

  void Recv_gsv_SKILL_LEARN_REPLY(t_PACKET* packet);
  void Recv_gsv_SKILL_LEVELUP_REPLY(t_PACKET* packet);

  void Recv_gsv_SELF_SKILL(t_PACKET* packet);
  void Recv_gsv_TARGET_SKILL(t_PACKET* packet);
  void Recv_gsv_POSITION_SKILL(t_PACKET* packet);
  void Recv_gsv_SKILL_START(t_PACKET* packet);

  void Recv_gsv_SKILL_CANCEL(t_PACKET* packet);

  ///
  /// result of skill
  ///
  void Recv_gsv_EFFECT_OF_SKILL(t_PACKET* packet);
  void Recv_gsv_DAMAGE_OF_SKILL(t_PACKET* packet); // 7,17占쏙옙 타占쏙옙占쏙옙 占쏙옙占�.
  void Recv_gsv_RESULT_OF_SKILL(t_PACKET* packet);

  void Recv_gsv_CLEAR_STATUS(t_PACKET* packet);
  void Recv_gsv_SPEED_CHANGED(t_PACKET* packet);

  void Recv_gsv_TRADE_REQ(t_PACKET* packet);
  void Recv_gsv_TRADE_REPLY(t_PACKET* packet);

  void Recv_gsv_P_STORE_OPENED(t_PACKET* packet);
  void Recv_gsv_P_STORE_CLOSED(t_PACKET* packet);
  void Recv_gsv_P_STORE_LIST_REPLY(t_PACKET* packet);
  void Recv_gsv_P_STORE_RESULT(t_PACKET* packet);
  void Recv_gsv_QUEST_REPLY(t_PACKET* packet);

  ///트占쏙옙占싱듸옙 占쏙옙占쏙옙
  void Recv_gsv_TRADE_P2P(t_PACKET* packet);
  void Recv_gsv_TRADE_P2P_ITEM(t_PACKET* packet);

  ///占쏙옙티 占쏙옙占쏙옙
  void Recv_gsv_PARTY_REQ(t_PACKET* packet);
  void Recv_gsv_PARTY_REPLY(t_PACKET* packet);
  void Recv_gsv_PARTY_MEMBER(t_PACKET* packet);
  void Recv_gsv_PARTY_LEVnEXP(t_PACKET* packet);
  void Recv_gsv_PARTY_ITEM(t_PACKET* packet);
  void Recv_gsv_PARTY_RULE(t_PACKET* packet);
  void Recv_gsv_CHANGE_OBJIDX(t_PACKET* packet);
  ///占쏙옙占쏙옙占쏙옙占쏙옙
  void Recv_gsv_CREATE_ITEM_REPLY(t_PACKET* packet);
  ///占쏙옙크( 占쏙옙占쏙옙창占쏙옙) 占쏙옙占쏙옙
  void Recv_gsv_BANK_LIST_REPLY(t_PACKET* packet);
  void Recv_gsv_MOVE_ITEM(t_PACKET* packet);
  ///占쌀몌옙탄 占쏙옙占쏙옙
  void Recv_gsv_SET_BULLET(t_PACKET* packet);

  void Recv_gsv_SERVER_DATA(t_PACKET* packet);
  ///PAT 占쏙옙占쏙옙
  void Recv_gsv_ASSEMBLE_RIDE_ITEM(t_PACKET* packet);

  /// 占실시곤옙 占쏙옙占쏙옙占실댐옙 占싱븝옙트 占쏙옙( m_nEventSTATUS ) 占쏙옙占쏙옙
  void Recv_GSV_SET_EVENT_STATUS(t_PACKET* packet);

  void Recv_GSV_SET_ITEM_LIFE(t_PACKET* packet);

  void Recv_wsv_CHATROOM(t_PACKET* packet);
  void Recv_tag_MCMD_HEADER(t_PACKET* packet);

  void Recv_wsv_MESSENGER_CHAT(t_PACKET* packet);
  void Recv_wsv_MEMO(t_PACKET* packet);

  void Recv_wsv_CHATROOM_MSG(t_PACKET* packet);

  void Recv_gsv_USED_ITEM_TO_REPAIR(t_PACKET* packet);
  void Recv_gsv_REPAIRED_FROM_NPC(t_PACKET* packet);

  void Recv_gsv_APPRAISAL_REPLY(t_PACKET* packet);
  //----------------------------------------------------------------------------------------------------	
  ///
  /// @brief 占쏙옙占� 占쏙옙챨占쏙옙占�
  ///
  //----------------------------------------------------------------------------------------------------
  void Recv_gsv_CRAFT_ITEM_REPLY(t_PACKET* packet);
  void Recv_gsv_SET_MONEY_ONLY(t_PACKET* packet);
  void Recv_gsv_REWARD_MONEY(t_PACKET* packet);

  //----------------------------------------------------------------------------------------------------	
  ///
  /// @brief 占쏙옙占쏙옙트 占쏙옙占쏙옙占쏙옙占�
  ///
  //----------------------------------------------------------------------------------------------------
  void Recv_gsv_REWARD_ITEM(t_PACKET* packet);
  void Recv_gsv_REWARD_ADD_ABILITY(t_PACKET* packet);
  void Recv_gsv_REWARD_SET_ABILITY(t_PACKET* packet);

  void Recv_wsv_CHAR_CHANGE(t_PACKET* packet);

  //----------------------------------------------------------------------------------------------------	
  ///
  /// @brief 占싱븝옙트 占쏙옙占쏙옙占쏙옙트 처占쏙옙 占쏙옙占쏙옙
  ///
  //----------------------------------------------------------------------------------------------------	
  void Recv_gsv_ADD_EVENTOBJ(t_PACKET* packet);

  //----------------------------------------------------------------------------------------------------	
  ///
  /// @brief HP, MP 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙킷.
  ///
  //----------------------------------------------------------------------------------------------------	
  void Recv_gsv_SET_HPnMP(t_PACKET* packet);

  //----------------------------------------------------------------------------------------------------	
  ///
  /// @brief Do_DeadEvent 占쏙옙占쏙옙 호占쏙옙占싹댐옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙청占쏙옙 처占쏙옙占싹곤옙 占쏙옙占쏙옙..
  ///
  //----------------------------------------------------------------------------------------------------
  void Recv_gsv_CHECK_NPC_EVENT(t_PACKET* packet);

  void Recv_wsv_CLAN_COMMAND(t_PACKET* packet);
  void Recv_wsv_CLAN_CHAT(t_PACKET* packet);
  void Recv_wsv_CLANMARK_REPLY(t_PACKET* packet);

  void Recv_gsv_ALLIED_CHAT(t_PACKET* packet);
  void Recv_gsv_ALLIED_SHOUT(t_PACKET* packet);

  void Recv_gsv_LOGOUT_REPLY(t_PACKET* packet);
  void Recv_gsv_ITEM_RESULT_REPORT(t_PACKET* packet);

  void Recv_gsv_MALL_ITEM_REPLY(t_PACKET* packet);

  void Recv_gsv_BILLING_MESSAGE(t_PACKET* packet);

  void Recv_gsv_BILLING_MESSAGE_EXT(t_PACKET* packet);
  void Set_BILLING_MESSAGE_EXT_KOR_And_PH(t_PACKET* packet);
  void Set_BILLING_MESSAGE_EXT_JPN(t_PACKET* packet);
  void Set_BILLING_MESSAGE_EXT_TW(t_PACKET* packet);
  void Set_BILLING_MESSAGE_EXT_Other(t_PACKET* packet);

  void Recv_wsv_CLANMARK_REG_TIME(t_PACKET* packet);

  void Recv_gsv_MOVE_ZULY(t_PACKET* packet);

  //----------------------------------------------------------------------------------------------------	
  ///
  /// 占쏙옙 占쏙옙호    占쏙옙占쏙옙 占쏙옙환 占쏙옙킷처占쏙옙 
  ///
  //----------------------------------------------------------------------------------------------------	
  void Recv_gsv_GODDNESS_MODE(t_PACKET* packet);
  //----------------------------------------------------------------------------------------------------	
  /// 占쏙옙틀카트 占쏙옙占쏙옙
  void Recv_gsv_PATSTATE_CHANGE(t_PACKET* packet);

  // 占심몌옙占쏙옙 占쏙옙占쏙옙 占쏙옙환 (占쏙옙占쏙옙)
  void Recv_gsv_CHARSTATE_CHANGE(t_PACKET* packet);

  // 2占싸쏙옙 카트.
  void Recv_gsv_CART_RIDE(t_PACKET* packet);

  //占쏙옙占쏙옙占쏙옙占쏙옙..
  void Recv_gsv_SCREEN_SHOT_TIME(t_PACKET* packet);

  // Updates a player name
  void Recv_gsv_UPDATE_NAME(t_PACKET* packet);
};

//-------------------------------------------------------------------------------------------------
#endif
