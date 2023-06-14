#ifndef UNGROUPCOMAND_H
#define UNGROUPCOMAND_H

#include <QUndoCommand>
#include "pcboard.h"


class UngroupComand : public QUndoCommand
{
   PcBoard *m_pBoard;
   SmartPtr<GraphicalItem> m_pContainer;
   vector<SmartPtr<GraphicalItem> > m_Items;
public:
   UngroupComand(PcBoard *pBoard,SmartPtr<GraphicalItem>& pCont);
   virtual ~UngroupComand() override;
   void undo() override;
   void redo() override;
};

#endif // UNGROUPCOMAND_H
