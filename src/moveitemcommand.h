#ifndef MOVEITEMCOMMAND_H
#define MOVEITEMCOMMAND_H

#include <QUndoCommand>
#include "pcboard.h"


class MoveItemCommand : public QUndoCommand
{
    vector<SmartPtr<GraphicalItem> > m_items;
    PointF m_diff;
    PcBoard *m_board;
public:
    MoveItemCommand(GraphicalItemsMap& map,PointF& diff,PcBoard *p);
    virtual ~MoveItemCommand() override;
    void undo() override;
    void redo() override;
};

#endif // MOVEITEMCOMMAND_H
