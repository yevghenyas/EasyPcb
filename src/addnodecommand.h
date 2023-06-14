#ifndef ADDNODECOMMAND_H
#define ADDNODECOMMAND_H

#include <QUndoCommand>
#include "pcboard.h"



class AddNodeCommand : public QUndoCommand
{
   SmartPtr<GraphicalItem> m_pItem;
   int m_index{-1};
   PcBoard *m_pBoard;
public:
   AddNodeCommand(NodeChangeItem& node,PcBoard *p);
   virtual ~AddNodeCommand() override;
   void redo() override;
   void undo() override;
};

#endif // ADDNODECOMMAND_H
