 #include "addnodecommand.h"

AddNodeCommand::AddNodeCommand(NodeChangeItem& node,PcBoard *p):
                               m_pItem(node.m_pItem),m_index(node.m_index),m_pBoard(p)
{
   node.clear();
}

AddNodeCommand::~AddNodeCommand()
{
}

void AddNodeCommand::redo()
{
   m_pItem->insertNode(m_index);
}

void AddNodeCommand::undo()
{
   PointF pt;
   m_pItem->removeNode(m_index,pt);
}
