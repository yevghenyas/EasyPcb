#include "addvirtconitem.h"

AddVirtConItem::AddVirtConItem(GraphicalItem *p,set<ITEM_ID>&& ids):m_pItem(p),connectedItems(std::move(ids))
{

}

AddVirtConItem::~AddVirtConItem()
{

}

void AddVirtConItem::redo()
{
   
}

void AddVirtConItem::undo()
{

}
