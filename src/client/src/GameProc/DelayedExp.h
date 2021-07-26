#ifndef _DELAYED_EXP_
#define _DELAYED_EXP_

#include <list>
#include "../Util/JSingleton.h"

struct EXP_DATA {
  int iOwnerIndex;
  int64_t iAddExp;
  int64_t iExp;
  int iGetTime; /// ������ ���� �����ð�. 

  int iStamina; /// ���¹̳ʵ� ���� ���¹̳ʰ� �������� ������⶧����..
};

//------------------------------------------------------------------------------------------
///
/// class CDelayedExp
/// Synchronizing display time of getting exp with real mob dead time
///
//------------------------------------------------------------------------------------------

class CDelayedExp {
private:
  std::list<EXP_DATA> m_ExpData;

  CDelayedExp(void );
public:
  ~CDelayedExp(void);

  CDelayedExp(const CDelayedExp&) = delete;
  CDelayedExp& operator=(const CDelayedExp &) = delete;
  CDelayedExp(CDelayedExp &&) = delete;
  CDelayedExp & operator=(CDelayedExp &&) = delete;

  static auto& GetSingleton(){
    static CDelayedExp inst;
    return inst;
  }

  void PushEXPData(int iOwnerIndex, int64_t iAddExp, int64_t iEXP, int iStamina);
  void GetExp(int      iOwnerIndex);

  void Proc();
};

#endif //_DELAYED_EXP_
