#include "groupcommand.h"

GroupCommand::GroupCommand(GraphicalItemsMap&& m,PcBoard *p):m_map(m),m_board(p)
{
   for(auto item:m_map)
   {
      item.second->setSelected(false);
   }
}

GroupCommand::~GroupCommand()
{
}

void GroupCommand::redo()
{
   QString name;
   if(!m_pContainer.get())
   {
      m_board->groupCommand(m_map,m_pContainer);
      //increment refs for a created container

   }
   else
   {
      m_board->regroupCommand(m_pContainer,
                              m_map);
//      m_pContainer->setZoom(m_board->getZoomValue());
   }
}

void GroupCommand::undo()
{
   m_board->ungroupCommand(m_pContainer);
}
