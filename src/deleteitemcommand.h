#ifndef DELETEITEMCOMMAND_H
#define DELETEITEMCOMMAND_H

#include <QUndoCommand>
#include "pcboard.h"

class DeleteItemCommand : public QUndoCommand
{
    vector<SmartPtr<GraphicalItem>> m_Items;
    PcBoard *m_pBoard;
public:
    DeleteItemCommand(SmartPtr<GraphicalItem>& p,PcBoard *pBoard);
    DeleteItemCommand(const vector<SmartPtr<GraphicalItem>>& p,PcBoard *pBoard);
    DeleteItemCommand(vector<SmartPtr<GraphicalItem>>&& p,PcBoard *pBoard);
    virtual ~DeleteItemCommand() override;
    void undo() override;
    void redo() override;
};

#endif // DELETEITEMCOMMAND_H
