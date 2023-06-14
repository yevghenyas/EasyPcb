#ifndef ADDITEMCOMMAND_H
#define ADDITEMCOMMAND_H

#include <QUndoCommand>
#include "pcboard.h"

class AddItemCommand : public QUndoCommand
{
    SmartPtr<GraphicalItem> m_pItem;
    PcBoard *m_pBoard;
public:
    AddItemCommand(SmartPtr<GraphicalItem>& p,const QString& n,PcBoard *pBoard);
    virtual ~AddItemCommand() override;
    void undo() override;
    void redo() override;
};

#endif // ADDITEMCOMMAND_H
