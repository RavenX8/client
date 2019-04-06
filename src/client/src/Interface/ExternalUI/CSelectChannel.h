#ifndef _CSelectChannel_
#define _CSelectChannel_
#include "tgamectrl/TDialog.h"
class CExternalUIManager;
struct tagCHANNEL_SRV;

class CSelectChannel : public CTDialog {
public:
          CSelectChannel(void );
  virtual ~CSelectChannel(void);

  unsigned Process(UINT uiMsg, WPARAM wParam, LPARAM lParam) override;
  void     Show() override;

  void SetEUIObserver(CExternalUIManager* pObserver);

private:
};
#endif
