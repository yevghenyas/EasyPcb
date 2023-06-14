#ifndef MOVENODECOMMAND_H
#define MOVENODECOMMAND_H


#include <QUndoCommand>
#include "pcboard.h"


class MoveNodeCommand : public QUndoCommand
{
    SmartPtr<GraphicalItem> m_pItem;
    PointF m_prevPos;
    PointF m_pos;
    int m_nodeIntex{-1};
    PcBoard *m_pBoard{nullptr};
public:
    MoveNodeCommand(SmartPtr<GraphicalItem>& pItem,PointF& pt_orig,PointF& pt,int index,PcBoard *pBoard);
    MoveNodeCommand(NodeChangeItem& node,PcBoard *pBoard);
    virtual ~MoveNodeCommand() override;
    void redo() override;
    void undo() override;
};

#endif // MOVENODECOMMAND_H
