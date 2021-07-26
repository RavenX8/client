#ifndef _LIVE_CHECK_
#define _LIVE_CHECK_

#include "../Util/JSingleton.h"

class CLiveCheck {
private:
  DWORD m_dwElapsedTime;
  DWORD m_dwLastRecoveryUpdateTime;

  CLiveCheck(void );
public:
  ~CLiveCheck(void);

  CLiveCheck(const CLiveCheck&) = delete;
  CLiveCheck& operator=(const CLiveCheck &) = delete;
  CLiveCheck(CLiveCheck &&) = delete;
  CLiveCheck & operator=(CLiveCheck &&) = delete;

  static auto& GetSingleton(){
    static CLiveCheck inst;
    return inst;
  }

  void Check();
  void ResetTime();
};

#endif
