#include "setpropscommand.h"



SetPropsCommand::SetPropsCommand(SmartPtr<GraphicalItem>& p,shared_ptr<PointF>& pos,
                                 shared_ptr<GeomCommonProps>& props,PcBoard *pBoard):m_pItem(p),
                                 m_pos(pos),m_geom(props),
                                 m_prevPos(m_pItem->x(),m_pItem->y()),
                                 m_prevProps(m_pItem->getGeometry()),m_board(pBoard)
{
}

SetPropsCommand::~SetPropsCommand()
{
}

void SetPropsCommand::redo()
{
   if(m_board)
       m_board->setItemPosAndGeometry(m_pItem.get(),*m_pos,m_geom.get());
   else
   {
      if(m_pos.get())
      {
         m_pItem->setX(m_pos->x());
         m_pItem->setY(m_pos->y());
      }
      if(m_geom.get())
      {
         m_pItem->setGeometry(*m_geom);
      }
   }
}

void SetPropsCommand::undo()
{
   if(m_board)
       m_board->setItemPosAndGeometry(m_pItem.get(),m_prevPos,&m_prevProps);
   else
   {
      if(m_pos.get())
      {
         m_pItem->setX(m_prevPos.x());
         m_pItem->setY(m_prevPos.y());
      }
      if(m_geom.get())
      {
         m_pItem->setGeometry(m_prevProps);
      }
   }
}
