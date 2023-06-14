#include "moveitemcommand.h"

MoveItemCommand::MoveItemCommand(GraphicalItemsMap& map,PointF& diff,PcBoard *p):
                                  m_diff(diff),m_board(p)
{
   for(auto& item:map)
   {
      m_items.emplace_back(item.second);
   }
}

MoveItemCommand::~MoveItemCommand()
{
}

void MoveItemCommand::redo()
{
   for(auto item:m_items)
   {
      item->moveFor(LEVEL_ALL,m_diff);
//      m_board->notifyVcConnectors(item,m_diff.x(),m_diff.y());
   }
   m_board->repaint();

}

void MoveItemCommand::undo()
{
    PointF diff(-m_diff.x(),-m_diff.y());
    for(auto item:m_items)
    {
       item->moveFor(LEVEL_ALL,diff);
    }
    m_board->repaint();
}
