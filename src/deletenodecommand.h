#ifndef DELERENIODECOMMAND_H
#define DELERENIODECOMMAND_H

#include <QUndoCommand>
#include "pcboard.h"

class DeleteNodeCommand : public QUndoCommand
{
    SmartPtr<GraphicalItem> m_pItem;
    int m_index{-1};
    PcBoard *m_pBoard;
    PointF m_point;
public:
    DeleteNodeCommand(NodeChangeItem& node,PcBoard *p);
    virtual ~DeleteNodeCommand() override;
    void redo() override;
    void undo() override;
};

#endif // DELERENIODECOMMAND_H
