#include "mousetrace.h"

MouseTrace::MouseTrace()
{

}

MouseTrace::MouseTrace(const MouseTrace& m):m_coord(m.m_coord)
{
   m_item = m.m_item;
}

void MouseTrace::connect(SmartPtr<GraphicalItem>& p)
{
   m_item = p;
}

void MouseTrace::disconnect()
{
   m_item.reset(nullptr);
}

bool MouseTrace::isConnected()
{
   return m_item.get() != nullptr;
}

PointF MouseTrace::getCoord()
{
   PointF pt(m_coord);
   if(isConnected())
   {
     pt.setX(m_item.get()->abs_x());
     pt.setY(m_item.get()->abs_y());
   }
   return pt;
}

void MouseTrace::setCoord(const PointF& pt)
{
   m_coord = pt;
}

bool MouseTrace::operator==( MouseTrace& m)
{
   return m_item.get() == m.m_item.get();
}
