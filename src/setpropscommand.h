#ifndef SETPROPSCOMMAND_H
#define SETPROPSCOMMAND_H

#include <QUndoCommand>
#include "pcboard.h"



class SetPropsCommand : public QUndoCommand
{
    SmartPtr<GraphicalItem> m_pItem;
    shared_ptr<PointF> m_pos;
    shared_ptr<GeomCommonProps> m_geom;
    PointF m_prevPos;
    GeomCommonProps m_prevProps;
    PcBoard *m_board;
public:
    SetPropsCommand(SmartPtr<GraphicalItem>& p,shared_ptr<PointF>& pos,shared_ptr<GeomCommonProps>& props,PcBoard *pBoard);
    virtual ~SetPropsCommand() override;
    void redo() override;
    void undo() override ;
};

#endif // SETPROPSCOMMAND_H
