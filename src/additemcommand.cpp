#include "additemcommand.h"

AddItemCommand::AddItemCommand(SmartPtr<GraphicalItem>& p,const QString& n,
                               PcBoard *pBoard):m_pItem(p),m_pBoard(pBoard)
{
}

AddItemCommand::~AddItemCommand()
{
}


void AddItemCommand::redo()
{
   //first update zoom
//   m_pItem->setZoom(m_pBoard->getZoomValue());
   m_pBoard->addGraphicalItemToLevels(m_pItem);
   m_pBoard->repaint();
}

void AddItemCommand::undo()
{
   m_pBoard->deleteGraphicalItem(m_pItem);
   m_pBoard->repaint();
}
