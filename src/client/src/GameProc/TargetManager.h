#ifndef _TARGET_MANAGER_
#define _TARGET_MANAGER_

#include "../Util/JSingleton.h"

class CTargetManager {
private:
  int m_iCurrentMouseTargetObject; /// ���콺 �̵��� ���� ���콺�� ����Ű�� ������Ʈ	

  CTargetManager(void );
public:
  ~CTargetManager(void);

  CTargetManager(const CTargetManager&) = delete;
  CTargetManager& operator=(const CTargetManager &) = delete;
  CTargetManager(CTargetManager &&) = delete;
  CTargetManager & operator=(CTargetManager &&) = delete;

  static auto& GetSingleton(){
    static CTargetManager inst;
    return inst;
  }

  void SetMouseTargetObject(int iObjectIDX);

  void Proc();
  void Draw();
};

#endif //_TARGET_MANAGER_
