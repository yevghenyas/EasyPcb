#ifndef AUTOCOMMANDITEM_H
#define AUTOCOMMANDITEM_H

#include <QUndoCommand>
#include "pcboard.h"

using ConnectorMap = map<QString,SmartPtr<GraphicalItem>>;
using MultiMap = map<ITEM_ID,vector<SmartPtr<GraphicalItem>>>;
using ConstructedLayer = multimap<QString,SmartPtr<GraphicalItem>>;


class AutoCommandItem : public QUndoCommand
{
   ConstructedLayer conMap;
   MultiMap multiMap;
   vector<SmartPtr<GraphicalItem>> vcCons;
   PcBoard *m_pBoard;
public:
   AutoCommandItem(ConstructedLayer&& cMap,MultiMap&& mMap,PcBoard *pBoard);
   virtual ~AutoCommandItem() override;
   void undo() override;
   void redo() override;

};

#endif // AUTOCOMMANDITEM_H
