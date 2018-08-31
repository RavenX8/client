#ifndef _CTCandidateWindow_
#define _CTCandidateWindow_

#include "winctrl.h"

//---------------------------------------------------------------------------------------------------------------------
/// IME에서의 Candidate Window Classes의 Base Class
/// -CTIme
///
/// @Author			최종진
/// @Date			2005/8/30
//---------------------------------------------------------------------------------------------------------------------
class CTCandidateWindow : public CWinCtrl {
public:
           CTCandidateWindow(void );
  virtual  ~CTCandidateWindow(void);
  unsigned Process(unsigned       uiMsg, WPARAM wParam, LPARAM lParam) override;

  void         Draw() override {};
  virtual void AddCandidate(const char* pszCandidate) = 0;
  virtual void Clear() = 0;
  virtual void SetSelection(short n) {}
};
#endif
