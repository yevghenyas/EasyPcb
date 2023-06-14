#include "movenodecommand.h"

MoveNodeCommand::MoveNodeCommand(SmartPtr<GraphicalItem>& pItem,PointF& pt_orig,PointF& pt,int index,PcBoard *pBoard):
                                  m_pItem(pItem),m_prevPos(pt_orig),m_pos(pt),m_nodeIntex(index),m_pBoard(pBoard)
{
}

MoveNodeCommand::MoveNodeCommand(NodeChangeItem& node,PcBoard *pBoard):m_pItem(node.m_pItem),
                                   m_prevPos(node.m_startPoint),m_pos(node.m_finishPoint),
                                 m_nodeIntex(node.m_index),m_pBoard(pBoard)
{
}

MoveNodeCommand::~MoveNodeCommand()
{

}

void MoveNodeCommand::redo()
{
   m_pItem->moveNode(m_pos,m_nodeIntex);
   m_pBoard->repaint();
}

void MoveNodeCommand::undo()
{
   m_pItem->moveNode(m_prevPos,m_nodeIntex);
   m_pBoard->repaint();
}
