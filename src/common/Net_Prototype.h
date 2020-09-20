/*
  2004. 3. 22 �߰� ���� - NPC�� �ΰ����ɿ� ���ؼ� �ǽð� �����Ǵ� �̺�Ʈ ��( m_nEventSTATUS ) ����.
    . GSV_SET_EVENT_STATUS ��Ŷ �߰�.
    . gsv_NPC_CHAR ��Ŷ�� ��� ���� m_nEventSTATUS �߰�.
    �� NPC�� ���� �̺�Ʈ ����Ÿ���� m_nEventSTATUS ���� �����Ͽ� 
    ��Ȳ�� �´� ��ȭ ����� ����Ʈ�� �ο�...

  2004. 3. 24 �߰� ���� - ���忡�� �����Ǵ� �̺�Ʈ ������Ʈ ���� ����/����.
    . gsv_ADD_EVENTOBJ ��Ŷ.

  2004. 3. 30 �߰� ���� - NPC ��ų��� ��Ŷ�� ��ų ��� ��ȣ ����..
    . gsv_SELF_SKILL, gsv_TARGET_SKILL ��Ŷ�� cNpcSkillMOTION ���� �߰�
    . ����� ��ų�� ��� �̰��� ����, NPC ��ų�ϰ�� ��Ŷ ũ�Ⱑ +1 �Ǹ� cNpcSkillMOTION[0]�� ��� ��ȣ ��.

  2004. 3. 31 �߰� ���� - gsv_DAMAGE_OF_SKILL ��Ŷ �߰�
    . gsv_EFFECT_OF_SKILL ��Ŷ���� ��ų �������, ������ ���� �ϳ��� ���� ���޵Ǿ�����
    . gsv_DAMAGE_OF_SKILL ��Ŷ�� �������� ��ų ������ְ� ���� ��� �ִ� ��Ŷ..
    . ��ų Ÿ�� 7, 17���� ����� ���۵�.

  2004. 3. 31 ���� ����.
    . gsv_EFFECT_OF_SKILL, gsv_DAMAGE_OF_SKILL ���� ����.
    . ��ų ����� ��ü �߽ɿ���, ��ų�� ����� ��ü �߽�����...
    . ��ų�� ����� ��ü�� gsv_EFFECT_OF_SKILL, gsv_DAMAGE_OF_SKILL�� ��Ŷ�� ���� ����Ǹ�,
    . ��ų�� ����� ��ü�� ���� ��� ��Ŷ�� ���۵��� gsv_RESULT_OF_SKILL�� ���۵ȴ�.

  2004. 4. 1  �߰� ����. 
    . tag_ADD_CHAR ����ȣ �߰� CAI_OBJ::Is_ALLIED(�������ȣ)�� ȣ���Ͽ� true�� �Ʊ�, false�� ����.

  2004. 4. 2  ����/�߰� ����
    . tag_FIELDITEM���� ���� ������ ���� �ð�(m_wRemainTIME)�� ���ܽ��� tag_DROPITEM���� �̸� ����.
    . �Ϲ� �ʵ�������ϰ�� gsv_ADD_FIELDITEM�� m_wRemainTIME ��� ���������� ������ �ð� ����.
    . ������ ��Ŷ�� tag_DROPITEM���� ������ �������� ���� �ð��� ITEM_OBJ_LIVE_TIME���� ����.
    . gsv_DAMAGE_OF_SKILL�� ������ ��ӽ� tag_DROPITEM[0]�� �� ��. ������ �Ϲ� ������ �����۰� ����

  2004. 4. 8  �߰� ����
    . gsv_SKILL_LEARN_REPLY ��Ŷ�� ���� ������ RESULT_SKILL_LEARN_DELETE �߰�..
    . ������ RESULT_SKILL_LEARN_DELETE �ϰ�� CUserDATA::Skill_DELETE(slot,skill)�� ȣ���Ͽ� �����Ѵ�.

  2004. 4. 13 ����/�߰� ����
    . tag_ADD_CHAR ��� m_btMoveMODE�� �̵� ���°� �߰�..
    . �¿� ������ ��ȯ��:: cli_ASSEMBLE_RIDE_ITEM, gsv_ASSEMBLE_RIDE_ITEM ��Ŷ ���.

  2004. 4. 16 ���� ����.
    . wsv_CHAR_LIST��Ŷ�� tagBasicInfo��� tagCHARINFO�� ��ü, m_nPartItemIDX �߰�.
    . tag_ADD_CHAR ����ü�� ��ȯ�� �ɸ��� ���� ��� �߰� ::
      tag_ADD_CHAR.m_dwStatusFALG &	FLAG_ING_DEC_LIFE_TIME �ϰ�� owner_obj_idx = (uint16_t)pAdjSTATUS[ btIDX++ ];
      �� �����ϸ��.
      . CLI_APPRAISAL_REQ, GSV_APPRAISAL_REPLY ��Ŷ �߰�.

  2004. 4. 20 ���� ����.
    . gsv_MOVE ��Ŷ�� m_bRunMODE => m_btMoveMODE ( tag_ADD_CHAR::m_btMoveMODE�� ������ )
    . gsv_JOIN_ZONE��Ŷ�� m_dwWorldTIME�� m_dwAccWorldTIME�� ����... ���� ������ ���� ���� ���� ���� �ð�

  2004. 4. 28 ����/�߰� ����.
      . gsv_RESULT_OF_SKILL�� m_nResultCNT�� m_nSKillIDX�� ����.
    . gsv_PARTY_LEVnEXP�� m_btFlagLevelUP �߰�.
    . GSV_SKILL_START ��Ŷ �߰� : �������� ��ų �ɽ��� ���۽� ���۵ȴ�.

  2004. 4. 29 �߰� ����.
    . cli_SET_WEIGHT_RATE�߰�, Ŭ���̾�Ʈ���� ������ ������ üũ�ؼ� ���������/�ִ� ������ ������
    . 80%�̻� �ٱ� �ҵ�, 100%�̻� �̵� �Ҵ� �Ǵ��Ѵ�.
    . cli_SET_WEIGHT_RATE���� ��Ŷ�� ���������� �ֺ��� gsv_SET_WEIGHT_RATE��Ŷ���� �߰��.
    . �ƹ�Ÿ ���ν� ��Ŷ gsv_AVT_CHAR�� m_btWeightRate�� ���� ������� �ԷµǾ� ����. 
    . cli_JOIN_ZONE��Ŷ �ȿ� m_btWeightRate �߰�.. ������ ��û�� m_btWeightRate�� ���� ������ �־� ����;

  2004. 5. 1 ���� ����
    . gsv_TARGET_SKILL��Ŷ�� �ӵ������� ���� ��ų �����ü ��ǥ, Ÿ�� ��ǥ �߰�...
    . �߰� ���: m_wSrvDIST, m_PosTO - gsv_ATTACK ��Ŷ�� �ӵ� ������ �����ϰ� �����Ͽ� ���.
    . Ÿ�� ��ų�� ��� �տ��� ��ĩ�Ͽ� gsv_SKILL_START��Ŷ�� ��ٸ��� ���� �����ɵ�...
    . gsv_MOUSECMD, gsv_ATTACK����� m_PosCUR��� m_wSrvDIST�� ����.
    . cli_CANTMOVE, gsv_ADJUST_POS, cli_MOUSECMD, gsv_MOUSECMD, cli_JOIN_ZONE, gsv_AVT_CHAR Z�� �߰�.

  2004. 5. 11 ���� ����.
    . gsv_INVENTORY_DATA��Ŷ�� m_WishITEMS[ MAX_WISH_ITEMS ] �߰�, ��� �뵵�� ���� ���� ���½�...
    . ���� ���� �ŷ��� ��Ŷ cli_P_STORE_XXX, gsv_P_STORE_XXXX �߰�.
    . cli_SET_WISHITEM �߰� ������� ���� ���ϴ� �������� �����Ҽ� �ִ�(���ͳ� ���θ�ó��..)

  2004. 5. 13. �߰� ����.
    . cli_CHATROOM, wsv_CHATROOM, cli_CHATROOM_MSG, wsv_CHATROOM_MSG ��Ŷ �߰�
    . ���: ���弭������ �Ϲ� ä�� ����ó��, ���ӻ󿡼� ���� - ä�÷��� ������.

  2004. 5. 17. �߰� ����.
    . cli_MEMO, wsv_MEMO��Ŷ �߰� �ɸ��Ͱ� ���� ������/�ޱ� ���

  2004. 5. 18. �߰� ����.
    . CLI_MESSENGER, WSV_MESSENGER ��Ŷ �߰�..  ģ�� ���� ��Ŷ..
    . 1:1 ä�ýô� cli_MESSENGER_CHAT, wsv_MESSENGER_CHAT��Ŷ���� ��/���� ��.

  2004. 5. 31. �߰� ����
    . CLI_CHAR_CHANGE �߰�.. ���� �÷��̵��� �ɸ��� ����ȭ�� �̵� �̵��Ҷ� ���Ӽ����� ����..
    . ����� ���弭������ WSV_CHAR_CHANGE ��Ŷ�� ���۵�. ��Ŷ ������ t_PACKETHEADER ���

  2004. 6. 1.  �߰� ����.
    . tag_ADD_CHAR ����ü�� ��ȯ�� �ɸ��Ϳ� ���� ��ȯ ��ų ��ȣ �߰� ::
      ���� tag_ADD_CHAR.m_dwStatusFALG & FLAG_ING_DEC_LIFE_TIME �ϰ�� owner_obj_idx = (uint16_t)pAdjSTATUS[ btIDX++ ];
      ���� owner_obj_idx != 0 �̸� nUsedSummonSkillIDX = pAdjSTATUS[ btIDX ++ ]�� ��´�.
    . ��ȯ�� ���� Ư��ġ�� ���(MaxHP,���ݷ�,����...) �� ��ȯ�� ���� ��ų ��ȣ�� ������ �ִ�.

  2004. 6. 8.	 �߰� ����.
    . ������ ���� ��û,��� ��Ŷ ( �Ҹ𼺾����� ����, NPC�� ���� ������ ) �߰�
    . ������ ���� �����, ���� ��� ��Ŷ�� ������ gsv_SET_MONEYnINV, gsv_SET_INV_ONLY�� �����
    . ��Ŷ Ÿ�Ը� ����Ǿ� ���۵�
    . ������ ���� ���� ��Ŷ �߰� : GSV_SET_ITEM_LIFE => ������ 0���� ������ ��� UpdateAbility()�� ȣ������� ��

  2004. 6. 14. ���� ����.
    . gsv_TOGGLE, gsv_EQUIP_ITEM, gsv_ASSEMBLE_RIDE_ITEM ��Ŷ�� m_nRunSPEED[0]�� ����� �̵��ӵ� �߰�...
    . if ( pPacket->m_HEADER.m_nSize == ( sizeof( gsv_XXX ) + sizeof(int16_t) ) ) �̸� m_nRunSPEED[0] ������ �̵� �ӵ� ����.

  2004. 6. 16. �߰� ����.
    . GSV_P_STORE_MONEYnINV ��Ŷ Ÿ�� �߰�, ������ gsv_SET_MONEYnINV�� ����.
    . gsv_P_STORE_RESULT ����� RESULT_P_STORE_TRADE_PART �߰� �ŷ��� �Ϻ� ǰ���� ǰ���ǰų� ���� ��� ��������
    . �۰� ���Խ� ���۵�..

  2004. 6. 17. �߰� ����.
    . ��Ȱ�� �г�Ƽ ����ġ ������ �������� gsv_JOIN_ZONE ��Ŷ�� ���� ����ġ m_lCurEXP �߰�

  2004. 6. 28. �߰� ����.
    . CLI_CHANNEL_LIST_REQ ��Ŷ �߰�. srv_LOGIN_REPLY ��Ŷ ������ ���ϴ� ���弭���� ID�� �����ϸ�
    . ���� ������ ä�� ��������Ʈ�� ���۵�. ���弭���� ä���� ������ cli_SELECT_SERVER��Ŷ�� ä�� ��ȣ��
    . �߰��ؼ� ���۽��Ѿ���.
  2004. 7. 8. �߰� ����.
    . CLI_CRAFT_ITEM_REQ, GSV_CRAFT_ITEM_REPLY ��Ŷ �߰�... Ŭ���̾�Ʈ ��û�ÿ��� ������ �´� ��Ŷ��
    . �־� ����, ������ ���� ��Ŷ(GSV_CRAFT_ITEM_REPLY)�� ����, m_btRESULT�� ���� �������� ���ϰ�쿡��
    . m_btOutCNT�� ��ȿ�� ���� ����ְ�, m_btRESULT�� ���а����ϰ�� �����ϸ� �ȵ�(��ý��д¿���-��ް���,�Ҹ�� �����۵������).
  2004. 7. 10. �߰� ����.
    . wsv_CHAT_ROOM_JOIN ��Ŷ�� �ڽ��� m_wUserID�߰�, �������� CHAT_REPLY_ROOM_MAKED�����
    . ���̸� �տ� m_wUserID(�ڽ�) ����.
    . gsv_CRAFT_ITEM_REPLY��Ŷ�� m_btRESULT���� CRAFT_UPGRADE_SUCCESS, CRAFT_UPGRADE_FAILED	�ϰ��
    . m_sInvITEM[ m_btOutCNT-1 ].m_iQuantity�� ������ ���Ȱ��� �������(��ȹ������)
  2004. 7. 20. �߰� ����.
    . gsv_AVT_CHAR�� ( tag_ADD_CHAR.m_dwStatusFALG & FLAG_ING_STORE_MODE ) �ϰ�� char szStoreTITLE[] �ٿ� ����
    . GSV_SKILL_CANCEL ��Ŷ �߰�
  2004. 7. 26. �߰� ����.
    . gsv_PARTY_MEMBER�� ��Ƽ�� ���� �� �߰�.
    . cli_PARTY_ITEM, gsv_PARTY_ITEM ��Ŷ �߰�. ��Ƽ���� �����۽����� ���� �й��ϰ��, �����ڿ� ������ ������� ����
  2004. 8. 8 ��������.
    . MOB�� HP���� int16_t���� int32_t�� ����.
  2004. 8. 10 �߰� ����.
    . tagCHARINFO ����ü�� m_dwRemainSEC������� �߰�, ���� 0�� �ƴҰ�� �����ɶ����� ���� �ð�(�ʴ���).
    . cli_DELETE_CHAR��Ŷ�� m_bDelete ������� �߰�, ������ ��û�� true�� ���������, false�� ������� ����,
    . ���� ��Ŷ���� wsv_DELETE_CHAR ���۵�.
  2004. 8. 23 �߰� ����.
    . gsv_SETEXP ��Ŷ�� ���� ���׹̳� �� �߰�
  2004. 9. 16 �߰� ����.
    . ��Ƽ ��� ���� tag_PARTY_MEMBER�� m_nSTAMINA �߰�.
    . �߰��� ��Ƽ ���� ���׹̳ʰ� < 3000 �����ϰ�� ... gsv_CHANGE_OBJIDX ��Ŷ�� �߰� ���۵�
  2004. 10. 4 �߰� ����.
    . gsv_SET_HPnMP��Ŷ �߰� : ������ ���� hp, mp�� ���۵�
  2004. 10. 5 �߰� ����.
    . gsv_CHECK_NPC_EVENT��Ŷ �߰�, ���� NPC�׿����� Ŭ���̾�Ʈ���� NPC�̺�Ʈ�� üũ�� ������ ���� ��û�ϴ�����
    . ������ Ŭ���̾�Ʈ���� ������û => Ŭ���̾�Ʈ ������ �����Ϸ�Ǹ� => ������ ������û ������ �ٲ�.
    . �������� ���� �׿��� ����ڿ��� ���� ����Ʈ�������� �������� �������� ������...
  2004. 10. 15 ���� / �߰� ����.
    . tag_ADD_CHAR ����ü�� char m_cTeamNO�� int32_t m_iTeamNO�� ����
    . gsv_JOIN_ZONE ����ü�� �ڽ��� ����ȣ �߰� int32_t m_iTeamNO;
  2004. 10. 30 �߰� ����.
    . Ŭ�� ��Ŷ �߰� CLI_CLAN_COMMAND, WSV_CLAN_COMMAND
    . Ŭ������ ��Ŷ�� ��� ���� ������ ������.
  2004. 11. 16 �߰� / ����.
    . CLI_ALLIED_CHAT, CLI_ALLIED_SHOUT, GSV_ALLIED_CHAT, GSV_ALLIED_SHOUT �߰�
    . PvP ������ ���� ����ȣ���Ը� ä���� �����Ѵ�.
    . gsv_AVT_CHAR����ü�� m_dwSubFLAG�߰� DataType.h�� FLAG_SUB_xxx �÷��� ������
    . tag_MY_CLAN����ü���� m_btClanPOS(Ŭ���� ����)�� tag_CLAN_ID ����ü�� �̵�.
    . ������ ������ Ŭ�� ���ɿ� GCMD_MEMBER_JOBnLEV�߰� :: �ڽ��� ����/������ �ٸ� ������� �뺸( �α�����/��������/�����Ŀ� ���� )
  2004. 11. 23 ����.
    . tag_CLAN_INFO ����, tag_MY_CLAN ��� �߰�
  2004. 12. 1 �߰�
    . 10�� ����ؾ��Ұ�� GSV_LOGOUT_REPLY ����
  2004. 12. 16 �߰�
    . ����/���ý� ������ �뺸 ��Ŷ( cli_ITEM_RESULT_REPORT ) �߰�, cli_CREATE_ITEM_EXP_REQ ��Ŷ�� ����
  2004. 12. 21 �߰� 
    . gsv_EFFECT_OF_SKILL�� ��ų �������� ���� m_nINT�� �߰�.
  2005. 01. 05 �߰�
    . CLI_SELECT_SERVER��û�� ���� ��� RESULT_SELECT_SERVER_NEED_CHARGE �߰� (�������ڰ� ������ ���ӽõ��Ҷ�)
    . ���� ���� ��Ŷ �߰�. gsv_BILLING_MESSAGE
  2005. 01. 11 ����
    . gsv_CHANGE_SKIN����, gsv_SET_ABILITY��Ŷ�� �ڽ����׸� ���޵ǰ� ������ �뺸 �ʿ��� ���(�Ӹ���,��,����) gsv_CHANGE_SKIN�� ���۵�
    . tagCHARINFO �������� �ɸ��� ����Ʈ�� �÷�Ƽ���ɸ��� ���� �ʵ� �߰�
    . wsv_CREATE_CHAR�ɸ��� ���� ��Ŷ��.... �÷�Ƽ�� �ɸ����� �����Ȱ��� ����...
    . gsv_BANK_LIST_REPLY��Ŷ�� BANK_REPLY_PLATINUM �߰�...�÷�Ƽ�� â������Ʈ�� ������ ���۵�(�������� �����Ұ��)
  2005. 03. 23 �߰�
    . Ŭ�� ��ũ ��û ��Ŷ cli_CLANMARK_REQ, wsv_CLANMARK_REPLY �߰�
  2005. 06. 07 ����/�߰�
    . GSV_NPC_CHAR ��Ŷ�� m_nCharIdx�� -(����) �̸� ���� �����̴�.
    . gsv_SET_NPC_SHOW NPC�� ����/���� ���� ����.
  2005. 06. 16 �߰�
    . ���� gsv_BILLING_MESSAGE���� Ȯ��� ������ gsv_BILLING_MESSAGE_EXT ��Ŷ �߰�
    . defined FLAG_PAY_XXX ���°� PAY_FLAG_����_XXX �� �̸� ����
  2005. 06. 20 �߰�
    . ���� ��ȯ ���� ��Ŷ GSV_GODDNESS_MODE �߰�.
  2005. 06. 22 ����
    . cli_TELEPORT_REQ ��Ŷ�� ���ǵ����� �����ϱ� ���� ������ ���� ��ǥ Ŭ���̾�Ʈ�� ���� ��ǥ(m_PosCUR) �߰�
  2005. 06. 23 �߰�
    . ��Ƽ / 1:1 �ŷ��� ���ݾ����� ���� �߰�
    . ��ȯ�� ���� ��Ŷ �߰�.( CLI_SUMMON_CMD, GSV_SUMMON_CMD )
  2005. 08. 22 �߰�
    . CLI_CART_RIDE / GSV_CART_RIDE 2�ν�īƮ ��û/���� ��Ŷ �߰�.
*/
#ifndef __NET_PROTOTYPE_H
#define __NET_PROTOTYPE_H
#include <cstdint>
#ifdef	__SERVER
  #include "DataTYPE.h"
  #include "CUserDATA.h"
  #include "PacketHEADER.h"
  #include "classHASH.h"
#else
#include "Common/DataTYPE.h"
#include "Common/CUserDATA.h"
#include "Util/PacketHEADER.h"
#include "Util/classHASH.h"
#endif

#define	__APPLY_04_10_15_TEAMNO			// 04.10.15���� ����ȣ ���� ��Ŷ �����ҷ� ?
#define	__INC_PLATINUM				// ����ȭ���� ���뿩��..

#pragma warning( disable:4200 )
#pragma warning( disable:4201 )
#pragma pack (push, 1)

//-------------------------------------------------------------------------------------------------

#define	SRV_ERROR					0x0700

#define	SRV_ANNOUNCE_TEXT			0x0701
#define	GSV_ANNOUNCE_CHAT			0x0702

#define	CLI_CHANNEL_LIST_REQ		0x0704
#define	LSV_CHANNEL_LIST_REPLY		0x0704

#define	CLI_CHECK_AUTH				0x0705
#define	SRV_CHECK_AUTH				0x0705

// 0x0706

#define	CLI_LOGOUT_REQ				0x0707
#define GSV_LOGOUT_REPLY			0x0707

#define LSV_LOGIN_REPLY				0x0708
#define	GSV_LOGIN_REPLY				0x0709

#define	CLI_SELECT_SERVER			0x070a
#define	LSV_SELECT_SERVER			0x070a

/*
#define	CLI_JOIN_WORLD				0x070b
#define WSV_JOIN_WORLD				0x070c		// ���� ������ ���� ���� ���� ����...
*/

#define	CLI_JOIN_SERVER_REQ			0x070b		// ����/�� ������ ������ ��û�Ѵ�.
#define	SRV_JOIN_SERVER_REPLY		0x070c

#define	GSV_GM_COMMAND				0x070d		// GM ���ɾ�.

#define	GSV_SET_GLOBAL_VAR			0x070e		// ���� ���� ����.
#define	GSV_SET_GLOBAL_FLAG			0x070f		// ���� �÷��� ��...

#define	WSV_MOVE_SERVER				0x0711		// ������ �̵��ض�...

#define	CLI_CHAR_LIST				0x0712
#define	WSV_CHAR_LIST				0x0712

#define	CLI_CREATE_CHAR				0x0713
#define	WSV_CREATE_CHAR				0x0713

#define	CLI_DELETE_CHAR				0x0714
#define WSV_DELETE_CHAR				0x0714

#define	CLI_SELECT_CHAR				0x0715
#define	GSV_SELECT_CHAR				0x0715

#define	GSV_INVENTORY_DATA			0x0716
#define	GSV_SET_MONEYnINV			0x0717
#define	GSV_SET_INV_ONLY			0x0718

#define	GSV_SERVER_DATA				0x0719

#define	GSV_RELAY_REQ				0x071a
#define	CLI_RELAY_REPLY				0x071a

#define	GSV_QUEST_DATA				0x071b

#define	CLI_CHAR_CHANGE				0x071c		// �ɸ��� ����â���� �̵���û :: ���� ������ ��Ŷ �����ؾ���.
#define	WSV_CHAR_CHANGE				0x071c		// �� ��Ŷ ������ CLI_CHAR_LIST�� ������ ��û :: ���� �������� ���۵Ǿ���

#define GSV_SET_MONEY_ONLY			0x071d		// ���������� ���絷... 

#define	GSV_REWARD_MONEY			0x071e		// ���� ���� �Ǿ���... ������ gsv_SET_MONEY_ONLY
#define	GSV_REWARD_ITEM				0x071f		// ������ ���� �Ǿ���
#define	GSV_REWARD_ADD_ABILITY		0x0720		// �ɷ�ġ �߰�
#define	GSV_REWARD_SET_ABILITY		0x0721		// �ɷ�ġ ����

#define	CLI_LOGOUT_CANCEL			0x0722

#define	GSV_QUEST_ONLY				0x0723
#define	GSV_WISH_LIST				0x0724

#define	CLI_QUEST_REQ				0x0730
#define	GSV_QUEST_REPLY				0x0730

#define	GSV_CHECK_NPC_EVENT			0x0731

#define	GSV_CHEAT_CODE				0x0751

#define	CLI_JOIN_ZONE				0x0753
#define	GSV_JOIN_ZONE				0x0753

#define GSV_INIT_DATA				0x0754

#define	CLI_REVIVE_REQ				0x0755
#define	GSV_REVIVE_REPLY			0x0755

#define	CLI_SET_REVIVE_POS			0x0756

#define	CLI_SET_VAR_REQ				0x0757
#define	GSV_SET_VAR_REPLY			0x0757

#define CLI_CHAR_INFO_REQ			0x0761
#define	GSV_CHAR_INFO_REPLY			0x0761

#define	CLI_SET_WEIGHT_RATE			0x0762
#define	GSV_SET_WEIGHT_RATE			0x0762

#define	GSV_ADJUST_POS				0x0770

#define	CLI_CANTMOVE				0x0771
#define	GSV_CANTMOVE				0x0771

// #define	CLI_ATTACK_START			0x0772
// #define	GSV_ATTACK_START			0x0772
// #define	GSV_ATTACK_STOP				0x0773

#define	GSV_CHANGE_NPC				0x0774

#define	CLI_SUMMON_CMD				0x0775
#define	GSV_SUMMON_CMD				0x0775

// 0x77

#define	CLI_SET_MOTION				0x0781
#define	GSV_SET_MOTION				0x0781

#define	CLI_TOGGLE					0x0782
#define	GSV_TOGGLE					0x0782

#define	CLI_CHAT					0x0783
#define	GSV_CHAT					0x0783

#define	CLI_WHISPER					0x0784
#define	GSV_WHISPER					0x0784

#define	CLI_SHOUT					0x0785
#define	GSV_SHOUT					0x0785

#define	CLI_PARTY_CHAT				0x0786
#define GSV_PARTY_CHAT				0x0786

#define	CLI_CLAN_CHAT				0x0787
#define	WSV_CLAN_CHAT				0x0787

#define	CLI_ALLIED_CHAT				0x0788
#define GSV_ALLIED_CHAT				0x0788

#define	CLI_ALLIED_SHOUT			0x0789
#define	GSV_ALLIED_SHOUT			0x0789

#define	GSV_SET_EVENT_STATUS		0x0790

#define	GSV_NPC_CHAR				0x0791
#define	GSV_MOB_CHAR				0x0792
#define	GSV_AVT_CHAR				0x0793

#define GSV_SUB_OBJECT			    0x0794

#define CLI_SETPOS				    0x0795

#define	CLI_STOP					0x0796
#define	GSV_STOP					0x0796

#define GSV_MOVE					0x0797

#define	CLI_ATTACK					0x0798
#define GSV_ATTACK					0x0798

#define	CLI_DAMAGE					0x0799
#define GSV_DAMAGE					0x0799

#define	CLI_MOUSECMD				0x079a
#define	GSV_MOUSECMD				0x079a

#define	GSV_SETEXP					0x079b

#define GSV_LEVELUP					0x079e

#define	CLI_HP_REQ					0x079f
#define	GSV_HP_REPLY				0x079f

#define	GSV_SET_HPnMP				0x07a0

#define	CLI_STORE_TRADE_REQ			0x07a1
#define	GSV_STORE_TRADE_REPLY		0x07a1

//#define	CLI_CREATE_ITEM_EXP_REQ		0x07a2

#define	CLI_USE_ITEM				0x07a3
#define GSV_USE_ITEM				0x07a3

#define	CLI_DROP_ITEM				0x07a4

#define	CLI_EQUIP_ITEM				0x07a5
#define	GSV_EQUIP_ITEM				0x07a5

#define	GSV_ADD_FIELDITEM			0x07a6
#define	GSV_SUB_FIELDITEM			0x07a6

#define	CLI_GET_FIELDITEM_REQ		0x07a7
#define GSV_GET_FIELDITEM_REPLY		0x07a7

#define CLI_TELEPORT_REQ			0x07a8
#define GSV_TELEPORT_REPLY			0x07a8

#define	CLI_USE_BPOINT_REQ			0x07a9
#define GSV_USE_BPOINT_REPLY		0x07a9

#define	CLI_SET_HOTICON				0x07aa
#define	GSV_SET_HOTICON				0x07aa

#define	CLI_SET_BULLET				0x07ab
#define	GSV_SET_BULLET				0x07ab

//#define	CLI_CHANGE_SKIN				0x07ac
#define	GSV_CHANGE_SKIN				0x07ac

#define	CLI_BANK_LIST_REQ			0x07ad
#define	GSV_BANK_LIST_REPLY			0x07ad

#define	CLI_MOVE_ITEM				0x07ae
#define	GSV_MOVE_ITEM				0x07ae

#define	CLI_CREATE_ITEM_REQ			0x07af
#define	GSV_CREATE_ITEM_REPLY		0x07af

#define GSV_SKILL_LEARN_REPLY		0x07b0

#define	CLI_SKILL_LEVELUP_REQ		0x07b1
#define GSV_SKILL_LEVELUP_REPLY		0x07b1

#define	CLI_SELF_SKILL				0x07b2
#define	GSV_SELF_SKILL				0x07b2

#define	CLI_TARGET_SKILL			0x07b3
#define	GSV_TARGET_SKILL			0x07b3

#define	CLI_POSITION_SKILL			0x07b4
#define	GSV_POSITION_SKILL			0x07b4

#define	GSV_EFFECT_OF_SKILL			0x07b5
#define	GSV_DAMAGE_OF_SKILL			0x07b6

#define	GSV_CLEAR_STATUS			0x07b7
#define	GSV_SPEED_CHANGED			0x07b8

#define	GSV_RESULT_OF_SKILL			0x07b9

#define	CLI_APPRAISAL_REQ			0x07ba		// ������ ���� �Ƿ�~~~
#define	GSV_APPRAISAL_REPLY			0x07ba

#define	GSV_SKILL_START				0x07bb		// ��ų �ɽ��� ��ŸƮ...

#define	CLI_CRAFT_ITEM_REQ			0x07bc
#define	GSV_CRAFT_ITEM_REPLY		0x07bc

#define	GSV_SKILL_CANCEL			0x07bd		// �����ߴ� ��ų ���.
//	0x07be

#define	CLI_SET_WISHITEM			0x07bf

#define	CLI_TRADE_P2P				0x07c0		// ���ΰ� �ŷ�...
#define GSV_TRADE_P2P				0x07c0

#define	CLI_TRADE_P2P_ITEM			0x07c1		// �ŷ� ������ �߰� / ����.
#define	GSV_TRADE_P2P_ITEM			0x07c1

#define CLI_P_STORE_OPEN			0x07c2		// ���� ���� ���� ��û
#define GSV_P_STORE_OPENED			0x07c2

#define CLI_P_STORE_CLOSE			0x07c3		// ���� ���� ��� ��û
#define	GSV_P_STORE_CLOSED			0x07c3

#define CLI_P_STORE_LIST_REQ		0x07c4		// ���� ���� ���� ����Ʈ ��û
#define GSV_P_STORE_LIST_REPLY		0x07c4

#define CLI_P_STORE_BUY_REQ			0x07c5		// ���� ���� ���� ���� ��û
#define	CLI_P_STORE_SELL_REQ		0x07c6		// ���� ���� ���� �Ǹ� ��û

#define	GSV_P_STORE_RESULT			0x07c6
#define	GSV_P_STORE_MONEYnINV		0x07c7

#define	CLI_ASSEMBLE_RIDE_ITEM		0x07ca
#define	GSV_ASSEMBLE_RIDE_ITEM		0x07ca

#define	CLI_USE_ITEM_TO_REPAIR		0x07cb		// �Ҹ� ���� ���������� ������ ���� ��û
#define	GSV_USED_ITEM_TO_REPAIR		0x07cb		// gsv_SET_INV_ONLY

#define	CLI_REPAIR_FROM_NPC			0x07cd		// NPC�� ���� ������ ���� ��û
#define	GSV_REPAIRED_FROM_NPC		0x07cd		// gsv_SET_MONEYnINV

#define	GSV_SET_ITEM_LIFE			0x07ce		// �������� ������ �����

#define	CLI_PARTY_REQ				0x07d0
#define	GSV_PARTY_REQ				0x07d0

#define	CLI_PARTY_REPLY				0x07d1
#define	GSV_PARTY_REPLY				0x07d1

#define	GSV_PARTY_MEMBER			0x07d2		// ��Ƽ�� �߰�/����.
//#define	GSV_PARTY_INFO			0x07d3		// ��Ƽ ����
#define	GSV_PARTY_ITEM				0x07d3		// ���� �й�� ������ ���� ����

#define	GSV_PARTY_LEVnEXP			0x07d4

#define	GSV_CHANGE_OBJIDX			0x07d5

#define	GSV_ADD_EVENTOBJ			0x07d6
// #define	GSV_SUB_EVENTOBJ			0x07d6

#define	CLI_PARTY_RULE				0x07d7
#define	GSV_PARTY_RULE				0x07d7

#define	CLI_ITEM_RESULT_REPORT		0x07d8
#define	GSV_ITEM_RESULT_REPORT		0x07d8

#define	CLI_MALL_ITEM_REQ			0x07d9
#define	GSV_MALL_ITEM_REPLY			0x07d9

#define	CLI_MOVE_ZULY				0x07da
#define	GSV_MOVE_ZULY				0x07da

#define	GSV_SET_NPC_SHOW			0x07db

#define	GSV_GODDNESS_MODE			0x07dc

#define	CLI_CART_RIDE				0x07dd
#define	GSV_CART_RIDE				0x07dd

#define	GSV_BILLING_MESSAGE_EXT		0x07de
#define	GSV_BILLING_MESSAGE			0x07df

#define CLI_CLAN_COMMAND			0x07e0
#define	WSV_CLAN_COMMAND			0x07e0

#define	CLI_MESSENGER				0x07e1
#define	WSV_MESSENGER				0x07e1

#define	CLI_MESSENGER_CHAT			0x07e2
#define	WSV_MESSENGER_CHAT			0x07e2

#define	CLI_CHATROOM				0x07e3
#define	WSV_CHATROOM				0x07e3

#define	CLI_CHATROOM_MSG			0x07e4
#define	WSV_CHATROOM_MSG			0x07e4

#define	CLI_MEMO					0x07e5
#define	WSV_MEMO					0x07e5

#define	CLI_CLANMARK_SET			0x07e6

#define	CLI_CLANMARK_REQ			0x07e7
#define	WSV_CLANMARK_REPLY			0x07e7

#define CLI_CLANMARK_REG_TIME		0x07e8
#define	WSV_CLANMARK_REG_TIME		0x07e8

#define	GSV_PATSTATE_CHANGE			0x07e9

#define	GSV_CHARSTATE_CHANGE		0x07ea

#define CLI_SCREEN_SHOT_TIME		0x07eb                          //��ũ���� ������ �����ð���û..(���翵 �߰� 2005.10.18)
#define GSV_SCREEN_SHOT_TIME		0x07eb                          //��ũ���� �������ʿ��� ���� �ð�����.. .(���翵 �߰� 2005.10.18)

#define SRV_UPDATE_NAME				0x07ec
#define PXY_UPDATE_NAME				0x07ed
#define PXY_SET_RIGHTS				0x07ef

//-------------------------------------------------------------------------------------------------

struct cli_CHECK_AUTH : public t_PACKETHEADER {
  /*
    �� ��⺰ ����Ÿ...
    �ʸ����� ��� GG_AUTH_DATA ����ü
  */
};

#define	AUTH_MODULE_nPROTECT	0xf1

struct srv_CHECK_AUTH : public t_PACKETHEADER {
  uint8_t m_btModuleTYPE;
  /*
    �� ��⺰ ����Ÿ...
    �ʸ����� ��� GG_AUTH_DATA ����ü
  */
};

struct srv_ERROR : public t_PACKETHEADER {
  uint16_t m_wErrorCODE;
  // szErrorMSG[]
};

struct lsv_ERROR : public srv_ERROR {};

struct gsv_ERROR : public srv_ERROR {};

struct gsv_GM_COMMAND : public t_PACKETHEADER {
  uint8_t  m_btCMD;
  uint16_t m_wBlockTIME;
};

#define	GM_CMD_SHUT		0x00		//	ä�� ����..
#define	GM_CMD_LOGOUT	0x01		//	���� ���� ������...

struct srv_LOGIN_REPLY : public t_PACKETHEADER {
  uint8_t  m_btResult;
  uint16_t m_wRight;
  uint16_t m_wPayType;
  /*  m_cResult == RESULT_LOGIN_REPLY_OK �ϰ��
      struct {
          // szServerNAME[]
    uint32_t dwServerID;
      } ;
  */
};

#define RESULT_LOGIN_REPLY_OK                   0x00
#define	RESULT_LOGIN_REPLY_KOREA_OK				RESULT_LOGIN_REPLY_OK

#define RESULT_LOGIN_REPLY_FAILED               0x01    // ����
#define RESULT_LOGIN_REPLY_NOT_FOUND_ACCOUNT    0x02    // ���� ����.
#define RESULT_LOGIN_REPLY_INVALID_PASSWORD     0x03    // ��� ����
#define RESULT_LOGIN_REPLY_ALREADY_LOGGEDIN     0x04    // �̹� �α��� ���̴�
#define RESULT_LOGIN_REPLY_REFUSED_ACCOUNT      0x05    // �������� �źε� �����̴�.
#define RESULT_LOGIN_REPLY_NEED_CHARGE          0x06    // ������ �ʿ��Ѵ�
#define	RESULT_LOGIN_REPLY_NO_RIGHT_TO_CONNECT	0x07	// ���� ������ ����.
#define	RESULT_LOGIN_REPLY_TOO_MANY_USER		0x08	// ���� �ο� ����
#define	RESULT_LOGIN_REPLY_NO_REAL_NAME			0x09
#define	RESULT_LOGIN_REPLY_INVALID_VERSION		0x0a	// ��ġ�� �޾ƾ� �Ѵ�.
#define	RESULT_LOGIN_REPLY_OUT_OF_IP			0x0b	// �Ǿ��� ���� ���� ip �ʰ�...

#define	RESULT_LOGIN_REPLY_TAIWAN_OK			0x10	// m_wPayType ���� Ÿ�̿� ���ݼ��� ü��
#define	RESULT_LOGIN_REPLY_TAIWAN_FAILED		0x11	// Ÿ�̿� ���ݼ��� ü��� ����

#define	RESULT_LOGIN_REPLY_JAPAN_OK				0x18	// m_wPayType ���� �Ϻ� ���ݼ��� ü��

/*
�Ϻ� ���� ���� RESULT_LOGIN_REPLY_JAPAN_OK�ϰ�� m_wPayType�� ����
PAY_FLAG_JP_???? �� & �����ؼ� TRUE�̸� ������
��:: )
  if ( m_wPayType & PAY_FLAG_JP_EXTRA_CHAR ) {
    // �ѱ��� �÷�Ƽ�� ����ó�� 5���� �ɸ��� ��밡��.
  }
*/
/*
�븸 ���� ���� RESULT_LOGIN_REPLY_TWAIN_OK�ϰ�� m_wPayType�� ��
0  :	// ����
1	:	// �ð��� - �� 1 �ð� ������ 6 ����Ʈ �谨, 1�ð��� ä���� ������ ���, 1�ð����� ����Ѵ�.
2	:	// �� ������ - �� 30 �� ������ 350 ����Ʈ �谨.
3	:	// �ð� ������ - �� 12 �ð� ������ 20 ����Ʈ �谨.
4	:	// 3���� ������ - �� 90 �� ������ 888 ����Ʈ �谨.
5	:	// ȫ�� ��������
6	:	// ȫ�� 3���� ������
7	:	// ȫ�� �ð���
8	:	// ȫ�� �ð� ������
9	:	// �� ������ - �� 1�� ������ 0.25����Ʈ �谨.
10	:	// �ð� ������  - �� 2�ð� ������ 12 ����Ʈ �谨.
11	:	// 10�� - �� 10�� ������ 150 ����Ʈ �谨.
12	:	// ���� ü��
101:	// ���� 3��
102:	// ���� 7��
103:	// ���� 10��

�븸 ���� ���� RESULT_LOGIN_REPLY_TWAIN_FAILED�ϰ�� m_wPayType�� ��
2	:	// �ߺ� �α��� (login result)	�̹� �α��� �� ����
3	:	// �н����� ���� (login result)	���� �Ǵ� �н����� ���� �Ǵ� ����Ʈ �������� ���� ����,  Joypark ȸ�� �߽� ����.
4	:	// ObjnetObiect ���� ����, Login Server��� �Ұ��� (Connection State Result)
5	:	// Login Server���� �ߴ� (Connection State Result)
6	:	// Login Server���� ���� (Connection State Result)
7	:	// ���� ���� (login result)	���� �Ǵ� �н����� ���� �Ǵ� ����Ʈ�� �������� ���� ����,  Joypark ȸ�� �߽� ����. 
8	:	// ����Ʈ ���� (login result)	����Ʈ ����
9	:	// �ð� ���� (login result)	���� ü��Ⱓ ����
10	:	// ���� �������� (login result)	���ݹ�� ����
11	:	// ���� (login result)	���� ��
12	:	// ���ϣ��� (login result)	���� ��
13	:	// ���ϣ��� (login result)	���� ��
14	:	// ���� 10 �� (login result)	���� ��
15	:	// �α� �ƿ� ���� (logout result)
16	:	// ��Ȯ���� ���� (������ ���̽� ���� login, relogin, logout result)	��Ȯ���� ����
17	:	// Ip�ߺ� (Ȩ ���� ��� login, relogin, result) 
18	:	// ���ӹ� ��ȿ�� ���ӹ� ip����Ʈ (��� : ���ӹ� login, relogin, result)	
20	:	// ���� ���� (relogin result)	���� �Ǵ� �н����� ���� ���� ����Ʈ�� �������� ���� ����, Joypark ȸ�� �߽� ����.
21	:	// �ߺ� �α��� (relogin result)	�̹� �¶��� ��Ȳ�� ���� 
23	:	// ����Ʈ ���� (relogin result)	����Ʈ ����
24	:	// �ð� ���� (relogin result)	���� ü��Ⱓ ����
25	:	// ����Ȯ�� ���ݹ�� (relogin result)	���ݹ�� ���� 
26	:	// ���� (relogin result)	���� ��
27	:	// ���� ���� (relogin result)	���� ��
28	:	// ���� ���� (relogin result)	���� ��
29	:	// ���� 10 �� (relogin result)	���� ��
30	:	// ���� ip  n�ʳ� �ߺ� �α��� (login result)	���콺�� ��� ���� �ϰ�, �� �� �� �ٽ� ����.
31	:	// Game Server��� ���� (Connection State result)
32	:	// Game Server��� ID�ߺ�(Connection State result)
33	:	// Game Server�� ���(login, relogin result)	���� ���� �Ұ���, ����� �ٽ� ���� �õ�.
34	:	// ���� ���� ���� ������ �ð� �ʰ�.(login, relogin result)	���ӽð� �ʰ�. 
35	:	// ���� ü���� �� ���� (login result)	�� ������ ���� ü�� ���� �������� ����.
101:	// Library������ Login Server �� ���� ����. (Connection State result)
*/

struct cli_CHANNEL_LIST_REQ : public t_PACKETHEADER {
  uint32_t m_dwServerID;
};

struct tagCHANNEL_SRV {
  uint8_t m_btChannelNO;  // ������ ä�� ���� ��ȣ
  uint8_t m_btLowAGE;     // ���� ������ ���� ���� 0 �̸� ���� ���� ���� ����
  uint8_t m_btHighAGE;    // ���� ������ �ְ� ���� 0 �̸� �ְ� ���� ���� ����
  int16_t m_nUserPERCENT; // ���� ���ӵǾ� �ִ� ����ھ�...
  // char m_szChannenNAME[]	// m_szChannelsNAME == "" �̸� CHANNEL-ä�ι�ȣ �� ������
};

struct lsv_CHANNEL_LIST_REPLY : public t_PACKETHEADER {
  uint32_t m_dwServerID;
  uint8_t  m_btChannelCNT;
  /*
  {
    tagCHANNEL_SRV X m_btChannelCNT;
  }
  */
};

struct cli_SELECT_SERVER : public t_PACKETHEADER {
  uint32_t m_dwServerID;
  uint8_t  m_btChannelNO;
};

struct lsv_SELECT_SERVER : public t_PACKETHEADER {
  uint8_t  m_btResult;
  uint32_t m_dwIDs[2];
  // szServerIP[];
  // uint16_t wServerPortNO;
};

#define	RESULT_SELECT_SERVER_OK					0x00
#define RESULT_SELECT_SERVER_FAILED				0x01
#define	RESULT_SELECT_SERVER_FULL				0x02	// �ο� ����
#define	RESULT_SELECT_SERVER_INVALID_CHANNEL	0x03	// �߸��� ä�� ����
#define	RESULT_SELECT_SERVER_CHANNEL_NOT_ACTIVE	0x04	// ä�� ���� ������
#define	RESUTL_SELECT_SERVER_INVALID_AGE		0x05	// ������ ���� �ʴ´�..
#define	RESULT_SELECT_SERVER_NEED_CHARGE		0x06	// ������ �ȵǾ� �ִ�

struct tagVAR_GLOBAL {
  int16_t  m_nWorld_PRODUCT;               // ���� ����
  uint32_t m_dwUpdateTIME;                 // ���ŵ� �ð�.
  int16_t  m_nWorld_RATE;                  // ���� ���� :: ���蹰�� 80~140
  uint8_t  m_btTOWN_RATE;                  // ���� ����					80~140
  uint8_t  m_btItemRATE[ MAX_PRICE_TYPE ]; // �����ۺ� ����				1~127
  uint32_t m_dwGlobalFLAGS;
};

struct gsv_SET_GLOBAL_VAR : public t_PACKETHEADER, public tagVAR_GLOBAL {};

struct gsv_SET_GLOBAL_FLAG : public t_PACKETHEADER {
  uint32_t m_dwGlobalFLAGS;
};

#define	ZONE_FLAG_PK_ALLOWED		0x00000001

/*
struct cli_JOIN_WORLD : public t_PACKETHEADER {
  uint32_t	m_dwID;
    uint8_t	m_MD5Password[ 32 ];
} ;

// m_cResult ���� lsv_LOGIN_REPLY�� ����.
struct wsv_JOIN_WORLD : public t_PACKETHEADER {
  uint8_t	m_btResult;
  uint32_t	m_dwID;
} ;
#define	RESULT_JOIN_WORLD_OK					0x00
#define	RESULT_JOIN_WORLD_FAILED				0x01
#define RESULT_JOIN_WORLD_TIME_OUT				0x02    // �ð� �ʰ��� ���� ������ �ٽ� �α��� �ؾ��Ѵ�.
#define RESULT_JOIN_WORLD_INVALID_PASSWORD		0x03    // ��� ����
#define RESULT_JOIN_WORLD_ALREADY_LOGGEDIN		0x04    // �̹� �α��� ���̴�
*/

// ����/�� ������ ���� ��û
struct cli_JOIN_SERVER_REQ : public t_PACKETHEADER {
  uint32_t m_dwID;
  uint8_t  m_MD5Password[ 64 ];
};

struct srv_JOIN_SERVER_REPLY : public t_PACKETHEADER {
  uint8_t  m_btResult;
  uint32_t m_dwID;
  uint32_t m_dwPayFLAG;
};

#define	RESULT_JOIN_SERVER_OK					0x00
#define	RESULT_JOIN_SERVER_FAILED				0x01
#define RESULT_JOIN_SERVER_TIME_OUT				0x02    // �ð� �ʰ��� ���� ������ �ٽ� �α��� �ؾ��Ѵ�.
#define RESULT_JOIN_SERVER_INVALID_PASSWORD		0x03    // ��� ����
#define RESULT_JOIN_SERVER_ALREADY_LOGGEDIN		0x04    // �̹� �α��� ���̴�

// Ŭ���̾�Ʈ���� ������ �̵� �����϶�� �뺸...
struct wsv_MOVE_SERVER : public t_PACKETHEADER {
  uint16_t m_wPortNO;
  uint32_t m_dwIDs[2];
  // szServerIP[]
};

struct cli_CHAR_LIST : public t_PACKETHEADER {};

// ����, ����,����, ��Ʈ �ε���...
struct tagCHARINFO {
  uint8_t  m_btCharRACE;
  int16_t  m_nLEVEL;
  int16_t  m_nJOB;
  uint32_t m_dwRemainSEC; // 0�� �ƴҰ�� �����ɶ����� ���� �ð�(�ʴ���)
#ifdef	__INC_PLATINUM
  uint8_t m_btIsPlatinumCHAR; // 0�� �ƴѰ��� �÷�Ƽ���� ��츸 ���� ������ �ɸ��̴�.
#endif
};

struct wsv_CHAR_LIST : public t_PACKETHEADER {
  uint8_t m_btCharCNT;
  /*
  {
    tagCHARINFO m_CharINFO;
    tagPartITEM m_PartITEM    [ MAX_BODY_PART ];
    char		szCharName[]
  } x m_btCharCNT
  */
};

struct cli_CREATE_CHAR : public t_PACKETHEADER {
  uint8_t m_btCharRACE;
  int8_t  m_cBoneSTONE;
  int8_t  m_cHairIDX;
  int8_t  m_cFaceIDX;
  int8_t  m_cWeaponTYPE;
  int16_t m_nZoneNO;
  // char szCharName[]
};

struct wsv_CREATE_CHAR : public t_PACKETHEADER {
  uint8_t m_btResult;
  uint8_t m_btIsPlatinumCHAR;
};

#define	RESULT_CREATE_CHAR_OK				0x00
#define	RESULT_CREATE_CHAR_FAILED			0x01
#define	RESULT_CREATE_CHAR_DUP_NAME			0x02
#define	RESULT_CREATE_CHAR_INVALID_VALUE	0x03
#define	RESULT_CREATE_CHAR_NO_MORE_SLOT		0x04	// ���̻� ����� ����.
#define	RESULT_CREATE_CHAR_BLOCKED			0x05	// �������� �ɸ��� ������ �����Ǿ� �ִ� ���´�.

struct cli_DELETE_CHAR : public t_PACKETHEADER {
  uint8_t m_btCharNO;
  bool    m_bDelete; // true�� ����, false�� ������� ���� �ɸ��� ��Ȱ
  // char szCharName[]
};

struct wsv_DELETE_CHAR : public t_PACKETHEADER {
  uint32_t m_dwDelRemainTIME; // 0�̸� ���� ���...
  // char szCharName[]
};

struct cli_LOGOUT_REQ : public t_PACKETHEADER {};

struct gsv_LOGOUT_REPLY : public t_PACKETHEADER {
  uint16_t m_wWaitSec;
};

struct gsv_INIT_DATA : public t_PACKETHEADER {
  int32_t  m_iRandomSEED;
  uint16_t m_wRandomINDEX;
};

struct cli_SELECT_CHAR : public t_PACKETHEADER {
  uint8_t m_btCharNO;
  uint8_t m_btRunMODE;
  uint8_t m_btRideMODE;
  // char szCharName[]
};

struct gsv_SELECT_CHAR : public t_PACKETHEADER {
  uint8_t m_btCharRACE;
  int16_t m_nZoneNO;
  tPOINTF m_PosSTART;
  int16_t m_nReviveZoneNO;

  tagPartITEM m_PartITEM[ MAX_BODY_PART ];
  //	tagPartITEM			m_RideITEM[ MAX_RIDING_PART ];

  tagBasicINFO    m_BasicINFO;
  tagBasicAbility m_BasicAbility;
  tagGrowAbility  m_GrowAbility;
  tagSkillAbility m_Skill;
  CHotICONS       m_HotICONS;
  uint32_t        m_dwUniqueTAG;
  //	char				szCharName[];
};

struct gsv_INVENTORY_DATA : public t_PACKETHEADER {
  // 2004. 9.11 ��ü... CInventory	m_INV;
  struct {
    int64_t     m_i64Money;
    tagBaseITEM m_ItemLIST[ INVENTORY_TOTAL_SIZE ];
  }             m_INV;
};

// ����Ʈ ����ġ ���� ������ �Ʒ� 2���� ��Ŷ���� �и��� ������.
struct gsv_QUEST_DATA : public t_PACKETHEADER {
  tagQuestData m_Quests;
  tagWishLIST  m_WishLIST;
};

struct gsv_QUEST_ONLY : public t_PACKETHEADER {
  tagQuestData m_Quests;
};

struct gsv_WISH_LIST : public t_PACKETHEADER {
  tagWishLIST m_WishLIST;
};

struct cli_JOIN_ZONE : public t_PACKETHEADER {
  uint8_t m_btWeightRate;
  int16_t m_nPosZ;
};

struct gsv_JOIN_ZONE : public t_PACKETHEADER {
  uint16_t      m_wServerObjectIndex;
  int16_t       m_nCurHP;
  int16_t       m_nCurMP;
  int64_t       m_lCurEXP;
  int64_t       m_lPenalEXP;
  tagVAR_GLOBAL m_VAR;
  uint32_t      m_dwAccWorldTIME; // ���� ���� ó�� ������ ������ ���� Ÿ��
#ifdef	__APPLY_04_10_15_TEAMNO
  int32_t m_iTeamNO; // �������� ������ ����ȣ	:: 04.10.15 �߰�..
#endif

};

struct cli_SCREEN_SHOT_TIME : public t_PACKETHEADER {
  uint16_t wCnt;
};

struct gsv_SCREEN_SHOT_TIME : public t_PACKETHEADER {
  uint16_t wYear;
  uint8_t  btMonth;
  uint8_t  btDay;
  uint8_t  btHour;
  uint8_t  btMin;
};

struct cli_REVIVE_REQ : public t_PACKETHEADER {
  uint8_t m_btReviveTYPE;
};

#define	REVIVE_TYPE_REVIVE_POS		0x01
#define REVIVE_TYPE_SAVE_POS		0x02
#define	REVIVE_TYPE_START_POS		0x03
#define	REVIVE_TYPE_CURRENT_POS		0x04

struct gsv_REVIVE_REPLY : public t_PACKETHEADER {
  int16_t m_nZoneNO;
};

struct cli_SET_REVIVE_POS : public t_PACKETHEADER {};

struct cli_SET_VAR_REQ : public t_PACKETHEADER {
  uint8_t m_btVarTYPE;
  int32_t m_iValue;
};

struct gsv_SET_VAR_REPLY : public cli_SET_VAR_REQ {};

#define	REPLY_GSV_SET_VAR_FAIL_BIT	0x080
// #define	REPLY_GSV_SET_JOB_xxx

#define	BIT_MOTION_STOP_CMD			0x080000

struct cli_SET_MOTION : public t_PACKETHEADER {
  int16_t m_nMotionNO;

  union {
    uint16_t m_wValue;

    struct {
      uint16_t m_wETC : 15;
      uint16_t m_wIsSTOP : 1;
    };
  };
};

struct gsv_SET_MOTION : public cli_SET_MOTION {
  uint16_t m_wObjectIDX;
};

struct cli_TOGGLE : public t_PACKETHEADER {
  uint8_t m_btTYPE;
};

#define	TOGGLE_TYPE_RUN		0x00
#define	TOGGLE_TYPE_SIT		0x01
#define	TOGGLE_TYPE_DRIVE	0x02

struct gsv_TOGGLE : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  uint8_t  m_btTYPE;
  int16_t  m_nRunSPEED[0]; // �� Ÿ�� ��������...
};

struct cli_CHAT : public t_PACKETHEADER {
  // szChatMSG[]
};

struct gsv_CHAT : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
};

struct cli_WHISPER : public t_PACKETHEADER {
  // szTargetAccount[]
  // szChatMSG[]
};

struct gsv_WHISPER : public t_PACKETHEADER {
  // szFromAccount[]
  // szChatMSG[]
};

struct cli_SHOUT : public cli_CHAT {};

struct gsv_SHOUT : public t_PACKETHEADER {};

struct cli_ALLIED_CHAT : public t_PACKETHEADER {};

struct gsv_ALLIED_CHAT : public gsv_CHAT {
  int32_t m_iTeamNO;
  // szChatMSG[]
};

struct cli_ALLIED_SHOUT : public t_PACKETHEADER {};

struct gsv_ALLIED_SHOUT : public t_PACKETHEADER {
  int32_t m_iTeamNO;
  // szFromAccount[]
  // szChatMSG[]
};

struct cli_PARTY_CHAT : public cli_CHAT {};

struct gsv_PARTY_CHAT : public gsv_CHAT {};

struct cli_CLAN_CHAT : public cli_CHAT {};

struct wsv_CLAN_CHAT : public gsv_SHOUT {};

struct gsv_SET_EVENT_STATUS : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  int16_t  m_nEventSTATUS;
};

#define	MOVE_MODE_WALK		0x00	// �ȱ� ���
#define	MOVE_MODE_RUN		0x01	// �ٱ� ���
#define	MOVE_MODE_DRIVE		0x02	// ���� ��� : �ڽ��� Ÿ�� �ִ�.
#define	MOVE_MODE_RIDEON	0x04	// ���� ��� : ���ǰͿ� Ÿ�� �ִ�.

struct tag_ADD_CHAR : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  tPOINTF  m_PosCUR;
  tPOINTF  m_PosTO;
  uint16_t m_wCommand;
  uint16_t m_wTargetOBJ;
  uint8_t  m_btMoveMODE; // 0:�ȱ�, 1:�ٱ�, 2:�� �¿�����ۿ� Ÿ���ִ�, 3: m_wTargetOBJ�� �¿�����ۿ� Ÿ���ִ�.
  int32_t  m_iHP;
#ifdef	__APPLY_04_10_15_TEAMNO
  int32_t m_iTeamNO; //	char m_cTeamNO => int32_t�� ���� 04.10.15 ����...
#else
  int8_t		m_cTeamNO;
#endif
  uint32_t m_dwStatusFALG;
  /*
  ���� ��ų�� ���� �ٲ� ��...	2004.1.4
  if ( m_dwStatusFALG & ( ING_FLAG_MAX_HP | ING_FLAG_xxx_MOV_SPEED | ING_FLAG_xxx_ATK_SPEED ) ) {
    �� ����ü �ڿ� m_nAdjSTATUS[ xx ]�پ� ���۵�.

    uint8_t btIDX = 0;
      if ( tag_ADD_CHAR.m_dwStatusFALG &  FLAG_ING_MAX_HP ) )
      inc_max_hp = pAdjSTATUS[ btIDX++ ];

    if ( tag_ADD_CHAR.m_dwStatusFALG &	FLAG_ING_INC_MOV_SPEED ) )
      inc_run_spd = pAdjSTATUS[ btIDX++ ];

    if ( tag_ADD_CHAR.m_dwStatusFALG &	FLAG_ING_DEC_MOV_SPEED ) )
      dec_run_spd = pAdjSTATUS[ btIDX++ ];

    if ( tag_ADD_CHAR.m_dwStatusFALG &	FLAG_ING_INC_ATK_SPEED ) )
      inc_atk_spd = pAdjSTATUS[ btIDX++ ];

    if ( tag_ADD_CHAR.m_dwStatusFALG &	FLAG_ING_DEC_ATK_SPEED ) )
      dec_atk_spd = pAdjSTATUS[ btIDX++ ];

    if ( tag_ADD_CHAR.m_dwStatusFALG &	FLAG_ING_DEC_LIFE_TIME ) )		// 2004. 4. 16 �߰�..
      owner_obj_idx = (uint16_t) pAdjSTATUS[ btIDX++ ];
  }
  */
};

struct gsv_MOB_CHAR : public tag_ADD_CHAR {
  int16_t m_nCharIdx;
  int16_t m_nQuestIDX;
  // int16_t		m_nAdjSTATUS[];
};

// gsv_NPC_CHAR��Ŷ���� m_nCharIdx < 0 �̸� �������(Ŭ���̾�Ʈ���� ���������� ����)
struct gsv_NPC_CHAR : public gsv_MOB_CHAR {
  float   m_fModelDIR;
  int16_t m_nEventSTATUS; // 2004. 3. 22 �߰�.., NPC�ΰ����ɿ� ���� ���������� �ٲ�� ������..
  // int16_t		m_nAdjSTATUS[];
};

/// ���� ������ �Ҹ�ź 
struct tagShotDATA {
  union {
    struct {
      uint16_t m_cType : 5;
      uint16_t m_nItemNo : 10;
    };

    uint16_t m_wShotITEM;
  };
};

/*
  ��� ��� 
  1. ���ݼӵ� = ����� ���Ȱ� + �нú� ������ + ���� ������
  2. �̵��ӵ� = �������� ������(�нú�����) + ���� ���������� 
*/
struct tag_CLAN_ID {
  uint32_t m_dwClanID;

  union {
    uint32_t m_dwClanMARK;
    uint16_t m_wClanMARK[2];
  };

  uint8_t m_btClanLEVEL; // Ŭ�� ����
  uint8_t m_btClanPOS;   // Ŭ�������� ����
};

struct gsv_AVT_CHAR : public tag_ADD_CHAR {
  uint8_t     m_btCharRACE;
  int16_t     m_nRunSpeed;    // �нú꿡 ���� ������ ������, ���ӿ� ���� �����Ȱ� ���� ..
  int16_t     m_nPsvAtkSpeed; // �нú� ����...  �⺻�ӵ�, ���ӿ� ���� �����Ȱ� ���� ..
  uint8_t     m_btWeightRate; // ���������/�ִ������*100
  tagPartITEM m_PartITEM [ MAX_BODY_PART ];
  tagShotDATA m_sShotItem [ MAX_SHOT_TYPE ];

  int16_t m_nJOB;
  uint8_t m_btLEVEL;

  //	int16_t		m_nRidingITEM [ MAX_RIDING_PART ];	// �¿� ������ ..
  tagPartITEM m_RidingITEM [ MAX_RIDING_PART ]; // �¿� ������ ..
  int16_t     m_nPosZ;

  uint32_t m_dwSubFLAG;

  // char		szUserID[]
  // int16_t	m_nAdjSTATUS[];
  // ( tag_ADD_CHAR.m_dwStatusFALG & FLAG_ING_STORE_MODE ) �ϰ�� char szStoreTITLE[] ����

  /*	Ŭ�� ���� �߰�...
  tag_CLAN_ID	m_CLAN;	== NULL�� �ƴϸ� Ŭ������ ����
  // char m_szClanNAME[];
  */
};

struct gsv_SUB_OBJECT : public t_PACKETHEADER {
  uint16_t m_wObjectIDX[ 1 ];
};

struct cli_SET_WEIGHT_RATE : public t_PACKETHEADER {
  uint8_t m_btWeightRate;
};

struct gsv_SET_WEIGHT_RATE : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  uint8_t  m_btWeightRate;
};

struct gsv_ADJUST_POS : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  tPOINTF  m_PosCUR;
  int16_t  m_nPosZ;
};

struct cli_CANTMOVE : public t_PACKETHEADER {
  tPOINTF m_PosCUR;
  int16_t m_nPosZ;
};

struct cli_SETPOS : public t_PACKETHEADER {
  tPOINTF m_PosCUR;
};

struct cli_STOP : public t_PACKETHEADER {
  tPOINTF m_PosCUR;
};

struct gsv_STOP : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  tPOINTF  m_PosCUR;
  int16_t  m_nPosZ;
};

struct cli_MOUSECMD : public t_PACKETHEADER {
  uint16_t m_wTargetObjectIDX;
  tPOINTF  m_PosTO;
  int16_t  m_nPosZ;
};

struct gsv_MOUSECMD : public t_PACKETHEADER {
  uint16_t m_wSourObjIDX;
  uint16_t m_wDestObjIDX;

  //	tPOINTF	m_PosCUR;
  uint16_t m_wSrvDIST;
  tPOINTF  m_PosTO;

  int16_t m_nPosZ;
};

struct cli_ATTACK : public t_PACKETHEADER {
  uint16_t m_wTargetObjectIndex;
};

struct gsv_ATTACK : public t_PACKETHEADER {
  uint16_t m_wAtkObjIDX;
  uint16_t m_wDefObjIDX;

  //	tPOINTF	m_PosCUR;
  uint16_t m_wSrvDIST;
  tPOINTF  m_PosTO;
};

struct gsv_MOVE : public gsv_MOUSECMD {
  uint8_t m_btMoveMODE; // tag_ADD_CHAR::m_btMoveMODE�� ������
};

// tag_DROPITEM�� ���� �����鼭 �����°�...������ ���� �ð��� �����̸� Ŭ���̾�Ʈ�� ITEM_OBJ_LIVE_TIME ���� ����.
struct tag_DROPITEM {
  tPOINTF     m_PosCUR;
  tagBaseITEM m_ITEM;
  uint16_t    m_wServerItemIDX;
  uint16_t    m_wOwnerObjIDX;
  // uint16_t m_wRemainTIME;		// ���� :: ITEM_OBJ_LIVE_TIME�� ����.
};

struct gsv_ADD_FIELDITEM : public t_PACKETHEADER, public tag_DROPITEM {
  uint16_t m_wRemainTIME;
};

struct gsv_SUB_FIELDITEM : public t_PACKETHEADER {
  uint16_t m_wServerItemIDX;
};

struct cli_GET_FIELDITEM_REQ : public t_PACKETHEADER {
  uint16_t m_wServerItemIDX;
};

struct gsv_GET_FIELDITEM_REPLY : public t_PACKETHEADER {
  uint16_t    m_wServerItemIDX;
  uint8_t     m_btResult;
  int16_t     m_nInventoryListNO;
  tagBaseITEM m_ITEM;
};

#define	REPLY_GET_FIELDITEM_REPLY_OK			0x00	// ȹ���ߴ�.
#define REPLY_GET_FIELDITEM_REPLY_NONE			0x01	// ����� �������̴�..
#define REPLY_GET_FIELDITEM_REPLY_NO_RIGHT		0x02	// ������ ������ ����.
#define	REPLY_GET_FIELDITEM_REPLY_TOO_MANY		0x03	// ���̻� �������� ������ ����.

struct cli_DAMAGE : public t_PACKETHEADER {
  uint16_t m_wDefObjIDX;
  uint16_t m_wRandomIDX;
};

union uniDAMAGE {
  uint64_t m_wDamage;

  struct {
    uint32_t m_wVALUE;
    uint32_t m_wACTION;
  };
};

struct gsv_DAMAGE : public t_PACKETHEADER {
  uint16_t m_wAtkObjIDX;
  uint16_t m_wDefObjIDX;

  uniDAMAGE    m_Damage;
  tag_DROPITEM m_DropITEM[ 0 ]; // �״� �������ϰ�쿡�� ���� ��� �ִ�. ��ӵ� ������ �ε���
};

#define	MAX_DAMAGE			0x05F5E0FF //	99,999,999

// gsv_DAMAGE::m_wDamage�� ����Ǵ� ��.
//#define	DMG_BIT_DUMMY		0x0800		// �߰� ������ ���� �Ŵ� :: �ڽ��� ������� �߰� ������ ����� ��.
//#define	DMG_BIT_IMMEDIATE	0x1000		// ��Ű �ް� �ﰢ ó�� �ϸ� �ȴ�:��)���� ���� ��ų�� ���� �ݻ�� ����
//#define	DMG_BIT_HITTED		0x2000		// �´� ������ �Ѵ�.
//#define	DMG_BIT_CRITICAL	0x4000		// ũ��Ƽ�� ��������
//#define	DMG_BIT_DEAD		0x8000		// �׾���.

// gsv_DAMAGE::m_wDamage�� ����Ǵ� ��.
#define	DMG_BIT_DUMMY		  0x0100000000
#define	DMG_BIT_IMMEDIATE	0x0200000000
#define	DMG_BIT_HITTED		0x0400000000
#define	DMG_BIT_CRITICAL	0x0800000000
#define	DMG_BIT_DEAD		  0x1000000000

// gsv_DAMAGE::m_wACTION�� ���� ��...
#define	DMG_ACT_RESERVED	0x01
#define	DMG_ACT_ATTACK		0x02
#define	DMG_ACT_HITTED		0x04
#define	DMG_ACT_CRITICAL	0x08
#define	DMG_ACT_DEAD		0x10

struct gsv_CHANGE_NPC : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  int16_t  m_nNPCNo;
};

struct cli_SUMMON_CMD : public t_PACKETHEADER {
  uint8_t m_btCMD;
};

#define	SUMMON_CMD_ATTACK		0x00
#define	SUMMON_CMD_DEFENSE		0x01
#define	SUMMON_CMD_STANDING		0x02

// �ڽ��� ���� ����ġ�� �����Ѵ�.
struct gsv_SETEXP : public t_PACKETHEADER {
  int64_t  m_lCurEXP;
  int16_t  m_nCurSTAMINA;
  uint16_t m_wFromObjIDX;
};

struct gsv_LEVELUP : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  int16_t  m_nCurLevel;
  int64_t  m_lCurEXP;
  int16_t  m_nBonusPoint;
  int16_t  m_nSkillPoint;
};

struct cli_HP_REQ : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
};

struct gsv_HP_REPLY : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  int32_t  m_iHP; // MOB hp�� �����ϱ�°��̱� ������...
};

struct gsv_SET_HPnMP : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  int32_t  m_nHP;
  int32_t  m_nMP;
};

struct tag_BUY_ITEM {
  int8_t   m_cTabNO;
  int8_t   m_cColNO;
  uint16_t m_wDupCNT; // �ߺ��� �������ϰ�� ������ ����
};

struct tag_SELL_ITEM {
  uint8_t  m_btInvIDX;
  uint16_t m_wDupCNT; // �ߺ��� �������ϰ�� �Ǹ��� ����
};

// �������� �ŷ�...
struct cli_STORE_TRADE_REQ : public t_PACKETHEADER {
  uint16_t m_wNPCObjIdx;
  int8_t   m_cBuyCNT;
  int8_t   m_cSellCNT;
  uint32_t m_dwEconomyTIME;
  /*
    tag_BUY_ITEM	x m_cBuyCNT
    tag_SELL_ITEM	x m_cSellCNT
  */
};

struct gsv_STORE_TRADE_REPLY : public t_PACKETHEADER {
  uint8_t m_btRESULT;
};

#define	STORE_TRADE_RESULT_PRICE_DIFF		0x01
#define	STORE_TRADE_RESULT_NPC_NOT_FOUND	0x02
#define	STORE_TRADE_RESULT_TOO_FAR			0x03
#define	STORE_TRADE_RESULT_OUT_OF_MONEY		0x04
#define	STORE_TRADE_RESULT_NOT_UNION_USER	0x05	// ���� ���տ��� �ƴϴ�
#define	STORE_TRADE_RESULT_OUT_OF_POINT		0x06	// ���� ����Ʈ�� ���ڸ���

// ����� �κ��丮 ����...
struct tag_SET_INVITEM {
  uint8_t     m_btInvIDX;
  tagBaseITEM m_ITEM;
};

// ����� �κ��丮 ������ ��...
struct gsv_SET_MONEYnINV : public t_PACKETHEADER {
  int64_t         m_i64Money;
  uint8_t         m_btItemCNT;
  tag_SET_INVITEM m_sInvITEM[ 0 ]; // ����� ���� ��ŭ ��� �ִ�...
};

struct gsv_SET_INV_ONLY : public t_PACKETHEADER {
  uint8_t         m_btItemCNT;
  tag_SET_INVITEM m_sInvITEM[ 0 ]; // ����� ���� ��ŭ ��� �ִ�...
};

struct gsv_SET_MONEY_ONLY : public t_PACKETHEADER {
  int64_t m_i64Money;
};

struct gsv_SET_ABILITY : public t_PACKETHEADER {
  uint16_t m_wAbilityTYPE;
  int32_t  m_iValue;
};

#define	SERVER_DATA_ECONOMY		0
#define	SERVER_DATA_NPC			1

struct gsv_SERVER_DATA : public t_PACKETHEADER {
  uint8_t m_btDataTYPE;
  uint8_t m_pDATA[0];
};

#define	RELAY_TYPE_RECALL	0x00

struct gsv_RELAY_REQ : public t_PACKETHEADER {
  uint16_t m_wRelayTYPE;

  union {
    uint8_t m_pBODY[0];

    struct {
      int16_t m_nCallZoneNO;
      tPOINTF m_PosCALL;
    };
  };
};

struct cli_RELAY_REPLY : public gsv_RELAY_REQ {};

struct cli_ITEM_SKILL : public t_PACKETHEADER {
  int16_t m_nInventoryIndex;
};

struct cli_USE_ITEM : public t_PACKETHEADER {
  int16_t m_nInventoryIndex;
  // uint16_t		m_wDestObjIDX;		:: �Ҹ� ���������� Ÿ�� ��ų ����
  // tPOINTF	m_PosTARGET;		:: �Ҹ� ���������� ��ġ ��ų ����
};

struct gsv_USE_ITEM : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  int16_t  m_nUseItemNO;
  uint8_t  m_btInvIDX[ 0 ];
};

struct cli_DROP_ITEM : public t_PACKETHEADER {
  uint8_t  m_btInventoryIndex;
  uint32_t m_uiQuantity;
};

// m_nEquipInvIDX�� �޵�� ���� ��� �־����.
// m_nWeaponInvIDX�� ��� Ż���� ��� 0 !!
struct cli_EQUIP_ITEM : public t_PACKETHEADER {
  int16_t m_nEquipInvIDX;  // ����â �κ��丮 ��ȣ :: m_nEquipInvIDX >= 1 && m_nEquipInvIDX < MAX_EQUIP_IDX
  int16_t m_nWeaponInvIDX; // ��� �������� ��ġ�ϰ� �ִ� �κ��丮 ��ȣ...
};

struct gsv_EQUIP_ITEM : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  int16_t  m_nEquipIndex;
  //	int16_t		m_nItemNO;
  tagPartITEM m_PartITEM;
  int16_t     m_nRunSPEED[ 0 ]; // �нú� ��ų������ ������ ������ : ���ӿ� ���� �����Ȱ� ���� :: �� �� ž�� ��������..
};

struct cli_ASSEMBLE_RIDE_ITEM : public t_PACKETHEADER {
  int16_t m_nRidingPartIDX; // t_eRidePART�ǰ�.
  int16_t m_nRidingInvIDX;  // cli_EQUIP_ITEM::m_nWeaponInvIDX ó��...
};

struct gsv_ASSEMBLE_RIDE_ITEM : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  int16_t  m_nRidingPartIDX;
  //	int16_t		m_nItemNO;
  tagPartITEM m_RideITEM;
  int16_t     m_nRunSPEED[ 0 ]; // �нú� ��ų������ ������ ������ : ���ӿ� ���� �����Ȱ� ���� :: �� ž�� ������...
};

struct cli_USE_ITEM_TO_REPAIR : public t_PACKETHEADER {
  int16_t m_nUseItemInvIDX;      // ������ ����� �������� �κ� ��ȣ
  int16_t m_nRepairTargetInvIDX; // ������ �������� �κ� ��ȣ
};

struct cli_REPAIR_FROM_NPC : public t_PACKETHEADER {
  uint16_t m_wNPCObjIDX;          // ���� ��û�� NPC ���� �ε���
  int16_t  m_nRepairTargetInvIDX; // ������ �������� �κ� ��ȣ
};

struct gsv_SET_ITEM_LIFE : public t_PACKETHEADER {
  int16_t m_nInventoryIDX; // ������ ����� �������� �κ��丮 ��ȣ
  int16_t m_nLife;         // ���������� ���� ����
};

//struct cli_CHANGE_SKIN : public t_PACKETHEADER {
//	uint8_t	m_btBodyIDX;
//	int16_t	m_nItemNO;
//} ;
struct gsv_CHANGE_SKIN : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  uint16_t m_wAbilityTYPE;
  int32_t  m_iValue;
};

struct cli_MOVE_ITEM : public t_PACKETHEADER {
  uint8_t     m_btMoveTYPE;
  uint8_t     m_btFromIDX;
  tagBaseITEM m_MoveITEM; // <<-- �ű� ������ �־.. uint16_t m_wQuantity;
#ifdef	__INC_PLATINUM
  uint8_t m_btUseSpecialTAB; // ���� 1�̸� .. ����Ƽ�� ���񽺷� �߰��� �ǿ� ������ �ֱ�..
#endif
};

#define	MOVE_ITEM_TYPE_INV2BANK		0x00		// ITEM :: Inventory --> Bank
#define	MOVE_ITEM_TYPE_BANK2INV		0x01		// ITEM :: Bank --> Inventory

// ��Ŷ ������ == gsv_MOVE_ITEM �̸�				â��=>�κ��丮 �̵�
// ��Ŷ ������ == gsv_MOVE_ITEM+sizeof(int64_t)��	�κ�=>â��, m_iCurMoney[0]�� ����� ����
struct gsv_MOVE_ITEM : public t_PACKETHEADER {
  int16_t m_nInvIDX;  // IDX == -1 ����
  int16_t m_nBankIDX; // IDX == -1 ����

  tagBaseITEM m_InvItem;
  tagBaseITEM m_BankITEM;
  int64_t     m_iCurMoney[0]; // ������ ���� �����...
};

#define	MOVE_ZULY_TYPE_INV2BANK		0x10		// ZULY :: Inventory --> Bank
#define MOVE_ZULY_TYPE_BANK2INV		0x11		// ZULY :: Bank --> Inventory

struct cli_MOVE_ZULY : public t_PACKETHEADER {
  uint8_t m_btMoveTYPE;
  int64_t m_i64MoveZuly; // �̵� �� �ٸ���
};

struct gsv_MOVE_ZULY : public t_PACKETHEADER {
  int64_t m_i64InvZuly;  // �̵��� ������ �ٸ���
  int64_t m_i64BankZuly; // �̵��� ������ �ٸ���
};

#define	BANK_REQ_OPEN				0x00		// â�� ����..
#define	BANK_REQ_CHANGE_PASSWORD	0x01		// â�� ��� �ٲܶ�...�ݵ�� â���� ���� �ִ� ���¿���...

struct cli_BANK_LIST_REQ : public t_PACKETHEADER {
  uint8_t m_btREQ;
  int8_t  m_szPassword[0];
};

#define	BANK_REPLY_INIT_DATA		0x00		// â�� ����Ÿ �ʱ�ȭ & �����ش�
#define	BANK_REPLY_SHOW_DATA		0x01		// â�� ���� �ش� m_btItemCNT == 0
#define	BANK_REPLY_NEED_PASSWORD	0x02		// â�� ����� �ʿ�..
#define	BANK_REPLY_CHANGED_PASSWORD	0x03		// â�� ����� �������
#define	BANK_REPLY_INVALID_PASSWORD	0x04		// â�� ����� Ʋ��
#define	BANK_REPLY_CLEARED_PASSWORD	0x05		// â�� ��� ������
#define	BANK_REPLY_PLATINUM			0x06		// �÷�Ƽ�� â�� ����( BANK_REPLY_INIT_DATA�ڿ� �̾ ���۵� )

struct gsv_BANK_LIST_REPLY : public t_PACKETHEADER {
  uint8_t         m_btREPLY;
  uint8_t         m_btItemCNT;
  tag_SET_INVITEM m_sInvITEM[ 0 ]; // ����� ���� ��ŭ ��� �ִ�...
};

struct cli_TELEPORT_REQ : public t_PACKETHEADER {
  int16_t m_nWarpIDX;
  tPOINTF m_PosCUR; // ���� ������ :: Ŭ���̾�Ʈ�� ���� ��ǥ�� �ָ� ���� ��ǥ�� ��
};

struct gsv_TELEPORT_REPLY : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  int16_t  m_nZoneNO;
  tPOINTF  m_PosWARP;
  uint8_t  m_btRunMODE;
  uint8_t  m_btRideMODE;
};

// m_btAbilityNO >= 100	: ��ų ����Ʈ �ε����� +100�ؼ� ����...
struct cli_USE_BPOINT_REQ : public t_PACKETHEADER {
  uint8_t m_btAbilityNO;
};

struct gsv_USE_BPOINT_REPLY : public t_PACKETHEADER {
  uint8_t m_btAbilityNO;
  int16_t m_nAbilityValue;
};

struct cli_SET_HOTICON : public t_PACKETHEADER {
  uint8_t    m_btListIDX;
  tagHotICON m_HotICON;
};

struct gsv_SET_HOTICON : public cli_SET_HOTICON {};

struct cli_SET_BULLET : public t_PACKETHEADER {
  struct {
    uint16_t m_wShotTYPE : 2;
    uint16_t m_wInventoryIDX : 14;
  };
};

struct gsv_SET_BULLET : public t_PACKETHEADER {
  uint16_t    m_wObjectIDX;
  tagShotDATA m_sShot; // m_sShot.m_wItemNO == 0 �ϰ�� �Ѿ� �� ��� !!!
};

struct gsv_SKILL_LEARN_REPLY : public t_PACKETHEADER {
  uint8_t m_btResult;
  uint8_t m_btSkillSLOT;
  int16_t m_nSkillIDX;
  int16_t m_nSkillPOINT;
};

#define	RESULT_SKILL_LEARN_FAILED			0x00		// ���� ����.
#define	RESULT_SKILL_LEARN_SUCCESS			0x01		// ���� ����.
#define	RESULT_SKILL_LEARN_NEED_JOB			0x02		// ������ ��ġ���� �ʴ´�.
#define	RESULT_SKILL_LEARN_NEED_SKILL		0x03		// ������ ��ų�� �ʿ��Ѵ�.
#define	RESULT_SKILL_LEARN_NEED_ABILITY		0x04		// �ɷ�ġ�� �����ϴ�
#define	RESULT_SKILL_LEARN_OUTOFSLOT		0x05		// ���̻� ��ų�� ���� ����.
#define	RESULT_SKILL_LEARN_INVALID_SKILL	0x06		// �߸��� ��ų ��ȣ��.
#define	RESULT_SKILL_LEARN_OUTOFPOINT		0x07		// ��ų ����Ʈ ����
#define	RESULT_SKILL_LEARN_DELETE			0x08		// m_btSkillSLOT�� m_nSkillIDX�� ��ų�� �����ض�..

// ��ų ��������...
struct cli_SKILL_LEVELUP_REQ : public t_PACKETHEADER {
  uint8_t m_btSkillSLOT;
  int16_t m_nNextLevelSkillIDX;
};

struct gsv_SKILL_LEVELUP_REPLY : public t_PACKETHEADER {
  uint8_t m_btResult;
  uint8_t m_btSkillSLOT;
  int16_t m_nSkillINDEX;
  int16_t m_nSkillPOINT;
};

#define	RESULT_SKILL_LEVELUP_SUCCESS		0x00
#define	RESULT_SKILL_LEVELUP_FAILED			0x01
#define	RESULT_SKILL_LEVELUP_OUTOFPOINT		0x02
#define	RESULT_SKILL_LEVELUP_NEED_ABILITY	0x03
#define	RESULT_SKILL_LEVELUP_NEED_JOB		0x04
#define	RESULT_SKILL_LEVELUP_NEED_SKILL		0x05
#define	RESULT_SKILL_LEVELUP_OUTOFZULY		0x06		// 05.05.25 �ٸ��� ���ڸ���

/// ��ų�� ��� ���� : ������ ������
struct cli_SELF_SKILL : public t_PACKETHEADER {
  uint8_t m_btSkillSLOT;
};

struct gsv_SELF_SKILL : public t_PACKETHEADER {
  uint16_t m_wSourObjIDX;
  int16_t  m_nSkillIDX;
  int8_t   cNpcSkillMOTION[ 0 ];
};

/// ��ų�� Ÿ�ٿ� ���� : ������ ������
struct cli_TARGET_SKILL : public t_PACKETHEADER {
  uint16_t m_wDestObjIDX;
  uint8_t  m_btSkillSLOT;
};

struct gsv_TARGET_SKILL : public t_PACKETHEADER {
  uint16_t m_wSourObjIDX;
  uint16_t m_wDestObjIDX;
  int16_t  m_nSkillIDX;

  uint16_t m_wSrvDIST;
  tPOINTF  m_PosTO; // ��ǥ ������	2004. 5. 1 �߰�

  int8_t cNpcSkillMOTION[ 0 ];
};

/// ��ų�� ������ ���� : ������ ������
struct cli_POSITION_SKILL : public t_PACKETHEADER {
  uint8_t m_btSkillSLOT;
  tPOINTF m_PosTARGET;
};

struct gsv_POSITION_SKILL : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  int16_t  m_nSkillIDX;
  tPOINTF  m_PosTARGET;
  int8_t   cNpcSkillMOTION[ 0 ];
};

struct gsv_EFFECT_OF_SKILL : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  uint16_t m_wSpellObjIDX;

  // ȫ��
#ifdef _GBC
  uint16_t m_nSkillIDX;
  uint16_t m_nINT;
  uint8_t  m_btSuccessBITS;
#else
  union {
    struct {
      uint16_t	m_nSkillIDX		: 12;
      uint16_t	m_btSuccessBITS : 2;	// ��������
      uint8_t			m_tmp1;
    } ;
    struct {
      uint8_t			m_tmp2;
      uint16_t	m_tmp3			: 6;
      uint16_t	m_nINT			: 10;	// �������� ����
    } ;
  } ;
#endif

};

struct gsv_DAMAGE_OF_SKILL : public gsv_EFFECT_OF_SKILL {
  union {
    uniDAMAGE m_Damage; // ���ذ� ������� ���� ��� ����.
    uint16_t  m_wDamage;
  };

  tag_DROPITEM m_DropITEM[ 0 ]; // �״� �������ϰ�쿡�� ���� ��� �ִ�. ��ӵ� ������ �ε���
};

struct gsv_RESULT_OF_SKILL : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  int16_t  m_nSkillIDX;
};

struct gsv_SKILL_START : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
};

#define	SKILL_CANCEL_NEED_ABILITY		0x01	// �ɷ�ġ ����
#define	SKILL_CANCEL_NEED_TARGET		0x02	// �ɽ��� �õ��� Ÿ���� ��������... MP�Ҹ� ����
#define	SKILL_CANCEL_TARGET_NOT_FOUND	0x03	// �ɽ����� ���������� Ÿ���� ��������... MP�Ҹ� ����

struct gsv_SKILL_CANCEL : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  uint8_t  m_btWHY;
};

/*
  **	�������� ��Ŷ�� ���� �ԷµǴ� ���... AND ��ų �÷��״� DataType.h�� ����� ���� ũ�� ������
    m_gsv_CLEAR_STATUS.m_nAdjVALUE[x]�� ���� �ԷµǾ� ����.

  dwClearedFLAG = Ŭ���̾�Ʈ���� & ~��������

  uint8_t btIDX=0;	
  if ( dwClearedFLAG &  FLAG_ING_HP )
    clientHP = m_gsv_CLEAR_STATUS.m_nAdjVALUE[ btIDX++ ];

  if ( dwClearedFLAG &  FLAG_ING_MP )
    clientMP = m_gsv_CLEAR_STATUS.m_nAdjVALUE[ btIDX++ ];
*/
struct gsv_CLEAR_STATUS : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  uint32_t m_dwStatusFLAG; // �������� ���� �÷���...
  int16_t  m_nAdjVALUE[0]; // �������� ������ ��...
};

struct gsv_SPEED_CHANGED : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  int16_t  m_nRunSPEED;    // �нú� ���¸� ����, ���� ���� ����
  int16_t  m_nPsvAtkSPEED; // �нú� ����...
  uint8_t  m_btWeightRate; // ���������/�ִ������*100
};

// ��� �������� �����Ƿ� :: m_nInventoryIndex�� ������ ���� ��û ..
struct cli_APPRAISAL_REQ : public t_PACKETHEADER {
  uint16_t m_wInventoryIndex;
};

struct gsv_APPRAISAL_REPLY : public t_PACKETHEADER {
  struct {
    uint16_t m_wInventoryIndex : 12;
    uint16_t m_btResult : 4;
  };
};

#define	RESULT_APPRAISAL_REPLY_OK		0	// ����
#define	RESULT_APPRAISAL_REPLY_FAILED	1	// ������.

/// �ŷ� ��� m_wObjectIDX���� ������ ������.
struct cli_TRADE_P2P : public t_PACKETHEADER {
  uint8_t  m_btRESULT;
  uint16_t m_wObjectIDX;
  int8_t   m_cTradeSLOT;
};

#define	RESULT_TRADE_REQUEST			0x00	// �ŷ� �䱸
#define	RESULT_TRADE_ACCEPT				0x01	// �ŷ� �³�
#define	RESULT_TRADE_REJECT				0x02	// �ŷ� �ź�
#define	RESULT_TRADE_CANCEL				0x03	// �ŷ� ���� ���
#define	RESULT_TRADE_CHECK_READY		0x04	// �ŷ� �غ� �Ϸ�
#define	RESULT_TRADE_UNCHECK_READY		0x05	// �ŷ� �غ� ���
#define	RESULT_TRADE_DONE				0x06	// �ŷ� ����
#define	RESULT_TRADE_BUSY				0x07	// �ŷ��� �Ҽ� ����.
#define	RESULT_TRADE_TOO_FAR			0x08	// �Ÿ��� �ִ�.	5m �̳�..
#define	RESULT_TRADE_NOT_TARGET			0x09	// ����� ����.
#define	RESULT_TRADE_OUT_OF_INV			0x0a	// ������ �κ��丮�� �����ϴ�.
#define	RESULT_TRADE_NO_CHARGE_TARGET	0x0b

/// �������� ���۵� m_wObjectIDX�� �ŷ��� ����.
struct gsv_TRADE_P2P : public t_PACKETHEADER {
  uint8_t  m_btRESULT;
  uint16_t m_wObjectIDX;
  int8_t   m_cTradeSLOT;
};

#define	MAX_TRADE_ITEM_SLOT		11
#define	TRADE_MONEY_SLOT_NO		(MAX_TRADE_ITEM_SLOT-1)

struct cli_TRADE_P2P_ITEM : public t_PACKETHEADER {
  int8_t   m_cTradeSLOT;
  int16_t  m_nInventoryIndex; // �κ��丮 ��ȣ
  uint32_t m_uiQuantity;      // ����
};

struct gsv_TRADE_P2P_ITEM : public t_PACKETHEADER {
  int8_t      m_cTradeSLOT;
  tagBaseITEM m_ITEM;
};

struct cli_SET_WISHITEM : public t_PACKETHEADER {
  uint8_t     m_btWishSLOT;
  tagBaseITEM m_ITEM;
};

// ���� ���� ����....
#define	MAX_P_STORE_ITEM_SLOT	31

// �Ǹ��� ������
struct tagPS_ITEM {
  union {
    uint8_t m_btInvIDX;  // �ǸŽ� �κ���ȣ
    uint8_t m_btWishIDX; // ���Ž� WishList ���� ��ȣ
  };

  tagBaseITEM m_ITEM;         // ������( �ߺ����� �������ϰ�� �ȷ��� ���� ����)
  uint32_t    m_dwPricePerEA; // 1���� �Ǹ� ��� ����
};

#define	STORE_MODE_TO_SELL		0x01
#define	STORE_MODE_TO_BUY		0x02

struct cli_P_STORE_OPEN : public t_PACKETHEADER {
  uint8_t m_btSellItemCNT; // �Ǹ� ��� ������ ��� ����
  uint8_t m_btWishItemCNT; // ���� ��� ������ ��� ����

  // ����� ������
  tagPS_ITEM m_ITEMs[ 0 ]; // �Ǹ� 3, ���� 2 �ϰ�� 0~2�� �Ǹ�, 3~4�� ������� ������
  // char	m_szStoreTITLE[0]
};

struct gsv_P_STORE_OPENED : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  int8_t   m_szStoreTITLE[0];
};

struct cli_P_STORE_CLOSE : public t_PACKETHEADER {};

struct gsv_P_STORE_CLOSED : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
};

struct tagPS_SLOT_PRICE {
  uint8_t     m_btSLOT;
  tagBaseITEM m_SlotITEM;
  uint32_t    m_dwPRICE;
};

struct cli_P_STORE_LIST_REQ : public t_PACKETHEADER {
  // ���� ���� ������ ��� �䱸
  uint16_t m_wStoreObjectIDX;
};

struct gsv_P_STORE_LIST_REPLY : public t_PACKETHEADER {
  uint8_t          m_btSellItemCNT;
  uint8_t          m_btWishItemCNT;
  tagPS_SLOT_PRICE m_SlotITEMs[0];
};

struct tagPS_SLOT_ITEM {
  uint8_t     m_btSLOT;
  tagBaseITEM m_SlotITEM;
};

// ���� ������ ���� ��û
struct cli_P_STORE_BUY_REQ : public t_PACKETHEADER {
  uint16_t        m_wStoreObjectIDX;
  uint8_t         m_btItemCNT;
  tagPS_SLOT_ITEM m_BuyITEMs[0];
};

struct tagSELL_ITEM {
  uint8_t     m_btInvIDX;   // �ȷ��� �������� ��ġ�� �κ��丮 ��ȣ
  uint8_t     m_btWishSLOT; // ���� ������ ���ϴ� �������� ��ġ�� ��ȣ
  tagBaseITEM m_SellITEM;   // �ȷ��� ������(�ߺ����� �������� ��� �ȷ��� �������� ����)
};

// ���� ������ �Ǹ� ��û
struct cli_P_STORE_SELL_REQ : public t_PACKETHEADER {
  uint16_t     m_wStoreObjectIDX;
  uint8_t      m_btItemCNT;
  tagSELL_ITEM m_SellITEMs[0];
};

struct gsv_P_STORE_RESULT : public t_PACKETHEADER {
  uint16_t        m_wStoreObjectIDX;
  uint8_t         m_btResult;
  uint8_t         m_btItemCNT;
  tagPS_SLOT_ITEM m_UpdatedITEM[0];
};

#define	RESULT_P_STORE_CANCLED				0x01	// �ŷ� ���,
#define	RESULT_P_STORE_TRADE_SOLD_OUT		0x02	// ������ ǰ��
#define	RESULT_P_STORE_TRADE_NO_MORE_NEED	0x03	// ���� ������ ���ϴ� �������� ������ ���� �ȾҴ�.
#define RESULT_P_STORE_BOUGHT_PART			0x04	// ���� �Ϸ�...��, �Ϻ� ǰ���� ǰ���Ǿ� ���� ����...
#define RESULT_P_STORE_BOUGHT_ALL			0x05	// ��û�� ǰ�� ��ü ���� �Ϸ� 
#define RESULT_P_STORE_SOLD_PART			0x06	// �Ǹ� �Ϸ�...��, �Ϻ� ǰ���� ���ԿϷ�Ǿ� �Ǹ� ����...
#define RESULT_P_STORE_SOLD_ALL				0x07	// ��û�� ǰ�� ��ü �Ǹ� �Ϸ�

/*
  party structure ...
*/
struct cli_PARTY_REQ : public t_PACKETHEADER {
  uint8_t  m_btREQUEST;
  uint32_t m_dwDestIDXorTAG;
};

struct gsv_PARTY_REQ : public t_PACKETHEADER {
  uint8_t  m_btREQUEST;
  uint32_t m_dwFromIDXorTAG;
  //	int16_t	m_nZoneNO;
};

#define	PARTY_REQ_MAKE				0x00	// ��Ƽ �Ἲ ��û 
#define	PARTY_REQ_JOIN				0x01	// ��Ƽ ���� ��û
#define	PARTY_REQ_LEFT				0x02	// ��Ƽ Ż�� ��û
#define	PARTY_REQ_CHANGE_OWNER		0x03	// ��¯ ���� ��û.

#define	PARTY_REQ_BAN				0x81	// ��Ƽ ���� ��û

struct cli_PARTY_REPLY : public t_PACKETHEADER {
  uint8_t  m_btREPLY;
  uint32_t m_dwDestIDXorTAG;
};

struct gsv_PARTY_REPLY : public t_PACKETHEADER {
  uint8_t  m_btREPLY;
  uint32_t m_dwFromIDXorTAG;
};

#define	PARTY_REPLY_NOT_FOUND				0x00	// ����� ã�� ���ߴ�.
#define	PARTY_REPLY_BUSY					0x01	// ����� ��Ƽ�� ���� �Ҽ� ���� ���´�.
#define	PARTY_REPLY_ACCEPT_MAKE				0x02
#define	PARTY_REPLY_ACCEPT_JOIN				0x03
#define	PARTY_REPLY_REJECT_JOIN				0x04	// ��Ƽ �Ἲ/������ ���� ..
#define	PARTY_REPLY_DESTROY					0x05	// ��Ƽ �ǰ� ����
#define PARTY_REPLY_FULL_MEMBERS			0x06	// ���� �����̴�.
#define	PARTY_REPLY_INVALID_LEVEL			0x07	// ������ ���� �ʴ´�
#define	PARTY_REPLY_CHANGE_OWNER			0x08
#define	PARTY_REPLY_CHANGE_OWNERnDISCONN	0x09	// PARTY_REPLY_CHANGE_OWNER�� ����...�� ���� ��¯�� disconnect�� ���·� ����.
#define	PAATY_REPLY_NO_CHARGE_TARGET		0x0a

#define	PARTY_REPLY_BAN						0x80
#define PARTY_REPLY_DISCONNECT				0x81	// ������ ���� �ƴ�...
#define	PARTY_REPLY_REJOIN					0x82	// ������ ������ �����ߴ�.

/*
struct tag_PARTY_USER {		// ���� �������� ���۵�
  uint16_t	m_wUserTAG;
  //char	m_szCharName[]
} ;
struct wsv_PARTY_USER : public t_PACKETHEADER {
  char	m_cUserCNT;					// -1�̸� ��Ƽ������ ����, ��� m_cType�� �߰��� ����ڼ�...
  uint16_t	m_wUserTAG[0];				// ��Ƽ�� ������ ��� ����...
//	tag_PARTY_USER[ m_cUserCNT ];		// ��Ƽ�� �߰��� ��� ����
} ;
*/
struct tag_PARTY_MEMBER {
  // �� �������� ���۵�
  uint32_t m_dwUserTAG;
  uint16_t m_wObjectIDX;
  uint32_t m_nMaxHP;
  uint32_t m_nHP;
  uint32_t m_dwStatusFALG;

  int16_t m_nCON; // uint8_t	m_btCON; //�ִ� ���� ������ 300���� ������ 2004. 7. 21

  uint16_t m_btRecoverHP; // item_recover_hp + passive_recover_hp
  uint16_t m_btRecoverMP; // item_recover_mp + passive_recover_mp

  int16_t m_nSTAMINA;

  //char	m_szCharName[];				// ** PARTY_MEMBER_ADD�� ��츸 ���� ��� ����
};

struct gsv_PARTY_MEMBER : public t_PACKETHEADER {
  uint8_t  m_btPartyRULE;    // ��Ƽ ��Ģ(�����ۺй�, ����ġ�й���)
  int8_t   m_cUserCNT;       // -1�̸� ��Ƽ������ ����, ��� m_cType�� �߰��� ����ڼ�, ObjTAG...
  uint32_t m_dwObjectTAG[0]; // ��Ƽ�� ������ ��� ����...
  /*
  tag_PARTY_MEMBER[ m_cUserCNT ];		// ��Ƽ�� �߰��� ��� ����
  */
};

#define	PARTY_MEMBER_SUB	(-1)

struct gsv_PARTY_LEVnEXP : public t_PACKETHEADER {
  uint8_t m_btLEVEL;

  struct {
    uint32_t m_iEXP : 31;
    uint32_t m_bitLevelUP : 1;
  };
};

struct gsv_CHANGE_OBJIDX : public t_PACKETHEADER {
  tag_PARTY_MEMBER m_Member;
};

struct gsv_PARTY_ITEM : public t_PACKETHEADER {
  uint16_t    m_wObjectIDX;
  tagBaseITEM m_ITEM;
};

#define	BIT_PARTY_RULE_EXP_PER_PLAYER		0x001	// ����ġ�� ������ʷ� ������...
#define	BIT_PARTY_RULE_ITEM_TO_ORDER		0x080	// ������� �켱�� �ο�...

struct cli_PARTY_RULE : public t_PACKETHEADER {
  uint8_t m_btPartyRUEL;
};

struct gsv_PARTY_RULE : public t_PACKETHEADER {
  uint8_t m_btPartyRULE;
};

struct gsv_ADD_EVENTOBJ : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  int8_t   m_cMapX;
  int8_t   m_cMapY;
  uint16_t m_wMapEventID;
  int16_t  m_nEventSTATUS;
};

/*
struct gsv_SUB_EVENTOBJ : public t_PACKETHEADER {
  uint16_t	m_wObjectIDX;
} ;
*/

struct gsv_SET_NPC_SHOW : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  bool     m_bShow; // m_bShow == true�̸� ����/false�̸� ����
};

//--------------------------- CLAN ���� ��Ŷ
#define	GCMD_CREATE				0x000		//������, /guildcreate [����]
#define	GCMD_INFO				0x001		//�������, /ginfo - ��忡 ���� �⺻���� ����
#define	GCMD_INVITE				0x002		//����ʴ�, /ginvite <�÷��̾�> - ��忡 �ش� �÷��̾� �ʴ��ϱ�
#define	GCMD_REMOVE				0x003		//����߹�, /gremove <�÷��̾�> - ��忡�� �ش� �÷��̾� �߹��ϱ�
#define	GCMD_PROMOTE			0x004		//���±�, /gpromote <�÷��̾�> - �ش� �÷��̾� ��� ��� �ø���
#define	GCMD_DEMOTE				0x005		//��尭��, /gdemote <�÷��̾�> - �ش� �÷��̾� ��� ��� ������
#define	GCMD_MOTD				0x006		//������, /gmotd <�Ҹ�> - ������ ��� �޽��� ���ϱ�
#define	GCMD_QUIT				0x007		//���Ż��, /gquit - ��忡�� Ż���ϱ�
#define	GCMD_ROSTER				0x008		//�����, /groster - ��ü ���� ��� ����
#define	GCMD_LEADER				0x009		//�������, /gleader <�÷��̾�> - �ٸ� �÷��̾�� ����� �����ϱ� (����� ����)
#define	GCMD_DISBAND			0x00a		//�����ü, /gdisband - ��� ��ü�ϱ� (����� ����) 

#define GCMD_INVITE_REQ			0x00b		//������ Ŭ�� ������ ��û�ߴ�. <��û���̸�>
#define	GCMD_INVITE_REPLY_YES	0x00c		//Ŭ������ �³� <��û���̸�>
#define	GCMD_INVITE_REPLY_NO	0x00d		//Ŭ������ ���� <��û���̸�>

#define	GCMD_SLOGAN				0x00e		//��彽�ΰ� /clan slotan ���ΰ�

#define	GCMD_MEMBER_JOBnLEV		0x00f		//�ڽ��� ����/������ �ٸ� ������� �뺸( �α�����/��������/�����Ŀ� ���� )

//-- ���� ���� Ÿ��...
#define	GCMD_LOGIN				0x0f0
#define	GCMD_LOGOUT				0x0f1
#define	GCMD_CHAT				0x0f2
#define	GCMD_ADJ_VAR			0x0f3
#define	GCMD_CLANMARK_SET		0x0f4
#define	GCMD_CLANMARK_GET		0x0f5
#define GCMD_CLANMARK_REGTIME	0x0f6

struct cli_CLAN_COMMAND : public t_PACKETHEADER {
  uint8_t m_btCMD;
};

struct cli_CLAN_CREATE : public cli_CLAN_COMMAND {
  uint16_t m_wMarkIDX[2];
  // char	m_szName[];
  // char m_szDesc[];
};

struct cli_CLAN_MEMBER_JOBnLEV : public cli_CLAN_COMMAND {
  int16_t m_nLEVEL;
  int16_t m_nJOB;
};

struct wsv_CLAN_MEMBER_JOBnLEV : public t_PACKETHEADER {
  uint8_t m_btRESULT;
  int16_t m_nLEVEL;
  int16_t m_nJOB;
  // char m_szCharNAME[]
};

//struct tag_CLAN_INFO {
//	int16_t		m_nClanLEVEL;
//	int32_t			m_iClanSCORE;
//	int32_t			m_iClanRATE;	// â��ȿ��
//	int64_t		m_biClanMONEY;
//	char szName[];				�̸�
//	char szDesc[];				���ΰ� ������ NULL
//	char szAlliedGroup[]		���� �̸� ������ NULL
//} ;

#define	MAX_CLAN_SKILL_SLOT		20
#ifdef	MAX_CLAN_SKILL_SLOT
struct tagClanSKILL {
  int16_t  m_nSkillIDX;
  uint32_t m_dwExpiredAbsSEC; // ��� ���� �ð�
  //	int16_t	m_nCount;			// ��� ���� Ƚ�� -> ���� ��밡���� Ƚ��?? �����͸�??
};

struct tagClanBIN {
  union {
    uint8_t      m_pDATA[ 0 ];
    tagClanSKILL m_SKILL[ MAX_CLAN_SKILL_SLOT ];
  };
};

#endif

struct tag_MY_CLAN : public tag_CLAN_ID {
  int32_t m_iClanSCORE; // Ŭ�� ����Ʈ
  int32_t m_iClanRATE;  // â��ȿ��
  int64_t m_biClanMONEY;
  int16_t m_nMemberCNT; // ��� ��
  int32_t m_iClanCONT;  // Ŭ�� �⿩��
#ifdef	MAX_CLAN_SKILL_SLOT
  tagClanBIN m_ClanBIN; // Ŭ�� ���̳ʸ� ����Ÿ..
#endif
  // char m_szClanName[];
  // char m_szClanDESC[];
  // char m_szClanMOTD[];
};

struct tag_CLAN_MEMBER {
  uint8_t m_btClanPOS;
  uint8_t m_btChannelNO; // ���� 0xff�̸� �������� �ʴ� ������...
  int32_t m_iClanCONTRIBUTE;
  int16_t m_nLEVEL;
  int16_t m_nJOB;
  // char m_szCharNAME[]
};

struct wsv_CLAN_COMMAND : public t_PACKETHEADER {
  uint8_t m_btRESULT;
  char    m_szStr[0];

  /*
  // m_btRESULT == RESULT_CLAN_MY_DATA
    tag_MY_CLAN	m_CLAN;
    // char m_szClanNAME[];
  */

  /*
  // m_btRESULT == RESULT_CLAN_INFO �ϰ��
  struct tag_CLAN_INFO
  */
  /*
  // m_btRESULT == RESULT_CLAN_ROSTER �ϰ��
  tag_CLAN_MEMBER	m_Members[ x ];
  */
  /*
  // m_btRESULT == RESULT_CLAN_KICK, RESULT_CLAN_POSITION �ϰ��
  strujct {
    char *m_szMember;	Ż����
    char *m_szMaster;	Ż���Ų�� == NULL�ϰ�� ������ ������
  */
  /*
  // m_btRESULT == RESULT_CLAN_JOINED
  struct {
    char *m_szMember;	�ű� ������
    char *m_szMaster;	���Խ�Ų��
  } ;
  */
  /*
  // m_btRESULT == RESULT_CLAN_SET :: �ֺ� ������� Ŭ�� ���� ����
  struct {
    uint16_t		m_wObjIDX;
    tag_CLAN_ID	;
  } ;
  */
};

struct wsv_RESULT_CLAN_SET : public t_PACKETHEADER {
  uint8_t     m_btRESULT;
  uint16_t    m_wObjIDX;
  tag_CLAN_ID m_ClanID;
};

#define	RESULT_CLAN_CREATE_OK			0x030		// ���� â�� �ߴ�.
#define RESULT_CLAN_JOIN_OK				0x031		// ���� �ű� ���Եƴ�
#define	RESULT_CLAN_DESTROY_OK			0x032
#define RESULT_CLAN_MY_DATA				0x033		// �̹� ���Ե� Ŭ���� �ִ� ���¿��� ���� Ŭ�� ����
#define	RESULT_CLAN_MOTD				0x034		// Ŭ�� ���� �ٲ����.
#define	RESULT_CLAN_SET					0x035		// �ֺ� ������� Ŭ�� ���� ����
#define	RESULT_CLAN_SLOGAN				0x036		// Ŭ�� ���ΰ� �ٲ����

#define	RESULT_CLAN_CREATE_FAILED		0x041
#define	RESULT_CLAN_CREATE_DUP_NAME		0x042
#define	RESULT_CLAN_CREATE_NO_RIGHT		0x043		// â�� ���� ����.
#define	RESULT_CLAN_CREATE_NO_CONDITION	0x044		// â�� ���� �ȸ´´�.

#define	RESULT_CLAN_DESTROYED			0x051		// Ŭ�� �ǰ� ����..
#define	RESULT_CLAN_DESTROY_FAILED		0x052
#define	RESULT_CLAN_DESTROY_NO_RIGHT	0x053		// �ǰ� ���� ����.

#define	RESULT_CLAN_JOIN_MEMBER			0x061		// �ű� �����ߴ� :: �� Ŭ���� ���� ����..
#define	RESULT_CLAN_JOIN_FAILED			0x062
#define	RESULT_CLAN_JOIN_NO_RIGHT		0x063		// ���� ��ų ������ ����.
#define	RESULT_CLAN_JOIN_HAS_CLAN		0x064		// �ٸ� ��忡 ������ �ִ�.
#define	RESULT_CLAN_JOIN_MEMBER_FULL	0x065		// ��� ��á��.

#define	RESULT_CLAN_INFO				0x071		// Ŭ�� ����
#define	RESULT_CLAN_ROSTER				0x072		// Ŭ�� ����Ʈ

#define	RESULT_CLAN_MEMBER_LOGIN		0x073		// ��ü���� �������� ��� �α���
#define	RESULT_CLAN_MEMBER_LOGOUT		0x074		// ��ü���� �������� ��� �α׾ƿ�
#define	RESULT_CLAN_MEMBER_POSITION		0x075		// ��ü���� �������� ��� ��������

#define	RESULT_CLAN_KICK				0x081		// Ŭ������ �߹��
#define	RESULT_CLAN_QUIT				0x082		// �Ҽ� Ŭ������ ������( Ŭ������ �ʱ�ȭ )
#define	RESULT_CLAN_POSITION			0x083		// Ŭ�������� ����� �����ƴ�.
#define	RESULT_CLAN_MEMBER_JOBnLEV		0x084		// �α��ε� ����� ����/����

// m_dwClanID�� Ŭ�� ��ũ�� ������ ��û
struct cli_CLANMARK_REQ : public t_PACKETHEADER {
  uint32_t m_dwClanID;
};

// m_btMARK����Ÿ�� �ڽ��� Ŭ�� ��ũ�� ����
struct cli_CLANMARK_SET : public t_PACKETHEADER {
  uint16_t m_wMarkCRC16;
  //	uint8_t	m_btMARK[ 0 ];
};

// m_dwClanIDŬ���� Ŭ����ũ ����Ÿ
#define	RESULT_CLANMARK_TOO_MANY_UPDATE		0x0001	// ��ũ ������ �����ð� �Ŀ� �ؾ� �Ѵ�...
#define	RESULT_CLANMARK_DB_ERROR			0x0002	// ��� ���� ����
#define	RESULT_CLANMAKR_SP_ERROR			0x0003	// ��� SP ����

struct wsv_CLANMARK_REPLY : public t_PACKETHEADER {
  uint32_t m_dwClanID;

  union {
    uint16_t m_wMarkCRC16;
    uint16_t m_wFailedReason; // m_dwClanID == 0 �ϰ�� ���� ���� ��� ����.
  };

  //	uint8_t	m_btMARK[ 0 ];
};

struct cli_CLANMARK_REG_TIME : public t_PACKETHEADER {};

struct wsv_CLANMARK_REG_TIME : public t_PACKETHEADER {
  uint16_t m_wYear;
  uint8_t  m_btMon;
  uint8_t  m_btDay;
  uint8_t  m_btHour;
  uint8_t  m_btMin;
  uint8_t  m_btSec;
};

struct gsv_CHEAT_CODE : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  char     m_szCheatCODE[0];
};

/*
  ������ ���� ����...
*/
#define	CREATE_ITEM_STEP	4

struct cli_CREATE_ITEM_REQ : public t_PACKETHEADER {
  uint8_t m_btSkillSLOT;                     // ����� ���� ��ų
  int8_t  m_cTargetItemTYPE;                 // ������ ������ Ÿ��( t_eITEM�� �� )
  int16_t m_nTargetItemNO;                   // ������ ������ ��ȣ
  int16_t m_nUseItemINV[ CREATE_ITEM_STEP ]; // ������ �Ҹ��� ������ �κ��丮 ��ȣ
  //	int16_t	m_nUseItemCNT[ CREATE_ITEM_STEP ];		// ������ �Ҹ��� ������ ����
};

struct gsv_CREATE_ITEM_REPLY : public t_PACKETHEADER {
  uint8_t     m_btRESULT;                       // ���...
  int16_t     m_nStepORInvIDX;                  // ���� ���н� ���е� ����, ������ �� �κ��丮��ȣ
  int16_t     m_nPRO_POINT[ CREATE_ITEM_STEP ]; // ���� ����Ʈ
  tagBaseITEM m_CreatedITEM;                    // ���� ������ ������ ������
};

#define	RESULT_CREATE_ITEM_SUCCESS				0x00	// ����
#define	RESULT_CREATE_ITEM_FAILED				0x01	// ����
#define	RESULT_CREATE_ITEM_INVALID_CONDITION	0x02	// ������ �����ϴ�.
#define	RESULT_CREATE_ITEM_NEED_ITEM			0x03	// �Ҹ�� �������� �����ϴ�
#define	RESULT_CREATE_ITEM_INVALID_ITEM			0x04	// �Ҹ�� �������� �������ϴ�
#define	RESULT_CREATE_ITEM_NEED_SKILL_LEV		0x05	// ���� ��ų ������ �����ϴ�

//struct cli_CREATE_ITEM_EXP_REQ : public t_PACKETHEADER {
//} ;

#define	REPORT_ITEM_CREATE_START				0x00	// ������ ���� ����
#define	REPORT_ITEM_CREATE_SUCCESS				0x01	// ������ ���� ����
#define	REPORT_ITEM_CREATE_FAILED				0x02	// ������ ���� ����
#define	REPORT_ITEM_UPGRADE_START				0x03	// ������ ���� ����
#define	REPORT_ITEM_UPGRADE_SUCCESS				0x04	// ������ ���� ����
#define	REPORT_ITEM_UPGRADE_FAILED				0x05	// ������ ���� ����

struct cli_ITEM_RESULT_REPORT : public t_PACKETHEADER {
  uint8_t m_btREPORT;
  uint8_t m_btItemType;
  int16_t m_nItemNO;
};

struct gsv_ITEM_RESULT_REPORT : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  uint8_t  m_btREPORT;
  uint8_t  m_btItemType;
  int16_t  m_nItemNO;
};

//---------------------------------------------------------
#define	CRAFT_GEMMING_REQ			0x01			// ���
#define CRAFT_BREAKUP_USE_SKILL		0x02			// ��ų�� ����
#define CRAFT_BREAKUP_FROM_NPC		0x03			// npc ���� ����
#define	CRAFT_UPGRADE_USE_SKILL		0x04			// ��ų�� ���
#define	CRAFT_UPGRADE_FROM_NPC		0x05			// npc ���� ���

struct cli_CRAFT_ITEM_REQ : public t_PACKETHEADER {
  uint8_t m_btTYPE;
};

struct cli_CRAFT_GEMMING_REQ : public cli_CRAFT_ITEM_REQ {
  uint8_t m_btEquipInvIDX; // ������ ���� ������ �κ� ��ȣ
  uint8_t m_btJemInvIDX;   // ���� ������ �κ� ��ȣ
};

struct cli_CRAFT_BREAKUP_REQ : public cli_CRAFT_ITEM_REQ {
  int16_t m_nSkillSLOTorNpcIDX; // ��� ��ų���� ��ȣ�Ǵ� npc��ȣ
  uint8_t m_btTargetInvIDX;     // �и��� ������ �κ� ��ȣ
};

#define	UPGRADE_ITEM_STEP		3

struct cli_CRAFT_UPGRADE_REQ : public cli_CRAFT_ITEM_REQ {
  int16_t m_nSkillSLOTorNpcIDX;                // ��� ��ų���� ��ȣ�Ǵ� npc��ȣ
  uint8_t m_btTargetInvIDX;                    // ����� ������ �κ� ��ȣ
  uint8_t m_btUseItemINV[ UPGRADE_ITEM_STEP ]; // ��ý� �Ҹ��� ������ �κ��丮 ��ȣ
};

#define	CRAFT_GEMMING_SUCCESS		0x01
#define	CRAFT_GEMMING_NEED_SOCKET	0x02	// ���Ͼ���.
#define	CRAFT_GEMMING_USED_SOCKET	0x03	// ��ֵ� ����....

#define	CRAFT_BREAKUP_SUCCESS_GEM	0x04	// ���� �и� ����
#define	CRAFT_BREAKUP_DEGRADE_GEM	0x05	// ���� �и� ����, ������� ����
#define	CRAFT_BREAKUP_CLEARED_GEM	0x06	// ���� �и� ����, ���� ������

#define	CRAFT_BREAKUP_SUCCESS		0x07	// ������ ���� ����

#define	CRAFT_UPGRADE_SUCCESS		0x10	// ��� ����
#define	CRAFT_UPGRADE_FAILED		0x11	// ��� ����
#define	CRAFT_UPGRADE_INVALID_MAT	0x12	// ��� �������� �߸��ƴ�.

struct gsv_CRAFT_ITEM_REPLY : public t_PACKETHEADER {
  uint8_t         m_btRESULT;
  uint8_t         m_btOutCNT;      // ����� ������ ����
  tag_SET_INVITEM m_sInvITEM[ 0 ]; // ����� ���� ��ŭ ��� �ִ�... 
  // ����) CRAFT_UPGRADE_SUCCESS, CRAFT_UPGRADE_FAILED �ϰ��
  // m_sInvITEM[ m_btOutCNT-1 ].m_iQuantity�� ������ ���Ȱ��� �������
};

/*
///	����Ʈ ���� ...
*/
struct cli_QUEST_REQ : public t_PACKETHEADER {
  uint8_t m_btTYPE;
  uint8_t m_btQuestSLOT;

  union {
    int32_t   m_iQuestID;
    t_HASHKEY m_TriggerHash;
  };
};

#define	TYPE_QUEST_REQ_ADD						0x01
#define	TYPE_QUEST_REQ_DEL						0x02
#define	TYPE_QUEST_REQ_DO_TRIGGER				0x03

struct gsv_QUEST_REPLY : public t_PACKETHEADER {
  uint8_t m_btResult;
  uint8_t m_btQuestSLOT;

  union {
    int32_t   m_iQuestID;
    t_HASHKEY m_TriggerHash;
  };
};

#define	RESULT_QUEST_REPLY_ADD_SUCCESS			0x01
#define	RESULT_QUEST_REPLY_ADD_FAILED			0x02
#define	RESULT_QUEST_REPLY_DEL_SUCCESS			0x03
#define	RESULT_QUEST_REPLY_DEL_FAILED			0x04
#define	RESULT_QUEST_REPLY_TRIGGER_SUCCESS		0x05		// �� ��Ŷ�� ������ Ŭ���̾�Ʈ������ ������ �����Ѵ�.
#define	RESULT_QUEST_REPLY_TRIGGER_FAILED		0x06

// ������ Ŭ���̾�Ʈ���� NPC�� ���� �ִ� �̺�Ʈ�� üũ�� ����....
struct gsv_CHECK_NPC_EVENT : public t_PACKETHEADER {
  int16_t m_nNpcIDX;
};

//--------------------------- Messenger ���� ��Ŷ
// ��Ŷ Ÿ���� CLI_MESSENGER, WSV_MESSENGER�� ����, ���� Ÿ�� m_btCMD�� ���� ��Ŷ ������ ������.
#define	FRIEND_STATUS_NORMAL		0x000		// �Ϲ�
#define FRIEND_STATUS_HUNT			0x001		// �����
#define	FRIEND_STATUS_STORE			0x002		// �����
#define	FRIEND_STATUS_QUEST			0x003		// ����Ʈ��
#define	FRIEND_STATUS_EAT			0x004		// �Ļ���
#define	FRIEND_STATUS_REST			0x005		// �޽���
#define	FRIEND_STATUS_ONLINE		0x007		// ���ӵ�		: ģ������� m_dwUserTAG�� �����ߴ�.
#define	FRIEND_STATUS_OFFLINE		0x008		// ���Ӳ���		: ģ������� m_dwUserTAG�� ���Ӳ���
#define	FRIEND_STATUS_REFUSE_MSG	0x009		// ��� �޼��� �ź�

#define	FRIEND_STATUS_REFUSED		0x080		// �� ��ȭ�� �����ߴ�
#define	FRIEND_STATUS_DELETED		0x081		// ���� ģ����Ͽ��� �����ߴ�

#define	MSGR_CMD_APPEND_REQ			0x001		// ģ�� �߰� ��û	: ����� �ݵ�� ������ �־�� ��
#define MSGR_CMD_APPEND_ACCEPT		0x002		// ģ�� �߰� ��û �³�
#define	MSGR_CMD_APPEND_REJECT		0x003		// ģ�� �߰� ��û ����
#define	MSGR_CMD_NOT_FOUND			0x004		// ģ�� �߰� ����(����� ������ �������...)
#define	MSGR_CMD_DELETE				0x005		// ģ�� ����		: ����߿��� m_dwUserTAG�� ����..
#define	MSGR_CMD_LIST				0x006		// ģ�� ����Ʈ	: ģ����� �䱸
#define	MSGR_CMD_REFUSE				0x007		// �޽��� ����	: ����߿��� m_dwUserTAG�� �޼��� �ź�
#define	MSGR_CMD_CHANGE_STATUS		0x008		// ���� ����

#define	MSGR_CMD_LOGIN				0x080		// ���������� ���
#define	MSGR_CMD_LOGOUT				0x081		// ���������� ���

struct tag_MCMD_HEADER : public t_PACKETHEADER {
  uint8_t m_btCMD;
};

struct cli_MCMD_APPEND_REQ : public tag_MCMD_HEADER {
  // ģ�� �߰� ��û�Ҷ�
  char m_szName[0];
};

struct cli_MCMD_APPEND_REPLY : public tag_MCMD_HEADER {
  // ģ�� �߰� ��û�� ���� ����
  uint16_t m_wUserIDX;
};

struct cli_MCMD_TAG : public tag_MCMD_HEADER {
  // ����, ���� ��û
  uint32_t m_dwUserTAG;
};

struct cli_MCMD_STATUS_REQ : public tag_MCMD_HEADER {
  // �� ���� ���� ��û
  uint8_t m_btStatus;
};

struct wsv_MCMD_APPEND_REQ : public tag_MCMD_HEADER {
  // ģ�� �߰� ��û�޾�����
  uint16_t m_wUserIDX;
  char     m_szName[0];
};

struct wsv_MCMD_APPEND_ACCEPT : public tag_MCMD_HEADER {
  // ģ�� �߰� �³��ϰ�� ����Ʈ�� �߰���
  uint32_t m_dwUserTAG;
  uint8_t  m_btStatus;
  char     m_szName[0];
};

struct wsv_MCMD_STATUS_REPLY : public tag_MCMD_HEADER {
  // ���������� ����:: m_dwUserTAG���� ���� �뺸
  uint32_t m_dwUserTAG;
  uint8_t  m_btStatus;
};

struct wsv_MCMD_LIST : public tag_MCMD_HEADER {
  // ģ�� ���
  uint8_t m_btFriendCNT;
  /*
  {
    wsv_MCMD_STATUS_REPLY
    m_szName[]
  } X m_btFriendCNT
  */
};

struct cli_MESSENGER_CHAT : public t_PACKETHEADER {
  uint32_t m_dwUserTAG;
  char     m_szMSG[0];
};

struct wsv_MESSENGER_CHAT : public t_PACKETHEADER {
  uint32_t m_dwUserTAG;
  char     m_szMSG[0];
};

//--------------------------- Chatting room ���� ��Ŷ
#define	CHAT_REQ_ROOM_LIST		0x001
#define	CHAT_REQ_MAKE			0x002
#define	CHAT_REQ_JOIN			0x003
#define	CHAT_REQ_LEFT			0x004
#define	CHAT_REQ_KICK_USER		0x005

#define	MAX_CHAT_ROOM_USERS		64

struct tag_CHAT_HEADER : public t_PACKETHEADER {
  uint8_t m_btCMD;
};

struct cli_CHAT_ROOM_MAKE : public tag_CHAT_HEADER {
  uint8_t m_btRoomTYPE;
  uint8_t m_btMaxUSER; // �ִ� �����ο�
  // szTitle[]
  // szPassword[]
};

struct cli_CHAT_ROOM_JOIN : public tag_CHAT_HEADER {
  uint8_t  m_btRoomTYPE;
  uint16_t m_wRoomID;
  // szPassword[]
};

struct cli_CHAT_ROOM_LIST : public tag_CHAT_HEADER {
  uint8_t  m_btRoomTYPE;
  uint16_t m_wFromRoomID;
};

struct cli_CHAT_ROOM_KICK : public tag_CHAT_HEADER {
  t_HASHKEY m_HashUSER;
};

struct tag_CHAT_ROOM {
  uint8_t m_btRoomTYPE;
  int16_t m_nRoomIDX;
  int8_t  m_cUserCNT;
  // szTitle[]
};

struct wsv_CHAT_ROOM_USER : public tag_CHAT_HEADER {
  uint16_t m_wUserID;
  // szName[]
};

/*
struct wsv_CHAT_ROOM_JOIN : public tag_CHAT_HEADER {
  uint16_t	m_wUserID;	// �ڽ�
  szRoomName[]		// ���̸�
  {
    uint16_t	m_wUserID;
    szUserName[];
  } x ???
} ;
*/
struct wsv_CHAT_ROOM_LIST : public tag_CHAT_HEADER {
  int8_t m_cRoomCNT;
  // tag_CHAT_ROOM[]
};

#define	CHAT_REPLY_ROOM_MAKED		0x001	// ����� ����			m_wUserID(�ڽ�)
#define	CHAT_REPLY_MAKE_FAILED		0x002

#define	CHAT_REPLY_ROOM_JOINED		0x010	// ���� ����			wsv_CHAT_ROOM_JOIN[]
#define	CHAT_REPLY_FULL_USERS		0x011	// �ο� ����
#define	CHAT_REPLY_INVALIED_PASS	0x012	// ��� Ʋ��
#define CHAT_REPLY_ROOM_NOT_FOUND	0x013	// �� ���� ����
#define CHAT_REPLY_USER_ADD			0x014	// ����� �߰�			m_wUserID, szName[]
#define	CHAT_REPLY_KICKED			0x015	// �߹� ���ߴ�			m_wUserID
#define CHAT_REPLY_USER_LEFT		0x016	// �濡�� ������.		m_wUserID
#define	CHAT_REPLY_USER_LEFT_NAME	0x017	// �濡�� ������.		����� �̸����� Ż��...

#define	CHAT_REPLY_ROOM_LIST		0x020
#define	CHAT_REPLY_ROOM_LIST_END	0x021

struct cli_CHATROOM_MSG : public t_PACKETHEADER {
  char m_szMSG[0];
};

struct wsv_CHATROOM_MSG : public t_PACKETHEADER {
  uint16_t m_wObjectID;
  char     m_szMSG[0];
};

//--------------------------- ���� ���� ��Ŷ
#define	MEMO_REQ_CONTENTS			0x001	// ���ŵ� ��ü ���� ���� �䱸
#define MEMO_REQ_SEND				0x002	// ���� ���� �䱸
#define MEMO_REQ_RECEIVED_CNT		0x003	// ������ ���� ���� �䱸

struct cli_MEMO : public t_PACKETHEADER {
  uint8_t m_btTYPE;
  /*
  struct tagReqMemoSend {
    // szTargetCHAR[]
    // szMemoContent[]			// 255�� �̸�
  } ;
  */
};

#define	MEMO_REPLY_RECEIVED_CNT			0x001	// ������ ���� ����
#define	MEMO_REPLY_CONTENTS				0x002	// ���� �����...
#define	MEMO_REPLY_SEND_OK				0x003	// ���� ����
#define	MEMO_REPLY_SEND_INVALID_TARGET	0x004	// ��� ����, ������ ����
#define	MEMO_REPLY_SEND_NOT_EXIST		0x005	// ����� �������� �ʴ´�.
#define	MEMO_REPLY_SEND_REFUSED			0x006	// ���� ���� �ź� ���·� ������ ����
#define MEMO_REPLY_SEND_FULL_MEMO		0x007	// ���� �������� ������ ������ ����
#define MEMO_REPLY_SEND_INVALID_CONTENT	0x008	// �������

struct wsv_MEMO : public t_PACKETHEADER {
  uint8_t m_btTYPE;
  int16_t m_nRecvCNT[0]; //  m_btTYPE == MEMO_REPLY_RECEIVED_CNT �ϰ�� ���� ���� ����
  /*
  struct tagReplyMemoCONTENTS {
    // uint32_t dwReceivedDATE			// classTIME::GetCurrentAbsSecond()�Լ��� ����� �ð���.
    // szFrom[]						// ������
    // szMemo[]						// ����
  } ;
  */
};

#define	REQ_MALL_ITEM_LIST					0x01	// ����Ʈ ��û
#define	REQ_MALL_ITEM_FIND_CHAR				0x02	// �ɸ� �̸� üũ
#define	REQ_MALL_ITEM_BRING					0x03	// ��������
#define	REQ_MALL_ITEM_GIVE					0x04	// �����ϱ�

struct cli_MALL_ITEM_REQ : public t_PACKETHEADER {
  uint8_t m_btReqTYPE;
  int16_t m_nDupCnt;     // REQ_MALL_ITEM_BRING, REQ_MALL_ITEM_GIVE�ÿ� ����..
  uint8_t m_btInvIDX[0]; // REQ_MALL_ITEM_GIVE�ϰ�� 0~39 ������ �κ��丮 ��ȣ
  // char m_szCharName[]							// REQ_MALL_ITEM_BRING, REQ_MALL_ITEM_GIVE ��û��
  // char m_szDesc[]								// REQ_MALL_ITEM_GIVE ��û�� �����ϸ�(�����Ҷ� �޼���)::�ִ� 80��
};

#define	REPLY_MALL_ITEM_CHECK_CHAR_FOUND	0x01	// ��� ã�Ҵ�
#define	REPLY_MALL_ITEM_CHECK_CHAR_NONE		0x02	// ��� ����.
#define	REPLY_MALL_ITEM_CHECK_CHAR_INVALID	0x03	// �ڱ� �ڽ� ������..�߸��� ���
#define	REPLY_MALL_ITEM_BRING_SUCCESS		0x04
#define	REPLY_MALL_ITEM_BRING_FAILED		0x05
#define	REPLY_MALL_ITME_GIVE_SUCCESS		0x06
#define	REPLY_MALL_ITEM_GIVE_FAILED			0x07
#define	REPLY_MALL_ITEM_LIST_START			0x08
#define	REPLY_MALL_ITEM_LIST_DATA			0x09
#define	REPLY_MALL_ITEM_LIST_END			0x0a
#define	REPLY_MALL_ITEM_NOT_FOUND			0x0b

struct gsv_MALL_ITEM_REPLY : public t_PACKETHEADER {
  uint8_t m_btReplyTYPE;
  uint8_t m_btCntOrIdx; // ���� / �� �κ��丮 �ε���

  /* MALL_ITEM_REQ_LIST ���� :: m_nCntOrIdx ������ŭ..
    m_btReplyTYPE == REPLY_MALL_ITEM_LIST_START
      m_btReplyTYPE == REPLY_MALL_ITEM_LIST_DATA
      {
        m_btCntOrIdx :: ����
        {
          tagBaseITEM
          szFrom[]		// ������ �ɸ�
          if ( '\0' != szFrom[0] ) {
            // szDesc[]	// �����Ҷ� �޼���
            // szTo[]	// ������ ��� �ɸ� �̸�
          }
        }
      }
    m_btReplyTYPE == REPLY_MALL_ITEM_LIST_END
  */
  // REPLY_MALL_ITEM_BRING_SUCCESS���� :: m_nCntOrIdx���� �� �κ��丮 �ε��� m_BringITEM[0]���� �߰��� ������
  union {
    tag_SET_INVITEM m_BringITEM[0]; // MALL_ITEM_REQ_BRING�� ���� ����(����� �κ��丮 ����) :: m_btCntOrIdx�� �� �κ��丮 �������� �̵�
  };
};

#define	BILLING_MSG_FREE_USER				0x00	//

//#define	BILLING_MSG_FEE_TYPE1				0x01	//  /���: ���ϴ� ���׿���� ����� �Դϴ�. (2004��06��01��)
//#define	BILLING_MSG_FEE_TYPE2				0x02	//         ���ϴ� ��������� ����� �Դϴ�. (18�ð�20��20��)

#define	BILLING_MSG_PAY_FU					0x01	// FU	�α��� �� ����ڴ� ���� ���̵� ��������� ��Ÿ����.
#define	BILLING_MSG_PAY_FA					0x02	// FA	�α��� �� ����ڴ� �������� ��������� ��Ÿ����.
#define	BILLING_MSG_PAY_FAP					0x03	// FAP	�α��� �� ����ڴ� �������� �����(�÷�Ƽ��)���� ��Ÿ����.
#define	BILLING_MSG_PAY_FQ					0x04	// FQ	�α��� �� ����ڴ� �������� ��������� ��Ÿ����.
#define	BILLING_MSG_PAY_GU					0x05	// GU	�α��� �� ����ڴ� ���ӹ� ��������� ��Ÿ����.
#define	BILLING_MSG_PAY_GQ					0x06	// GQ	���ӹ濡�� �α����� �õ��Ͽ����� ���ӹ��� ���ݱⰣ(�ð�)�� ����Ǿ� ������������ �α��� �Ǿ����� ��Ÿ����.
#define	BILLING_MSG_PAY_IQ					0x07	// IQ	���ӹ濡�� �α����� �õ��Ͽ����� �̹� ���� IP ���ڸ� ��� ����ϰ� �־� ������������ �α��� �Ǿ����� ��Ÿ����.

#define	BILLING_MSG_KICKOUT_CHEAT_DETECT	0x21	// ���ٵ� ġƮ �߰ߵǼ� ����
#define	BILLING_MSG_KICKOUT_ALREADY_LOGIN	0x22	// �̹� ���ӵ� �����̶� ���� ����
#define	BILLING_MSG_KICKOUT_TIME_EXPIRED	0x23	// ���� �ð� ����
#define	BILLING_MSG_KICKOUT_DUP_LOGIN		0x24	// �ٸ� ���� �����ؼ� ���� ����
#define BILLING_MSG_KICKOUT_NO_RIGHT		0x25	// ������ ������ ����...
#define	BILLING_MSG_KICKOUT_OUT_OF_IP		0x26	// pc�濡�� �����Ҽ� �ִ� ip ������ �ʰ�
#define	BILLING_MSG_KICKOUT_EXIST_ACCOUNT	0x27	// ���ӵǾ� �ִ� ������ ©����.

#define	BILLING_MSG_JPN_NEED_CHARGE			0x41	// �Ϻ� ���� ���� �ʿ�!!! gsv_BILLING_MESSAGE2 ��Ŷ������ ���۵ǰ� m_dwPayFlag�� �ʿ��� ���� �÷��� ���õǾ� ����

//#define	BILLING_MSG_TIME_ALERT				0x31	// ���� �ð� �뺸
//#define	BILLING_MSG_TIME_EXPIRED			0x32	// <����> ���� �ð��� �ٵǾ� ������ �����մϴ�.

#define	BILLING_MSG_TYPE_SET_DATE			0x81	// ���� ���� ����	szMsg = 200309101025
#define	BILLING_MSG_TYPE_SET_TIME			0x82	// ���� �ð� ����	szMsg = ������
#define	BILLING_MSG_TYPE_TIME_ALERT			0x83	// ���� �ð� ���	szMsg = ������

#define	BILLING_MSG_KICKOUT_TAIWAN			0xf0	// �븸 ���ݼ������� ©��� �뺸 �޾�����..
#define	BILLING_MSG_TYPE_TAIWAN				0xf1	// �븸 ���ݼ������� ���� �޼���.. �����ֱ⸸ �ϸ� ��

struct gsv_BILLING_MESSAGE : public t_PACKETHEADER {
  uint8_t m_btTYPE;
  // char m_szMsg[];
};

struct gsv_BILLING_MESSAGE2 : public t_PACKETHEADER {
  uint8_t  m_btTYPE;
  int8_t   m_cFunctionTYPE; // �Һз�( A ~ R )
  uint32_t m_dwPayFlag;
};

// �ѱ� ���� ���� ��Ʈ~
#define	PAY_FLAG_KR_FU				(0x01 << BILLING_MSG_PAY_FU)	// FU	�α��� �� ����ڴ� ���� ���̵� ��������� ��Ÿ����.
#define	PAY_FLAG_KR_FA				(0x01 << BILLING_MSG_PAY_FA)	// FA	�α��� �� ����ڴ� �������� ��������� ��Ÿ����.
#define	PAY_FLAG_KR_FAP				(0x01 << BILLING_MSG_PAY_FAP)	// FAP	�α��� �� ����ڴ� �������� �����(�÷�Ƽ��)���� ��Ÿ����.
#define	PAY_FLAG_KR_FQ				(0x01 << BILLING_MSG_PAY_FQ)	// FQ	�α��� �� ����ڴ� �������� ��������� ��Ÿ����.
#define	PAY_FLAG_KR_GU				(0x01 << BILLING_MSG_PAY_GU)	// GU	�α��� �� ����ڴ� ���ӹ� ��������� ��Ÿ����.
#define	PAY_FLAG_KR_GQ				(0x01 << BILLING_MSG_PAY_GQ)	// GQ	���ӹ濡�� �α����� �õ��Ͽ����� ���ӹ��� ���ݱⰣ(�ð�)�� ����Ǿ� ������������ �α��� �Ǿ����� ��Ÿ����.
#define	PAY_FLAG_KR_IQ				(0x01 << BILLING_MSG_PAY_IQ)	// IQ	���ӹ濡�� �α����� �õ��Ͽ����� �̹� ���� IP ���ڸ� ��� ����ϰ� �־� ������������ �α��� �Ǿ����� ��Ÿ����.

// #define	PAY_FLAG_KR_PLATINUM		( FLAG_PAY_FAP|FLAG_PAY_GU )

// �Ϻ� ���� ���� ��Ʈ~
//#define	FLAG_JPAY_CHAT			(0x01 << 0)						// 0	Free Play 	ĳ���ۼ�, ä��
#define	PAY_FLAG_JP_BATTLE			(0x01 << 0)	 					// 1	Entry	��Ʋ, ���̽� �ʵ�
#define	PAY_FLAG_JP_COMMUNITY		(0x01 << 1)	 					// 2	Community	ģ�����, ä�ù�
#define	PAY_FLAG_JP_TRADE			(0x01 << 2)	 					// 3	Trade	�ŷ�, ����
#define	PAY_FLAG_JP_STOCK_SPACE		(0x01 << 3)	 					// 4	Stock Space	��ũ
#define	PAY_FLAG_JP_EXTRA_STOCK		(0x01 << 4)	 					// 5	Extra Stock	�߰���ũ �����̽�
#define	PAY_FLAG_JP_STARSHIP_PASS	(0x01 << 5)	 					// 6	Starship Pass	�༺�� �̵�
#define	PAY_FLAG_JP_DUNGEON_ADV		(0x01 << 6)	 					// 7	Dungeon Adventure	���� �ʵ�
#define	PAY_FLAG_JP_EXTRA_CHAR		(0x01 << 7)	 					// 8	Extra Character	�ۼ����� ĳ�� �߰�

#define	PAY_FLAG_JP_ALL				(0xff << 0)						//      ��� ���� ����~

// �������¿� ���� �÷��� ���ɿ��θ� üũ�� ��Ʈ...
// �ѱ� ������ FLAG_PAY_FAP,FLAG_PAY_GU�� ���� �Ʒ� ��� ��Ʈ on 
// �̿� �ѱ� ������ PLAY_FLAG_EXTRA_STOCK, PLAY_FLAG_EXTRA_CHAR ��Ʈ off
#define	PLAY_FLAG_BATTLE			(0x01 << 0)	 					// 1	Entry	��Ʋ, ���̽� �ʵ�
#define	PLAY_FLAG_COMMUNITY			(0x01 << 1)	 					// 2	Community	ģ�����, ä�ù�
#define	PLAY_FLAG_TRADE				(0x01 << 2)	 					// 3	Trade	�ŷ�, ����
#define	PLAY_FLAG_STOCK_SPACE		(0x01 << 3)	 					// 4	Stock Space	��ũ
#define	PLAY_FLAG_EXTRA_STOCK		(0x01 << 4)	 					// 5	Extra Stock	�߰���ũ �����̽�
#define	PLAY_FLAG_STARSHIP_PASS		(0x01 << 5)	 					// 6	Starship Pass	�༺�� �̵�
#define	PLAY_FLAG_DUNGEON_ADV		(0x01 << 6)	 					// 7	Dungeon Adventure	���� �ʵ�
#define	PLAY_FLAG_EXTRA_CHAR		(0x01 << 7)	 					// 8	Extra Character	�ۼ����� ĳ�� �߰�

// �ѱ� �⺻ ����
#define	PLAY_FLAG_KOREA_DEFAULT		( PLAY_FLAG_BATTLE | PLAY_FLAG_COMMUNITY | PLAY_FLAG_TRADE | PLAY_FLAG_STOCK_SPACE | PLAY_FLAG_STARSHIP_PASS | PLAY_FLAG_DUNGEON_ADV )
#define	PLAY_FLAG_TAIWAN_DEFAULT	PLAY_FLAG_KOREA_DEFAULT

#define	EXT_BILLING_MSG_PAY_KOREA			0x1001	// �ѱ� ���� ����
#define	EXT_BILLING_MSG_PAY_JAPAN			0x1002	// �Ϻ� ���� �÷���
#define	EXT_BILLING_MSG_PAY_TAIWAN			0x1003	// �븸 ���� Ÿ�Թ�ȣ

#define	EXT_BILLING_MSG_TYPE_SET_DATE		0x1010	// ���� ���� ����	m_dwPayType = m_dwPlayingFlag = 0	szMsg = Ÿ��\t200309101025\tŸ��\t200309101025...
#define	EXT_BILLING_MSG_TYPE_SET_TIME		0x1011	// ���� �ð� ����	m_dwPayType = m_dwPlayingFlag = 0	szMsg = Ÿ��\t������\tŸ��\t������...
#define EXT_BILLING_MSG_TYPE_TIME_EXPIRED	0x1012	// PayType=0�̰� m_dwPlayingFlag[0]�� ����� ���� �÷�������
#define EXT_BILLING_MSG_TYPE_TIME_ALERT		0x1013	// PayType=0�̰� m_dwPlayingFlag[0]�� �ð�����Ǵ� ���� �÷�������

struct gsv_BILLING_MESSAGE_EXT : public t_PACKETHEADER {
  uint16_t m_wMsgTYPE;
  uint32_t m_dwPayType;
  uint32_t m_dwPlayingFlag[ 4 ]; // �����ְ�~ :: 32 * 4 = 128
  // char m_szMsg[];
};

//-------------------------------------------------------------------------------------------------
struct gsv_GODDNESS_MODE : public t_PACKETHEADER {
  uint8_t  m_btOnOff;    // ����/����
  uint16_t m_wObjectIDX; // ��� ���� �ɸ��� �ε���
};

//-------------------------------------------------------------------------------------------------
struct gsv_PATSTATE_CHANGE : public t_PACKETHEADER {
  // GSV_PATSTATE_CHANGE
  uint8_t  m_btOnOff;    // īƮ ��ȯ ����/�Ұ���(1/0) 
  uint32_t m_dwCoolTIME; // īƮ ��ȯ �Ұ��� �����̸� ���⿡ ��Ÿ�� ��
  int16_t  m_nMaxPatHP;  // īƮ ü��
  uint16_t m_wObjectIDX; // ����� īƮ ü�� 
};

//-------------------------------------------------------------------------------------------------
struct gsv_CHARSTATE_CHANGE : public t_PACKETHEADER {
  uint16_t m_wObjectIDX;
  uint32_t m_dwFLAG;
};

struct cli_CART_RIDE : public t_PACKETHEADER {
  uint8_t  m_btType;
  uint16_t m_wOwnerObjIDX; // �¿� �ƹ�Ÿ
  uint16_t m_wGuestObjIDX; // Ż �ƹ�Ÿ
};

struct gsv_CART_RIDE : public cli_CART_RIDE {};

#define	CART_RIDE_REQ				0x01	// Ż�� ?	m_wGuestObjIDX���Ը� ���۵�
#define	CART_RIDE_ACCEPT			0x02	// Ż��		�ֺ� ��� �ɸ��� ���۵�
#define	CART_RIDE_REFUSE			0x03	// ��Ż��	m_wOwnerObjIDX���Ը� ���۵�
#define	CART_RIDE_OWNER_NOT_FOUND	0x04	// �¿� �شٴ� �ɸ��� �������
#define	CART_RIDE_GUEST_NOT_FOUND	0x05	// �¿���� �ɸ��� �������

struct gsv_UPDATE_NAME : public t_PACKETHEADER {
  /*
    This update player's name
  */
  int16_t m_nCharID;
  // new name;
};

struct pxy_UPDATE_NAME : public t_PACKETHEADER {};

struct pxy_SET_RIGHTS : public t_PACKETHEADER {
  uint32_t wRIGHT;
};

//-------------------------------------------------------------------------------------------------

#if defined(__SERVER) && !defined(__SKIP_SRV_PROTOTYPE)
#include "SRV_Prototype.h"
#endif

//-------------------------------------------------------------------------------------------------
#define	__T_PACKET

struct t_PACKET {
  union {
    t_PACKETHEADER   m_HEADER;
    uint8_t          m_pDATA[ MAX_PACKET_SIZE ];
    t_NETWORK_STATUS m_NetSTATUS;

    cli_CHECK_AUTH m_cli_CHECK_AUTH;
    srv_CHECK_AUTH m_srv_CHECK_AUTH;

    lsv_ERROR m_lsv_ERROR;
    gsv_ERROR m_gsv_ERROR;

    gsv_GM_COMMAND m_gsv_GM_COMMAND;

    gsv_SET_GLOBAL_VAR  m_gsv_SET_GLOBAL_VAR;
    gsv_SET_GLOBAL_FLAG m_gsv_SET_GLOBAL_FLAG;

    cli_LOGOUT_REQ   m_cli_LOGOUT_REQ;
    gsv_LOGOUT_REPLY m_gsv_LOGOUT_REPLY;

    // CLI <-> LSV
    srv_LOGIN_REPLY m_srv_LOGIN_REPLY;

    cli_CHANNEL_LIST_REQ   m_cli_CHANNEL_LIST_REQ;
    lsv_CHANNEL_LIST_REPLY m_lsv_CHANNEL_LIST_REPLY;

    cli_SELECT_SERVER m_cli_SELECT_SERVER;
    lsv_SELECT_SERVER m_lsv_SELECT_SERVER;

    // CLI <-> WSV/GSV
    cli_JOIN_SERVER_REQ   m_cli_JOIN_SERVER_REQ;
    srv_JOIN_SERVER_REPLY m_srv_JOIN_SERVER_REPLY;
    /*
    cli_JOIN_WORLD				m_cli_JOIN_WORLD;
    wsv_JOIN_WORLD				m_wsv_JOIN_WORLD;
    */

    // CLI <-> WSV
    wsv_MOVE_SERVER m_wsv_MOVE_SERVER;

    cli_CHAR_LIST m_cli_CHAR_LIST;
    wsv_CHAR_LIST m_wsv_CHAR_LIST;

    cli_CREATE_CHAR m_cli_CREATE_CHAR;
    wsv_CREATE_CHAR m_wsv_CREATE_CHAR;

    cli_DELETE_CHAR m_cli_DELETE_CHAR;
    wsv_DELETE_CHAR m_wsv_DELETE_CHAR;

    gsv_INIT_DATA m_gsv_INIT_DATA;

    cli_SELECT_CHAR m_cli_SELECT_CHAR;
    gsv_SELECT_CHAR m_gsv_SELECT_CHAR;

    // CLI <-> GSV
    gsv_SERVER_DATA m_gsv_SERVER_DATA;
    gsv_RELAY_REQ   m_gsv_RELAY_REQ;
    cli_RELAY_REPLY m_cli_RELAY_REPLY;

    cli_JOIN_ZONE m_cli_JOIN_ZONE;
    gsv_JOIN_ZONE m_gsv_JOIN_ZONE;

    cli_REVIVE_REQ     m_cli_REVIVE_REQ;
    gsv_REVIVE_REPLY   m_gsv_REVIVE_REPLY;
    cli_SET_REVIVE_POS m_cli_SET_REVIVE_POS;

    cli_SET_VAR_REQ   m_cli_SET_VAR_REQ;
    gsv_SET_VAR_REPLY m_gsv_SET_VAR_REPLY;

    gsv_INVENTORY_DATA m_gsv_INVENTORY_DATA;
    gsv_QUEST_DATA     m_gsv_QUEST_DATA;

    gsv_QUEST_ONLY m_gsv_QUEST_ONLY;
    gsv_WISH_LIST  m_gsv_WISH_LIST;

    gsv_CHEAT_CODE m_gsv_CHEAT_CODE;

    cli_SET_MOTION m_cli_SET_MOTION;
    gsv_SET_MOTION m_gsv_SET_MOTION;

    cli_TOGGLE m_cli_TOGGLE;
    gsv_TOGGLE m_gsv_TOGGLE;

    cli_CHAT m_cli_CHAT;
    gsv_CHAT m_gsv_CHAT;

    cli_WHISPER m_cli_WHISPER;
    gsv_WHISPER m_gsv_WHISPER;

    cli_SHOUT m_cli_SHOUT;
    gsv_SHOUT m_gsv_SHOUT;

    cli_PARTY_CHAT m_cli_PARTY_CHAT;
    gsv_PARTY_CHAT m_gsv_PARTY_CHAT;

    cli_CLAN_CHAT m_cli_CLAN_CHAT;
    wsv_CLAN_CHAT m_wsv_CLAN_CHAT;

    cli_ALLIED_CHAT m_cli_ALLIED_CHAT;
    gsv_ALLIED_CHAT m_gsv_ALLIED_CHAT;

    cli_ALLIED_SHOUT m_cli_ALLIED_SHOUT;
    gsv_ALLIED_SHOUT m_gsv_ALLIED_SHOUT;

    gsv_SET_EVENT_STATUS m_gsv_SET_EVENT_STATUS;

    tag_ADD_CHAR   m_tag_ADD_CHAR;
    gsv_MOB_CHAR   m_gsv_MOB_CHAR;
    gsv_NPC_CHAR   m_gsv_NPC_CHAR;
    gsv_AVT_CHAR   m_gsv_AVT_CHAR;
    gsv_SUB_OBJECT m_gsv_SUB_OBJECT;

    cli_SET_WEIGHT_RATE m_cli_SET_WEIGHT_RATE;
    gsv_SET_WEIGHT_RATE m_gsv_SET_WEIGHT_RATE;

    gsv_ADJUST_POS m_gsv_ADJUST_POS;

    cli_CANTMOVE m_cli_CANTMOVE;
    cli_SETPOS   m_cli_SETPOS;

    cli_STOP m_cli_STOP;
    gsv_STOP m_gsv_STOP;

    cli_ATTACK m_cli_ATTACK;
    gsv_ATTACK m_gsv_ATTACK;

    cli_DAMAGE m_cli_DAMAGE;
    gsv_DAMAGE m_gsv_DAMAGE;

    gsv_SETEXP  m_gsv_SETEXP;
    gsv_LEVELUP m_gsv_LEVELUP;

    cli_HP_REQ   m_cli_HP_REQ;
    gsv_HP_REPLY m_gsv_HP_REPLY;

    gsv_SET_HPnMP m_gsv_SET_HPnMP;

    cli_MOUSECMD m_cli_MOUSECMD;
    gsv_MOUSECMD m_gsv_MOUSECMD;

    gsv_MOVE m_gsv_MOVE;

    gsv_CHANGE_NPC m_gsv_CHANGE_NPC;

    cli_STORE_TRADE_REQ   m_cli_STORE_TRADE_REQ;
    gsv_STORE_TRADE_REPLY m_gsv_STORE_TRADE_REPLY;

    gsv_SET_MONEYnINV m_gsv_SET_MONEYnINV;
    gsv_SET_INV_ONLY  m_gsv_SET_INV_ONLY;

    cli_USE_ITEM m_cli_USE_ITEM;
    gsv_USE_ITEM m_gsv_USE_ITEM;

    cli_EQUIP_ITEM m_cli_EQUIP_ITEM;
    gsv_EQUIP_ITEM m_gsv_EQUIP_ITEM;

    cli_ASSEMBLE_RIDE_ITEM m_cli_ASSEMBLE_RIDE_ITEM;
    gsv_ASSEMBLE_RIDE_ITEM m_gsv_ASSEMBLE_RIDE_ITEM;

    cli_USE_ITEM_TO_REPAIR m_cli_USE_ITEM_TO_REPAIR;
    gsv_SET_INV_ONLY       m_gsv_USED_ITEM_TO_REPAIR;

    cli_REPAIR_FROM_NPC m_cli_REPAIR_FROM_NPC;
    gsv_SET_MONEYnINV   m_gsv_REPAIRED_FROM_NPC;

    gsv_SET_ITEM_LIFE m_gsv_SET_ITEM_LIFE;

    //cli_CHANGE_SKIN				m_cli_CHANGE_SKIN;
    gsv_CHANGE_SKIN m_gsv_CHANGE_SKIN;

    cli_DROP_ITEM m_cli_DROP_ITEM;

    cli_MOVE_ITEM m_cli_MOVE_ITEM;
    gsv_MOVE_ITEM m_gsv_MOVE_ITEM;

    cli_MOVE_ZULY m_cli_MOVE_ZULY;
    gsv_MOVE_ZULY m_gsv_MOVE_ZULY;

    cli_CREATE_ITEM_REQ   m_cli_CREATE_ITEM_REQ;
    gsv_CREATE_ITEM_REPLY m_gsv_CREATE_ITEM_REPLY;

    //cli_CREATE_ITEM_EXP_REQ		m_cli_CREATE_ITEM_EXP_REQ;

    cli_ITEM_RESULT_REPORT m_cli_ITEM_RESULT_REPORT;
    gsv_ITEM_RESULT_REPORT m_gsv_ITEM_RESULT_REPORT;

    cli_BANK_LIST_REQ   m_cli_BANK_LIST_REQ;
    gsv_BANK_LIST_REPLY m_gsv_BANK_LIST_REPLY;

    gsv_ADD_FIELDITEM m_gsv_ADD_FIELDITEM;
    gsv_SUB_FIELDITEM m_gsv_SUB_FIELDITEM;

    cli_GET_FIELDITEM_REQ   m_cli_GET_FIELDITEM_REQ;
    gsv_GET_FIELDITEM_REPLY m_gsv_GET_FIELDITEM_REPLY;

    cli_TELEPORT_REQ   m_cli_TELEPORT_REQ;
    gsv_TELEPORT_REPLY m_gsv_TELEPORT_REPLY;

    cli_USE_BPOINT_REQ   m_cli_USE_BPOINT_REQ;
    gsv_USE_BPOINT_REPLY m_gsv_USE_BPOINT_REPLY;

    cli_SET_HOTICON m_cli_SET_HOTICON;
    gsv_SET_HOTICON m_gsv_SET_HOTICON;

    cli_SET_BULLET m_cli_SET_BULLET;
    gsv_SET_BULLET m_gsv_SET_BULLET;

    cli_TRADE_P2P m_cli_TRADE_P2P;
    gsv_TRADE_P2P m_gsv_TRADE_P2P;

    cli_TRADE_P2P_ITEM m_cli_TRADE_P2P_ITEM;
    gsv_TRADE_P2P_ITEM m_gsv_TRADE_P2P_ITEM;

    cli_SET_WISHITEM m_cli_SET_WISHITEM;

    cli_P_STORE_OPEN   m_cli_P_STORE_OPEN;
    gsv_P_STORE_OPENED m_gsv_P_STORE_OPENED;

    cli_P_STORE_CLOSE  m_cli_P_STORE_CLOSE;
    gsv_P_STORE_CLOSED m_gsv_P_STORE_CLOSED;

    cli_P_STORE_LIST_REQ   m_cli_P_STORE_LIST_REQ;
    gsv_P_STORE_LIST_REPLY m_gsv_P_STORE_LIST_REPLY;

    cli_P_STORE_BUY_REQ  m_cli_P_STORE_BUY_REQ;
    cli_P_STORE_SELL_REQ m_cli_P_STORE_SELL_REQ;

    gsv_P_STORE_RESULT m_gsv_P_STORE_RESULT;

    gsv_SKILL_LEARN_REPLY m_gsv_SKILL_LEARN_REPLY;

    cli_SKILL_LEVELUP_REQ   m_cli_SKILL_LEVELUP_REQ;
    gsv_SKILL_LEVELUP_REPLY m_gsv_SKILL_LEVELUP_REPLY;

    cli_SELF_SKILL m_cli_SELF_SKILL;
    gsv_SELF_SKILL m_gsv_SELF_SKILL;

    cli_TARGET_SKILL m_cli_TARGET_SKILL;
    gsv_TARGET_SKILL m_gsv_TARGET_SKILL;

    cli_POSITION_SKILL m_cli_POSITION_SKILL;
    gsv_POSITION_SKILL m_gsv_POSITION_SKILL;

    gsv_EFFECT_OF_SKILL m_gsv_EFFECT_OF_SKILL;
    gsv_DAMAGE_OF_SKILL m_gsv_DAMAGE_OF_SKILL;
    gsv_RESULT_OF_SKILL m_gsv_RESULT_OF_SKILL;
    gsv_SKILL_START     m_gsv_SKILL_START;

    gsv_SKILL_CANCEL m_gsv_SKILL_CANCEL;

    gsv_CLEAR_STATUS  m_gsv_CLEAR_STATUS;
    gsv_SPEED_CHANGED m_gsv_SPEED_CHANGED;

    cli_APPRAISAL_REQ   m_cli_APPRAISAL_REQ;
    gsv_APPRAISAL_REPLY m_gsv_APPRAISAL_REPLY;

    cli_QUEST_REQ   m_cli_QUEST_REQ;
    gsv_QUEST_REPLY m_gsv_QUEST_REPLY;

    gsv_CHECK_NPC_EVENT m_gsv_CHECK_NPC_EVENT;

    cli_PARTY_REQ m_cli_PARTY_REQ;
    gsv_PARTY_REQ m_gsv_PARTY_REQ;

    cli_PARTY_REPLY m_cli_PARTY_REPLY;
    gsv_PARTY_REPLY m_gsv_PARTY_REPLY;

    //		wsv_PARTY_USER				m_wsv_PARTY_USER;

    gsv_PARTY_MEMBER  m_gsv_PARTY_MEMBER;
    gsv_PARTY_LEVnEXP m_gsv_PARTY_LEVnEXP;
    gsv_CHANGE_OBJIDX m_gsv_CHANGE_OBJIDX;
    gsv_PARTY_ITEM    m_gsv_PARTY_ITEM;
    cli_PARTY_RULE    m_cli_PARTY_RULE;
    gsv_PARTY_RULE    m_gsv_PARTY_RULE;

    gsv_ADD_EVENTOBJ m_gsv_ADD_EVENTOBJ;
    //		gsv_SUB_EVENTOBJ			m_gsv_SUB_EVENTOBJ;

    cli_CLAN_COMMAND        m_cli_CLAN_COMMAND;
    cli_CLAN_CREATE         m_cli_CLAN_CREATE;
    cli_CLAN_MEMBER_JOBnLEV m_cli_CLAN_MEMBER_JOBnLEV;
    wsv_CLAN_MEMBER_JOBnLEV m_wsv_CLAN_MEMBER_JOBnLEV;
    wsv_CLAN_COMMAND        m_wsv_CLAN_COMMAND;
    wsv_RESULT_CLAN_SET     m_wsv_RESULT_CLAN_SET;

    tag_CHAT_HEADER m_tag_CHAT_HEADER;

    cli_CHAT_ROOM_MAKE m_cli_CHAT_ROOM_MAKE;
    cli_CHAT_ROOM_JOIN m_cli_CHAT_ROOM_JOIN;
    cli_CHAT_ROOM_LIST m_cli_CHAT_ROOM_LIST;
    cli_CHAT_ROOM_KICK m_cli_CHAT_ROOM_KICK;

    wsv_CHAT_ROOM_USER m_wsv_CHAT_ROOM_USER;
    wsv_CHAT_ROOM_LIST m_wsv_CHAT_ROOM_LIST;

    cli_CHATROOM_MSG m_cli_CHATROOM_MSG;
    wsv_CHATROOM_MSG m_wsv_CHATROOM_MSG;

    cli_MEMO m_cli_MEMO;
    wsv_MEMO m_wsv_MEMO;

    tag_MCMD_HEADER m_tag_MCMD_HEADER;

    cli_MCMD_APPEND_REQ   m_cli_MCMD_APPEND_REQ;
    cli_MCMD_APPEND_REPLY m_cli_MCMD_APPEND_REPLY;
    cli_MCMD_TAG          m_cli_MCMD_TAG;
    cli_MCMD_STATUS_REQ   m_cli_MCMD_STATUS_REQ;

    wsv_MCMD_APPEND_REQ    m_wsv_MCMD_APPEND_REQ;
    wsv_MCMD_APPEND_ACCEPT m_wsv_MCMD_APPEND_ACCEPT;
    wsv_MCMD_STATUS_REPLY  m_wsv_MCMD_STATUS_REPLY;
    wsv_MCMD_LIST          m_wsv_MCMD_LIST;

    cli_MESSENGER_CHAT m_cli_MESSENGER_CHAT;
    wsv_MESSENGER_CHAT m_wsv_MESSENGER_CHAT;

    cli_CRAFT_ITEM_REQ    m_cli_CRAFT_ITEM_REQ;
    cli_CRAFT_GEMMING_REQ m_cli_CRAFT_GEMMING_REQ;
    cli_CRAFT_BREAKUP_REQ m_cli_CRAFT_BREAKUP_REQ;
    cli_CRAFT_UPGRADE_REQ m_cli_CRAFT_UPGRADE_REQ;

    gsv_CRAFT_ITEM_REPLY m_gsv_CRAFT_ITEM_REPLY;

    gsv_SET_MONEY_ONLY m_gsv_SET_MONEY_ONLY;
    gsv_SET_ABILITY    m_gsv_SET_ABILITY;

    cli_MALL_ITEM_REQ   m_cli_MALL_ITEM_REQ;
    gsv_MALL_ITEM_REPLY m_gsv_MALL_ITEM_REPLY;

    gsv_BILLING_MESSAGE     m_gsv_BILLING_MESSAGE;
    gsv_BILLING_MESSAGE_EXT m_gsv_BILLING_MESSAGE_EXT;
    gsv_BILLING_MESSAGE2    m_gsv_BILLING_MESSAGE2;

    gsv_SET_NPC_SHOW m_gsv_SET_NPC_SHOW;

    cli_CLANMARK_SET   m_cli_CLANMARK_SET;
    cli_CLANMARK_REQ   m_cli_CLANMARK_REQ;
    wsv_CLANMARK_REPLY m_wsv_CLANMARK_REPLY;

    cli_CLANMARK_REG_TIME m_cli_CLANMARK_REG_TIME;
    wsv_CLANMARK_REG_TIME m_wsv_CLANMARK_REG_TIME;

    gsv_GODDNESS_MODE m_gsv_GODDNESS_MODE;

    cli_SCREEN_SHOT_TIME m_cli_SCREEN_SHOT_TIME;
    gsv_SCREEN_SHOT_TIME m_gsv_SCREEN_SHOT_TIME;

    cli_SUMMON_CMD m_cli_SUMMON_CMD;
    // gsv_SUMMON_CMD			m_gsv_SUMMON_CMD;
    gsv_PATSTATE_CHANGE m_gsv_PATSTATE_CHANGE;

    gsv_CHARSTATE_CHANGE m_gsv_CHARSTATE_CHANGE;
    cli_CART_RIDE        m_cli_CART_RIDE;
    gsv_CART_RIDE        m_gsv_CART_RIDE;

    gsv_UPDATE_NAME m_gsv_UPDATE_NAME;
    pxy_UPDATE_NAME m_pxy_UPDATE_NAME;
    pxy_SET_RIGHTS  m_pxy_SET_RIGHTS;

#if defined(__SERVER) && !defined(__SKIP_SRV_PROTOTYPE)
    gsv_LOG_SQL					m_gsv_LOG_SQL;

    zws_SERVER_INFO				m_zws_SERVER_INFO;

    zws_CONFIRM_ACCOUNT_REQ		m_zws_CONFIRM_ACCOUNT_REQ;
    wls_CONFIRM_ACCOUNT_REPLY	m_wls_CONFIRM_ACCOUNT_REPLY;

    zws_ADD_ACCOUNT				m_zws_ADD_ACCOUNT;
    zws_SUB_ACCOUNT				m_zws_SUB_ACCOUNT;
    zas_SUB_ACCOUNT				m_zas_SUB_ACCOUNT;
    zas_SERVER_TYPE				m_zas_SERVER_TYPE;

    zas_MESSAGE					m_zas_MESSAGE;
    zas_MESSAGE_EXT				m_zas_MESSAGE_EXT;

    zas_KICK_ACCOUNT			m_zas_KICK_ACCOUNT;
    zas_CHECK_ACCOUNT			m_zas_CHECK_ACCOUNT;

    wls_KICK_ACCOUNT			m_lws_KICK_ACCOUNT;

    gsv_BLOCK					m_gsv_BLOCK;

    lsv_CHECK_ACCOUNT			m_lsv_CHECK_ACCOUNT;

    gsv_CHANGE_CHAR				m_gsv_CHANGE_CHAR;

    srv_CHEAT					m_gsv_CHEAT_REQ;
    srv_CHEAT					m_wsv_CHEAT_REQ;

    srv_ACTIVE_MODE				m_srv_ACTIVE_MODE;
    srv_USER_LIMIT				m_srv_USER_LIMIT;
    wls_CHANNEL_LIST			m_wls_CHANNEL_LIST;

    wls_ACCOUNT_LIST			m_wls_ACCOUNT_LIST;
    zws_ACCOUNT_LIST			m_zws_ACCOUNT_LIST;

    srv_SET_WORLD_VAR			m_srv_SET_WORLD_VAR;

    mon_SERVER_STATUS_REQ		m_mon_SERVER_STATUS_REQ;
    srv_SERVER_LIST_REPLY		m_srv_SERVER_LIST_REPLY;
    wls_SERVER_STATUS_REPLY		m_wls_SERVER_STATUS_REPLY;
    gsv_SERVER_STATUS_REPLY		m_gsv_SERVER_STATUS_REPLY;

    ost_SERVER_USERLOGOUT		m_ost_SERVER_USERLOGOUT;
    gsv_SERVER_USERLOGOUT_REPLY m_gsv_SERVER_USERLOGOUT_REPLY;
    ost_SERVER_ZONEINFO			m_ost_SERVER_ZONEINFO;
    gsv_SERVER_ZONEINFO_REPLY	m_gsv_SERVER_ZONEINFO_REPLY;
    ost_SERVER_USERINFO			m_ost_SERVER_USERINFO;
    gsv_SERVER_USERINFO_REPLY	m_gsv_SERVER_USERINFO_REPLY;
    ost_SERVER_CHGUSER			m_ost_SERVER_CHGUSER;
    gsv_SERVER_CHGUSER_REPLY	m_gsv_SERVER_CHGUSER_REPLY;
    ost_SERVER_MOVECHR			m_ost_SERVER_MOVECHR;
    gsv_SERVER_MOVECHR_REPLY	m_gsv_SERVER_MOVECHR_REPLY;
    ost_SERVER_ZONEANNOUNCE		m_ost_SERVER_ZONEANNOUNCE;
    ost_SERVER_IPSEARCH			m_ost_SERVER_IPSEARCH;
    gsv_SERVER_IPSEARCH_REPLY	m_gsv_SERVER_IPSEARCH_REPLY;

    tag_ZWS_CLAN_PACKET			m_zws_CLAN_PACKET;
    zws_CREATE_CLAN				m_zws_CREATE_CLAN;
    zws_SET_USER_CLAN			m_zws_SET_USER_CLAN;
    zws_DEL_USER_CLAN			m_zws_DEL_USER_CLAN;

    gsv_ADJ_CLAN_VAR			m_gsv_ADJ_CLAN_VAR;
    wsv_SET_CLAN_VAR			m_wsv_SET_CLAN_VAR;

    bkd_MAKE_ACCOUNT			m_bkd_MAKE_ACCOUNT;
    bkd_MOD_ACCOUNT				m_bkd_MOD_ACCOUNT;
/*
    gsv_ADD_ZONE_LIST			m_gsv_ADD_ZONE_LIST;

    gsv_DEL_ZONE				m_gsv_DEL_ZONE;
    wsv_DEL_ZONE				m_wsv_DEL_ZONE;

    gsv_WARP_USER				m_gsv_WARP_USER;
    srv_DO_TRIGGER_NCPOBJ		m_srv_DO_TRIGGER_NPCOBJ;
    srv_DO_TRIGGER_EVENTOBJ		m_srv_DO_TRIGGER_EVENTOBJ;

    srv_SET_NPCOBJ_VAR			m_srv_SET_NPCOBJ_VAR;
    srv_SET_EVENTOBJ_VAR		m_srv_SET_EVENTOBJ_VAR;

    gsv_LEVEL_UP				m_gsv_LEVEL_UP;
    wsv_PARTY_CMD				m_wsv_PARTY_CMD;
  */
#endif
  };
};

//-------------------------------------------------------------------------------------------------
#pragma pack (pop)
#pragma warning( default:4201 )
#pragma warning( default:4200 )
#endif	// __NET_PROTOTYPE_H
