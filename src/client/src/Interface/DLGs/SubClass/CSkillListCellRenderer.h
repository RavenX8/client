#ifndef _CSkillListCellRenderer_
#define _CSkillListCellRenderer_
#include "tgamectrl/IListcellrenderer.h"

class CSkillListCellRenderer : public IListCellRenderer {
public:
            CSkillListCellRenderer(void                  );
  virtual   ~CSkillListCellRenderer(void                 );
  CWinCtrl* GetListCellRendererComponent(const CTObject* pObject, bool selected, bool mouseover) override;

};
#endif
