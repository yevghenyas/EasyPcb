#include "deleteitemcommand.h"

DeleteItemCommand::DeleteItemCommand(SmartPtr<GraphicalItem>& p,PcBoard *pBoard):
                                       m_pBoard(pBoard)
{
   m_Items.push_back(p);
}

DeleteItemCommand::DeleteItemCommand(vector<SmartPtr<GraphicalItem>>&& p,PcBoard *pBoard):
                                       m_Items(p),m_pBoard(pBoard)
{
}

DeleteItemCommand::DeleteItemCommand(const vector<SmartPtr<GraphicalItem>>& p,PcBoard *pBoard):
                                       m_Items(p),m_pBoard(pBoard)
{
}


DeleteItemCommand::~DeleteItemCommand()
{

}

void DeleteItemCommand::redo()
{
   for(auto& item:m_Items)
      m_pBoard->deleteGraphicalItem(item);
   m_pBoard->repaint();
}

void DeleteItemCommand::undo()
{
//   m_pItem->setZoom(m_pBoard->getZoomValue());
    for(auto& item:m_Items)
       m_pBoard->addGraphicalItemToLevels(item);
   m_pBoard->repaint();
}
