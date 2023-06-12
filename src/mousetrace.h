#ifndef MOUSETRACE_H
#define MOUSETRACE_H

#include "common.h"
#include "smartptr.h"
#include "graphicalitem.h"

class MouseTrace
{
    PointF m_coord{-1,-1};
    SmartPtr<GraphicalItem> m_item;
public:
   MouseTrace();
   MouseTrace(const MouseTrace& t);
   void connect(SmartPtr<GraphicalItem>& p);
   void disconnect();
   bool operator==(MouseTrace& m);
   bool isConnected();
   PointF getCoord();
   void setCoord(const PointF& pt);
   ITEM_ID getConnectedItemId(){return isConnected() ? m_item.get()->getID() : ID_NONE;}
   SmartPtr<GraphicalItem> getConnectedItem(){m_item;}
};

#endif // MOUSETRACE_H
