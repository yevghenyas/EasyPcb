#include "rotateitemcommand.h"

RotateItemCommand::RotateItemCommand(SmartPtr<GraphicalItem>& p,PcBoard *pBoard):
                                      m_pItem(p),m_pBoard(pBoard)
{

}

RotateItemCommand::~RotateItemCommand()
{

}

void RotateItemCommand::redo()
{
   m_pItem->rotate(LEVEL_ALL,-1,-1,90 * 16);
}

void RotateItemCommand::undo()
{
   m_pItem->rotate(LEVEL_ALL,-1,-1,-90 * 16 );
}
