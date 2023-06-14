#include "movebetweenlayerscommand.h"

MoveBetweenLayersCommand::MoveBetweenLayersCommand(SmartPtr<GraphicalItem>& p,const QString& n,PcBoard *pBoard,
                                                   BOARD_LEVEL_ID newLevel,
                                                   ContainerType type):m_pItem(p),name(n),
                                                   m_pBoard(pBoard),level(newLevel),m_containerTyoe(type)
{
   int i = 0;
}

MoveBetweenLayersCommand::~MoveBetweenLayersCommand()
{
}

void MoveBetweenLayersCommand::redo()
{
   //just go to the previous level
   undo();
}

void MoveBetweenLayersCommand::undo()
{
   BOARD_LEVEL_ID prevLevel = m_pItem->getLevel();
   m_pBoard->moveBetweenLayers(level,name,m_pItem,m_containerTyoe);
   level = prevLevel;
}
