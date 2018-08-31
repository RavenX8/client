#ifndef _CAvatarState_
#define _CAvatarState_

class CJustModelAVT;

class CAvatarState {
public:
               CAvatarState(void     );
  virtual      ~CAvatarState(void    );
  virtual void Enter(CJustModelAVT*  pAvt) = 0;
  virtual void Update(CJustModelAVT* pAvt) = 0;
  virtual void Leave(CJustModelAVT*  pAvt) = 0;
};

class CAvatarStateNormal : public CAvatarState {
public:
          CAvatarStateNormal(void ) {}
  virtual ~CAvatarStateNormal(void) {}
  void    Enter(CJustModelAVT*    pAvt) override;
  void    Update(CJustModelAVT*   pAvt) override {}
  void    Leave(CJustModelAVT*    pAvt) override {}
};

class CAvatarStateSitdown : public CAvatarState {
public:
          CAvatarStateSitdown(void ) {}
  virtual ~CAvatarStateSitdown(void) {}
  void    Enter(CJustModelAVT*     pAvt) override;
  void    Update(CJustModelAVT*    pAvt) override;
  void    Leave(CJustModelAVT*     pAvt) override {}
};

class CAvatarStateSit : public CAvatarState {
public:
          CAvatarStateSit(void  ) {}
  virtual ~CAvatarStateSit(void ) {}
  void    Enter(CJustModelAVT*  pAvt) override;
  void    Update(CJustModelAVT* pAvt) override {}
  void    Leave(CJustModelAVT*  pAvt) override {}
};

class CAvatarStateStandup : public CAvatarState {
public:
          CAvatarStateStandup(void ) {}
  virtual ~CAvatarStateStandup(void) {}
  void    Enter(CJustModelAVT*     pAvt) override;
  void    Update(CJustModelAVT*    pAvt) override;
  void    Leave(CJustModelAVT*     pAvt) override {}
};
#endif
