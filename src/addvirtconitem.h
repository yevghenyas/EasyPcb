#ifndef ADDVIRTCONITEM_H
#define ADDVIRTCONITEM_H

#include <QUndoCommand>
#include "common.h"
#include <set>

class AddVirtConItem : public QUndoCommand
{
   GraphicalItem *m_pItem;
   set<ITEM_ID> connectedItems;
public:
   AddVirtConItem(GraphicalItem *p,set<ITEM_ID>&& ids);
   virtual ~AddVirtConItem() override;
   void virtual undo() override;
   void virtual redo() override;
};

#endif // ADDVIRTCONITEM_H
