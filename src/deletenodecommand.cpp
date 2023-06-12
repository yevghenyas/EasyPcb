#include "deletenodecommand.h"

DeleteNodeCommand::DeleteNodeCommand(NodeChangeItem& node,PcBoard *p):
    m_pItem(node.m_pItem),m_index(node.m_index),m_pBoard(p),m_point(node.m_startPoint)
{
   node.clear();
}

DeleteNodeCommand::~DeleteNodeCommand()
{
}

void DeleteNodeCommand::redo()
{
   PointF pt;
   if(m_pItem->removeNode(m_index,pt))
       m_point = pt;;
}

void DeleteNodeCommand::undo()
{
   m_pItem->insertNode(m_index);
   m_pItem->moveNode(m_point,m_index);
}
