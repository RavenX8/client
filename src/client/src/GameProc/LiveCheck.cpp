#include "StdAfx.h"
#include "./LiveCheck.h"
#include "../Network/CNetwork.h"
#include "cli_alive.h"

/// for singleton
//CLiveCheck _liveCheck;

CLiveCheck::CLiveCheck(void) {
  m_dwElapsedTime            = 0;
  m_dwLastRecoveryUpdateTime = g_GameDATA.GetGameTime();
}

CLiveCheck::~CLiveCheck(void) {}

//----------------------------------------------------------------------------------------------
/// 2�п� �ѹ��� ������ ��Ŷ �۽�
//----------------------------------------------------------------------------------------------
void        CLiveCheck::Check() {
  const int iCheckTime = 270000;
  //--------------------------------------------------------------------------------
  /// Update check frame
  //--------------------------------------------------------------------------------
  DWORD dwCurrentTime        = g_GameDATA.GetGameTime();
  DWORD dwFrameElapsedTime   = dwCurrentTime - m_dwLastRecoveryUpdateTime; /// ���������ӿ��� ���� ������ ���̿� �帥�ð��� �����ش�.
  m_dwLastRecoveryUpdateTime = dwCurrentTime;

  m_dwElapsedTime += dwFrameElapsedTime;

  if ( m_dwElapsedTime > iCheckTime ) {
    g_pNet->Send_PACKET(RoseCommon::Packet::CliAlive::create());
    LogString(LOG_NORMAL, "Send Live Packet[ %d ]", dwCurrentTime / 1000);

    m_dwElapsedTime -= iCheckTime;
  }
}

void CLiveCheck::ResetTime() {
  m_dwElapsedTime            = 0;
  m_dwLastRecoveryUpdateTime = g_GameDATA.GetGameTime();
}
