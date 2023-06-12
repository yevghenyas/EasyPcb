/*
 * Copyright 2017 <copyright holder> <email>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "graphicalitem.h"
#include "drawwrapper.h"


GraphicalItem::GraphicalItem(BOARD_LEVEL_ID level,float x,float y,ITEM_ID id):
m_id(id),m_parent(nullptr),m_bVisible(false),m_level(level),m_selected(false),m_x(x),m_y(y)
                                              //static_cast<ITEM_ID>(-1))
{

}
void GraphicalItem::moveDown(BOARD_LEVEL_ID )
{
   m_y += 1.0f/PIXELS_PER_MM;
}

void GraphicalItem::moveLeft(BOARD_LEVEL_ID )
{
   m_x -= 1.0f/PIXELS_PER_MM;
}

void GraphicalItem::moveRight(BOARD_LEVEL_ID )
{
   m_x += 1.0f/PIXELS_PER_MM;
}

void GraphicalItem::moveUp(BOARD_LEVEL_ID )
{
   m_y -= 1.0f/PIXELS_PER_MM;
}


bool GraphicalItem::isSelected()
{
   return m_selected;
}

void GraphicalItem::setSelected(bool select)
{
   m_selected = select;
}

void GraphicalItem::setX(float x)
{
   m_x = x;
}

void GraphicalItem::setY(float y)
{
   m_y = y;
}

void GraphicalItem::moveFor(BOARD_LEVEL_ID , PointF dif)
{
   m_x += dif.x();
   m_y += dif.y();
}

void GraphicalItem::getLevels(set<BOARD_LEVEL_ID>& setIds)
{
   setIds.insert(m_level);   
}

void GraphicalItem::coordRelativeToAbsolute(float& x, float& y)
{
   if(m_parent != nullptr)
   {
      x = m_x + m_parent->x();
      y = m_y + m_parent->y();
   }
   else
   {
      x = m_x;
      y = m_y;
   }
}

void GraphicalItem::coordAbsoluteToRelative(float& x, float& y)
{
   if(m_parent != nullptr)
   {
      m_x = x - m_parent->x();
      m_y = y - m_parent->y();
   }
   else
   {
      m_x = x;
      m_y = y;
   }
}

float GraphicalItem::abs_x()
{
   if(m_dragCoord.get() != nullptr)
      return m_dragCoord->x();

   float x;
   if(m_parent)
   {
      return m_x + m_parent->x();
   }
   else
   {
      return m_x;
   }
}

float GraphicalItem::abs_y()
{
   if(m_dragCoord.get() != nullptr)
      return m_dragCoord->y();

   float y;
   if(m_parent)
   {
      return m_y + m_parent->y();
   }
   else
   {
      return m_y;
   }
}




void GraphicalItem::unsetParent(bool )
{
   coordRelativeToAbsolute(m_x,m_y);
   m_parent = nullptr;
}

void GraphicalItem::setParent(GraphicalItem* p,bool bRecalcCoord)
{
   m_parent = p;
   if(bRecalcCoord)
      coordAbsoluteToRelative(m_x,m_y);
   
}

GraphicalItem* GraphicalItem::isConnectable(PointF& pt)
{
   return nullptr;
}

/*
void GraphicalItem::paintItemWithConnector(QPainter &p, QColor &c, BOARD_LEVEL_ID idLevel)
{
   paintItem(p,c,idLevel);
   DrawWrapper::drawConnectorPlate(p,c,m_x,m_y,k_zoom);
}
*/

bool GraphicalItem::isAboveTheCenter(PointF &pt)
{
  float x1,y1;
  coordRelativeToAbsolute(x1,y1);
  if(pt.x() > (x1 -  minGraularity * 4) && pt.x() < (x1 +  minGraularity * 4) &&
     pt.y() > (y1 -  minGraularity * 4) && pt.y() < (y1 +  minGraularity * 4))
  {
      pt.setX(x1);
      pt.setY(y1);
      return true;
  }
  return false;
}

void GraphicalItem::setDragCoord(PointF& pt)
{
   if(m_dragCoord.get() == nullptr)
   {
      m_dragCoord.reset(new PointF(pt.x(),pt.y()));
   }
   else
   {
      m_dragCoord->setX(pt.x());
      m_dragCoord->setY(pt.y());
   }
}
