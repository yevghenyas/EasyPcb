#ifndef GROUPUNGROUPCOMMAND_H
#define GROUPUNGROUPCOMMAND_H

#include <QUndoCommand>
#include "pcboard.h"


class  GroupCommand : public QUndoCommand
{
   //items to group
   GraphicalItemsMap m_map;
   PcBoard *m_board;
   //name and pointer to container
   SmartPtr<GraphicalItem> m_pContainer;
public:
   GroupCommand(GraphicalItemsMap&& m,PcBoard *p);
   virtual ~GroupCommand() override;
   void redo() override;
   void undo() override;
};

#endif // GROUPUNGROUPCOMMAND_H
