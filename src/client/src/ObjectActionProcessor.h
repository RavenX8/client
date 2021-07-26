#ifndef _OBJECTACTIONPROCESS_
#define _OBJECTACTIONPROCESS_

#include "Util/JSingleton.h"

class CActionProcessChain;

//------------------------------------------------------------------------------------------------
/// Managing for CActionProcessChain
//------------------------------------------------------------------------------------------------
class CObjectActionProcessor {
private:
  CActionProcessChain* m_pRootActionChain;

  CObjectActionProcessor(void );
public:
  ~CObjectActionProcessor(void);

  CObjectActionProcessor(const CObjectActionProcessor&) = delete;
  CObjectActionProcessor& operator=(const CObjectActionProcessor &) = delete;
  CObjectActionProcessor(CObjectActionProcessor &&) = delete;
  CObjectActionProcessor & operator=(CObjectActionProcessor &&) = delete;

  static auto& GetSingleton(){
    static CObjectActionProcessor inst;
    return inst;
  }

  void MakeChain();
  void ClearChain();

  void SetNextChain(CActionProcessChain* pChain);
  void ProcessChain(HNODE                hNode);
};

//------------------------------------------------------------------------------------------------
/// Interface for "getUserdata function"
//------------------------------------------------------------------------------------------------
class CActionProcessChain {
protected:
  CActionProcessChain* m_pNextActionChain;

public:
          CActionProcessChain(void );
  virtual ~CActionProcessChain(void);

  virtual void DeleteNextChian();
  virtual void AddNextChain(CActionProcessChain* pNext);
  virtual void ProcessChain(HNODE                hNode) = 0 { *(int*)nullptr = 10; };
};

//------------------------------------------------------------------------------------------------
/// Warp object process
//------------------------------------------------------------------------------------------------
class CWarpObjectActionProcessor : public CActionProcessChain {
public:
  CWarpObjectActionProcessor(void );
  ~CWarpObjectActionProcessor(void);

  void ProcessChain(HNODE hNode) override;
};

//------------------------------------------------------------------------------------------------
/// Warp object process
//------------------------------------------------------------------------------------------------
class CEventObjectActionProcessor : public CActionProcessChain {
public:
  CEventObjectActionProcessor(void );
  ~CEventObjectActionProcessor(void);

  void ProcessChain(HNODE hNode) override;
};

#endif //_OBJECTACTIONPROCESS_
